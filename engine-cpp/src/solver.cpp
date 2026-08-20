#include "resurge/solver.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace resurge {

namespace {

double clamp01(double v) { return std::max(0.0, std::min(1.0, v)); }

std::string escape_json(const std::string& s) {
    std::ostringstream out;
    for (char c : s) {
        switch (c) {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default: out << c;
        }
    }
    return out.str();
}

} // namespace

RegionDecision CounterfactualSolver::decide(
    std::uint64_t offset,
    std::uint64_t length,
    const std::vector<Candidate>& candidates,
    const std::vector<Evidence>& evidence) const {

    RegionDecision decision;
    decision.offset = offset;
    decision.length = length;

    if (candidates.empty()) {
        decision.level = TruthLevel::Lost;
        return decision;
    }

    std::map<std::string, Evidence> evidence_by_id;
    for (const auto& e : evidence) evidence_by_id[e.id] = e;

    double posterior_sum = 0.0;
    for (const auto& candidate : candidates) {
        CandidateScore score;
        score.candidate_id = candidate.id;
        score.eliminated = candidate.hard_contradiction;
        score.contradiction = candidate.hard_contradiction ? 1.0 : 0.0;

        double support_acc = clamp01(candidate.physical_support) * 2.0;
        double support_weight = 2.0;

        for (const auto& c : candidate.constraints) {
            auto it = evidence_by_id.find(c.evidence_id);
            const double reliability = it == evidence_by_id.end() ? 0.5 : clamp01(it->second.reliability);
            const double independence = it == evidence_by_id.end() ? 0.5 : clamp01(it->second.independence);
            const double w = std::max(0.0, c.weight) * (0.25 + 0.75 * reliability) * (0.25 + 0.75 * independence);

            if (c.satisfied) {
                score.satisfied++;
                support_acc += w;
                support_weight += w;
            } else {
                score.failed++;
                score.contradiction += w;
                if (reliability >= 0.92 && c.weight >= 0.9) score.eliminated = true;
            }
        }

        score.support = support_weight > 0.0 ? clamp01(support_acc / support_weight) : 0.0;
        if (!score.eliminated) {
            const double penalty = std::exp(-2.4 * score.contradiction);
            score.posterior = std::max(1e-12, score.support * penalty);
            posterior_sum += score.posterior;
        }
        decision.scores.push_back(score);
    }

    if (posterior_sum <= 0.0) {
        decision.level = TruthLevel::Lost;
        decision.normalized_entropy = 1.0;
        return decision;
    }

    std::vector<double> probabilities;
    for (auto& score : decision.scores) {
        if (!score.eliminated) score.posterior /= posterior_sum;
        probabilities.push_back(score.eliminated ? 0.0 : score.posterior);
    }
    decision.normalized_entropy = normalized_entropy(probabilities);

    auto best = std::max_element(decision.scores.begin(), decision.scores.end(),
        [](const CandidateScore& a, const CandidateScore& b) { return a.posterior < b.posterior; });

    std::size_t survivors = std::count_if(decision.scores.begin(), decision.scores.end(),
        [](const CandidateScore& s) { return !s.eliminated && s.posterior > 1e-9; });

    decision.confidence = best->posterior;
    if (survivors == 1 && best->posterior > 0.999999) {
        decision.level = TruthLevel::Proven;
        decision.chosen_candidate = best->candidate_id;
    } else if (best->posterior >= 0.97 && decision.normalized_entropy <= 0.2) {
        decision.level = TruthLevel::Corroborated;
        decision.chosen_candidate = best->candidate_id;
    } else if (best->posterior >= 0.70) {
        decision.level = TruthLevel::Inferred;
        decision.chosen_candidate = best->candidate_id;
    } else if (survivors > 1) {
        decision.level = TruthLevel::Ambiguous;
    } else {
        decision.level = TruthLevel::Lost;
    }

    if (decision.chosen_candidate) {
        const auto& chosen = *std::find_if(candidates.begin(), candidates.end(), [&](const Candidate& c) {
            return c.id == *decision.chosen_candidate;
        });
        for (const auto& c : chosen.constraints) {
            if (c.satisfied && !c.evidence_id.empty()) decision.proof_evidence.push_back(c.evidence_id);
        }
        std::sort(decision.proof_evidence.begin(), decision.proof_evidence.end());
        decision.proof_evidence.erase(std::unique(decision.proof_evidence.begin(), decision.proof_evidence.end()), decision.proof_evidence.end());
    }

    return decision;
}

double CounterfactualSolver::normalized_entropy(const std::vector<double>& probabilities) {
    std::vector<double> p;
    for (double v : probabilities) if (v > 0.0) p.push_back(v);
    if (p.size() <= 1) return 0.0;
    const double total = std::accumulate(p.begin(), p.end(), 0.0);
    if (total <= 0.0) return 1.0;
    double h = 0.0;
    for (double v : p) {
        const double q = v / total;
        h -= q * std::log2(q);
    }
    return clamp01(h / std::log2(static_cast<double>(p.size())));
}

std::vector<ReadPriority> AcquisitionPlanner::rank(const std::vector<ReadTarget>& targets) const {
    std::vector<ReadPriority> ranked;
    ranked.reserve(targets.size());
    for (const auto& t : targets) {
        const double gain = std::max(0.0, t.expected_information_gain);
        const double impact = std::max(0.0, t.dependency_impact);
        const double uniqueness = std::max(0.0, t.evidence_uniqueness);
        const double stress = std::max(0.02, t.media_stress);
        ranked.push_back({t, (gain * (0.5 + impact) * (0.5 + uniqueness)) / stress});
    }
    std::stable_sort(ranked.begin(), ranked.end(), [](const auto& a, const auto& b) {
        return a.priority > b.priority;
    });
    return ranked;
}

FusionResult MultiCloneFusion::fuse(const std::vector<CloneObservation>& observations) const {
    if (observations.empty()) return {};
    const auto n = observations.front().bytes.size();
    for (const auto& obs : observations) {
        if (obs.bytes.size() != n || obs.byte_confidence.size() != n) {
            throw std::invalid_argument("clone observations must have equal byte/confidence lengths");
        }
    }

    FusionResult out;
    out.bytes.resize(n, 0);
    out.confidence.resize(n, 0.0);
    out.ambiguous.resize(n, false);

    for (std::size_t i = 0; i < n; ++i) {
        std::map<std::uint8_t, double> support;
        double total = 0.0;
        for (const auto& obs : observations) {
            const double c = clamp01(obs.byte_confidence[i]);
            support[obs.bytes[i]] += c;
            total += c;
        }
        auto best = std::max_element(support.begin(), support.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        out.bytes[i] = best->first;
        out.confidence[i] = total > 0.0 ? clamp01(best->second / total) : 0.0;

        double second = 0.0;
        for (const auto& [byte, value] : support) if (byte != best->first) second = std::max(second, value);
        out.ambiguous[i] = second > 0.0 && std::abs(best->second - second) <= 0.15 * std::max(best->second, second);
    }
    return out;
}

std::string truth_level_name(TruthLevel level) {
    switch (level) {
        case TruthLevel::Physical: return "PHYSICAL";
        case TruthLevel::Proven: return "PROVEN";
        case TruthLevel::Corroborated: return "CORROBORATED";
        case TruthLevel::Inferred: return "INFERRED";
        case TruthLevel::Ambiguous: return "AMBIGUOUS";
        case TruthLevel::Lost: return "LOST";
    }
    return "LOST";
}

std::string decision_to_json(const RegionDecision& d) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(6);
    out << "{\"offset\":" << d.offset
        << ",\"length\":" << d.length
        << ",\"level\":\"" << truth_level_name(d.level) << "\""
        << ",\"confidence\":" << d.confidence
        << ",\"normalized_entropy\":" << d.normalized_entropy;
    if (d.chosen_candidate) out << ",\"candidate\":\"" << escape_json(*d.chosen_candidate) << "\"";
    out << ",\"proof_evidence\":[";
    for (std::size_t i = 0; i < d.proof_evidence.size(); ++i) {
        if (i) out << ',';
        out << '\"' << escape_json(d.proof_evidence[i]) << '\"';
    }
    out << "],\"scores\":[";
    for (std::size_t i = 0; i < d.scores.size(); ++i) {
        if (i) out << ',';
        const auto& s = d.scores[i];
        out << "{\"candidate\":\"" << escape_json(s.candidate_id) << "\""
            << ",\"support\":" << s.support
            << ",\"contradiction\":" << s.contradiction
            << ",\"posterior\":" << s.posterior
            << ",\"eliminated\":" << (s.eliminated ? "true" : "false") << '}';
    }
    out << "]}";
    return out.str();
}

} // namespace resurge

#pragma once
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace resurge {

enum class TruthLevel { Physical, Proven, Corroborated, Inferred, Ambiguous, Lost };

enum class EvidenceKind {
    PhysicalRead,
    FileSystem,
    Journal,
    Checksum,
    FileFormat,
    Replica,
    Temporal,
    Neighbor,
    Parity,
    Application
};

struct Evidence {
    std::string id;
    EvidenceKind kind;
    std::string source;
    double reliability{1.0};
    double independence{1.0};
    std::string statement;
};

struct ConstraintResult {
    std::string constraint_id;
    bool satisfied{false};
    double weight{1.0};
    std::string evidence_id;
    std::string explanation;
};

struct Candidate {
    std::string id;
    std::vector<std::uint8_t> bytes;
    std::vector<ConstraintResult> constraints;
    double physical_support{0.0};
    bool hard_contradiction{false};
};

struct CandidateScore {
    std::string candidate_id;
    double support{0.0};
    double contradiction{0.0};
    double posterior{0.0};
    std::size_t satisfied{0};
    std::size_t failed{0};
    bool eliminated{false};
};

struct RegionDecision {
    std::uint64_t offset{0};
    std::uint64_t length{0};
    TruthLevel level{TruthLevel::Lost};
    std::optional<std::string> chosen_candidate;
    double confidence{0.0};
    double normalized_entropy{1.0};
    std::vector<CandidateScore> scores;
    std::vector<std::string> proof_evidence;
};

struct ReadTarget {
    std::uint64_t offset{0};
    std::uint64_t length{0};
    double expected_information_gain{0.0};
    double dependency_impact{0.0};
    double evidence_uniqueness{0.0};
    double media_stress{0.0};
};

struct ReadPriority {
    ReadTarget target;
    double priority{0.0};
};

struct CloneObservation {
    std::string clone_id;
    std::vector<std::uint8_t> bytes;
    std::vector<double> byte_confidence;
};

struct FusionResult {
    std::vector<std::uint8_t> bytes;
    std::vector<double> confidence;
    std::vector<bool> ambiguous;
};

} // namespace resurge

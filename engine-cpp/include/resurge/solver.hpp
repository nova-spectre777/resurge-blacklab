#pragma once
#include "resurge/model.hpp"
#include <vector>

namespace resurge {

class CounterfactualSolver {
public:
    RegionDecision decide(std::uint64_t offset,
                          std::uint64_t length,
                          const std::vector<Candidate>& candidates,
                          const std::vector<Evidence>& evidence) const;

    static double normalized_entropy(const std::vector<double>& probabilities);
};

class AcquisitionPlanner {
public:
    std::vector<ReadPriority> rank(const std::vector<ReadTarget>& targets) const;
};

class MultiCloneFusion {
public:
    FusionResult fuse(const std::vector<CloneObservation>& observations) const;
};

std::string truth_level_name(TruthLevel level);
std::string decision_to_json(const RegionDecision& decision);

} // namespace resurge

#include "resurge/solver.hpp"
#include <iostream>

using namespace resurge;

int main() {
    std::vector<Evidence> evidence = {
        {"e-physical", EvidenceKind::PhysicalRead, "clone-a", 0.82, 0.95, "unstable repeated read"},
        {"e-journal", EvidenceKind::Journal, "ext4-journal", 0.99, 0.93, "journal references candidate block"},
        {"e-crc", EvidenceKind::Checksum, "sqlite-page", 0.995, 0.98, "page checksum/structure constraint"},
        {"e-neighbor", EvidenceKind::Neighbor, "extent-neighbor", 0.90, 0.72, "extent continuity"}
    };
    Candidate a{"candidate-A", {0x52,0x45,0x53,0x55,0x52,0x47,0x45}, {
        {"journal-link", true, 1.0, "e-journal", "journal sequence remains valid"},
        {"crc", true, 1.0, "e-crc", "structured page validates"},
        {"neighbor", true, 0.7, "e-neighbor", "neighboring extent agrees"}
    }, 0.78, false};
    Candidate b{"candidate-B", {0x52,0x45,0x53,0x58,0x52,0x47,0x45}, {
        {"journal-link", true, 1.0, "e-journal", "journal sequence remains valid"},
        {"crc", false, 1.0, "e-crc", "structured page fails"},
        {"neighbor", true, 0.7, "e-neighbor", "neighboring extent agrees"}
    }, 0.80, false};

    CounterfactualSolver solver;
    auto decision = solver.decide(4096, 7, {a,b}, evidence);
    std::cout << decision_to_json(decision) << "\n";

    AcquisitionPlanner planner;
    auto ranked = planner.rank({
        {8192, 4096, 0.92, 0.95, 0.96, 0.18},
        {12288, 4096, 0.15, 0.30, 0.40, 0.05},
        {16384, 4096, 0.61, 0.70, 0.80, 0.25}
    });
    std::cerr << "next-read=" << ranked.front().target.offset << " priority=" << ranked.front().priority << "\n";
    return 0;
}

#include "resurge/solver.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace resurge;

int main() {
    std::vector<Evidence> e = {
        {"strong", EvidenceKind::Checksum, "fixture", 0.999, 0.999, "strong independent check"},
        {"weak", EvidenceKind::Neighbor, "fixture", 0.70, 0.40, "weak neighbor"}
    };
    Candidate good{"good", {1,2,3}, {{"c1",true,1.0,"strong","ok"},{"c2",true,0.5,"weak","ok"}}, 0.9, false};
    Candidate bad{"bad", {1,2,4}, {{"c1",false,1.0,"strong","bad"},{"c2",true,0.5,"weak","ok"}}, 0.9, false};
    CounterfactualSolver solver;
    auto d = solver.decide(0,3,{good,bad},e);
    assert(d.chosen_candidate.has_value());
    assert(*d.chosen_candidate == "good");
    assert(d.level == TruthLevel::Proven || d.level == TruthLevel::Corroborated);
    assert(d.normalized_entropy < 0.1);

    AcquisitionPlanner p;
    auto ranked = p.rank({{10,1,0.9,0.9,0.9,0.2},{20,1,0.4,0.5,0.5,0.1}});
    assert(ranked.size()==2);
    assert(ranked[0].priority >= ranked[1].priority);

    MultiCloneFusion f;
    auto fused = f.fuse({
        {"a",{1,2,3},{0.9,0.9,0.9}},
        {"b",{1,8,3},{0.8,0.2,0.8}},
        {"c",{1,2,9},{0.7,0.8,0.1}}
    });
    assert((fused.bytes == std::vector<std::uint8_t>{1,2,3}));
    assert(fused.confidence[0] > 0.99);
    assert(CounterfactualSolver::normalized_entropy({1.0,0.0}) == 0.0);
    std::cout << "engine-cpp tests passed\n";
}

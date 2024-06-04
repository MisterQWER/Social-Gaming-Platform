#include "ParallelFor.h"

ParallelFor::ParallelFor(const std::string_view& list_element, const std::vector<StringExpressionTypePair>& list, RuleContainer sub_rules) :
    list_element(list_element),
    list(list),
    sub_rules(sub_rules) {
        printParallelFor();
    }

void ParallelFor::addEventsTo(GameInstance& game) {
    std::cout << "execute parallel for" << std::endl;
    for (const auto& rule : sub_rules.getSubRules()) {
        //rule->execute();
    }
}

void ParallelFor::printParallelFor() {
    std::cout << "List element: " << list_element << '\n';
    for(auto expPair : list) {
        std::cout << "exp pair = " << expPair.first << '\n';
    }
}

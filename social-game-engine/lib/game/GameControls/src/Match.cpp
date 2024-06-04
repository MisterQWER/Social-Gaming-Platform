#include "Match.h"

MatchCase::MatchCase(const std::vector<StringExpressionTypePair>& case_condition, RuleContainer sub_rules) :
    case_condition(case_condition), sub_rules(sub_rules) {}

void MatchCase::executeMatch() const {
    std::cout << "execute match case" << std::endl;
    for (const auto& rule : sub_rules.getSubRules()) {
        //rule->execute();
    }
}

Match::Match(const std::vector<StringExpressionTypePair>& match_expression, const std::vector<MatchCase>& match_cases) :
    match_expression(match_expression), match_cases(match_cases) {}

void Match::addEventsTo(GameInstance& game) {
    std::cout << "execute match" << std::endl;
    for (const auto& match_case : match_cases) {
        match_case.executeMatch();
    }
}

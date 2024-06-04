#ifndef RULES_H
#define RULES_H

#include <iostream>
#include <unordered_map>
#include <functional>

#include "Common.h"
#include "BaseRule.h"
#include "Discard.h"
#include "Message.h"
#include "InputChoice.h"
#include "For.h"
#include "ParallelFor.h"
#include "Scores.h"
#include "Extend.h"
#include "Match.h"
#include "Assignment.h"
#include "RuleContainer.h"
#include "Value.h"

using StringExpressionTypePair = std::pair<std::string_view, ExpressionType>;

namespace GameControl {
    constexpr std::string_view Discard = "discard";
    constexpr std::string_view Message = "message";
    constexpr std::string_view InputChoice = "input_choice";
    constexpr std::string_view For = "for";
    constexpr std::string_view ParallelFor = "parallel_for";
    constexpr std::string_view Match = "match";
    constexpr std::string_view Scores = "scores";
    constexpr std::string_view Extend = "extend";
    constexpr std::string_view Assignment = "assignment";
}

class RulesExtraction {
    public:
        using RuleExtractor = std::function<std::unique_ptr<BaseRule>(const ts::Node&, const std::string_view&)>;

        RulesExtraction();

        std::unique_ptr<BaseRule> traverseRulesRecursively(const ts::Node& node, const std::string_view& source_code);
        
    private:
        // Map of rule type to extraction method
        std::unordered_map<std::string_view, RuleExtractor> rule_extractors;

        void registerExtractor(const std::string_view& rule_type, RuleExtractor extractor);

        // Helper methods to extract sub-rules for specific rule types
        RuleContainer extractSubRules(const ts::Node& node, const std::string_view& source_code);
        MatchCase extractMatchCase(const ts::Node& node, const std::string_view& source_code);
        
        // Extraction methods for various rule types
        std::unique_ptr<BaseRule> extractDiscard(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractMessage(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractInputChoice(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractForAndParallel(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractMatch(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractScores(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractExtend(const ts::Node& node, const std::string_view& source_code);
        std::unique_ptr<BaseRule> extractAssignment(const ts::Node& node, const std::string_view& source_code);
};

class Rules {
    public:
        Rules(const ts::Node& rules_node, const std::string_view& source_code);

        const std::vector<std::unique_ptr<BaseRule>>& getGameRules() const { return game_rules; };

    private:
        std::vector<std::unique_ptr<BaseRule>> game_rules;
        void appendNewRule(std::unique_ptr<BaseRule> new_rule);
        void extractGameRules(const ts::Node& node, const std::string_view& source_code);
};

#endif 
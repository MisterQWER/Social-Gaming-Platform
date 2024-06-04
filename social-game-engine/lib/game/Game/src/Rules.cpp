#include "Rules.h"

////////////////////////////////// Rules Implementation ///////////////////////////////////////////////

Rules::Rules(const ts::Node& rules_node, const std::string_view& source_code) {
    assert(rules_node.getType() == "rules");
    extractGameRules(rules_node, source_code);
}

void Rules::appendNewRule(std::unique_ptr<BaseRule> new_rule) {
    game_rules.push_back(std::move(new_rule));
}

void Rules::extractGameRules(const ts::Node& node, const std::string_view& source_code) {
    ts::Node rules_body = node.getChildByFieldName("body");
    RulesExtraction rule_extraction;
    
    for (uint32_t i = 0; i < rules_body.getNumNamedChildren(); i++) {
        ts::Node child = rules_body.getNamedChild(i);
        std::string_view node_type = child.getType();

        if (node_type != "comment") {
            std::unique_ptr<BaseRule> new_rule = rule_extraction.traverseRulesRecursively(child, source_code);
            // retrieve the directed children only
            appendNewRule(std::move(new_rule));
        }
    }
}


//////////////////////////////////// Extraction Implementation //////////////////////////////////////////

RulesExtraction::RulesExtraction() {
    // Initialize with all default extractors
    registerExtractor(GameControl::Discard,     std::bind(&RulesExtraction::extractDiscard, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::Message,     std::bind(&RulesExtraction::extractMessage, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::InputChoice, std::bind(&RulesExtraction::extractInputChoice, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::For,         std::bind(&RulesExtraction::extractForAndParallel, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::ParallelFor, std::bind(&RulesExtraction::extractForAndParallel, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::Match,       std::bind(&RulesExtraction::extractMatch, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::Scores,      std::bind(&RulesExtraction::extractScores, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::Extend,      std::bind(&RulesExtraction::extractExtend, this, std::placeholders::_1, std::placeholders::_2));
    registerExtractor(GameControl::Assignment,  std::bind(&RulesExtraction::extractAssignment, this, std::placeholders::_1, std::placeholders::_2));
}

void RulesExtraction::registerExtractor(const std::string_view& rule_type, RuleExtractor extractor) {
    rule_extractors[rule_type] = extractor;
}

std::unique_ptr<BaseRule> RulesExtraction::traverseRulesRecursively(const ts::Node& node, const std::string_view& source_code) {
    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        ts::Node child = node.getNamedChild(i);
        std::string_view node_type = child.getType();

        auto rule_extractor = rule_extractors.find(node_type);
        if (rule_extractor != rule_extractors.end()) {
            return rule_extractor->second(child, source_code);
        } else {
            return traverseRulesRecursively(child, source_code);
        }
    }

    return nullptr;
}

// Extracting subrules within For, Parallel For and Match cases
RuleContainer RulesExtraction::extractSubRules(const ts::Node& node, const std::string_view& source_code) {
    RuleContainer container;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        ts::Node child = node.getNamedChild(i);
        std::string_view node_type = child.getType();

        if (node_type != "comment") {
            std::unique_ptr<BaseRule> new_rule = traverseRulesRecursively(child, source_code);
            container.addRule(std::move(new_rule));
        }
    }

    return container;
}

std::unique_ptr<BaseRule> RulesExtraction::extractDiscard(const ts::Node& node, const std::string_view& source_code) {
    std::vector<StringExpressionTypePair> int_expression;
    std::vector<StringExpressionTypePair> name_list;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "expression") {
            int_expression = extractExpression(source_code, child);
        } else if (child.getType() == "qualified_identifier") {
            name_list = extractExpression(source_code, child);
        }
    }

    return std::make_unique<Discard>(int_expression, name_list);
}

std::unique_ptr<BaseRule> RulesExtraction::extractMessage(const ts::Node& node, const std::string_view& source_code) {
    std::vector<StringExpressionTypePair> player_set;
    std::string_view message;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "player_set") {
            player_set = extractExpression(source_code, child);
        } else if (child.getType() == "expression") {
            std::vector<StringExpressionTypePair> expression = extractExpression(source_code, child);
            if (expression.size() != 0)
                message = expression.at(0).first;
        }
    }

    return std::make_unique<Message>(player_set, message);
}

std::unique_ptr<BaseRule> RulesExtraction::extractInputChoice(const ts::Node& node, const std::string_view& source_code) {
    ts::Node player_node = node.getChildByFieldName("player");
    ts::Node prompt_node = node.getChildByFieldName("prompt");
    ts::Node choices_node = node.getChildByFieldName("choices");
    ts::Node target_node = node.getChildByFieldName("target");
    ts::Node timer_node = node.getChildByFieldName("timeout");

    std::string_view player = player_node.getSourceRange(source_code);
    std::vector<StringExpressionTypePair> prompt = extractExpression(source_code, prompt_node);
    std::vector<StringExpressionTypePair> choices = extractExpression(source_code, choices_node);
    std::vector<StringExpressionTypePair> target = extractExpression(source_code, target_node);

    int timer = 0;
    if (timer_node.isNull() == false) {
        timer = std::stoi(std::string(timer_node.getSourceRange(source_code)));
    }

    return std::make_unique<InputChoice>(player, prompt, choices, target, timer);
}

std::unique_ptr<BaseRule> RulesExtraction::extractForAndParallel(const ts::Node& node, const std::string_view& source_code) {
    std::string_view list_element;
    std::vector<StringExpressionTypePair> list;
    RuleContainer sub_rules;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "identifier") {
            list_element = child.getSourceRange(source_code);
        } else if (child.getType() == "expression") {
            list = extractExpression(source_code, child);
        } else if (child.getType() == "body") {
            sub_rules = extractSubRules(child, source_code);
        }
    }

    if (node.getType() == GameControl::For) {
        return std::make_unique<For>(list_element, list, sub_rules);
    } else {
        return std::make_unique<ParallelFor>(list_element, list, sub_rules);
    }
}

MatchCase RulesExtraction::extractMatchCase(const ts::Node& node, const std::string_view& source_code) {
    ts::Node condition = node.getChildByFieldName("guard");
    ts::Node case_body = node.getChildByFieldName("body");

    std::vector<StringExpressionTypePair> case_condition = extractExpression(source_code, condition);
    RuleContainer sub_rules = extractSubRules(case_body, source_code);

    return MatchCase(case_condition, sub_rules);
}

std::unique_ptr<BaseRule> RulesExtraction::extractMatch(const ts::Node& node, const std::string_view& source_code) {
    std::vector<StringExpressionTypePair> match_expression;     // field after "match"
    std::vector<MatchCase> match_cases;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "expression") {
            match_expression = extractExpression(source_code, child);
        } else if (child.getType() == "match_entry") {
            match_cases.push_back(extractMatchCase(child, source_code));
        }
    }

    return std::make_unique<Match>(match_expression, match_cases);
}

std::unique_ptr<BaseRule> RulesExtraction::extractScores(const ts::Node& node, const std::string_view& source_code) {
    ts::Node keys_node = node.getChildByFieldName("keys").getChildByFieldName("elements");
    std::vector<StringExpressionTypePair> keys_list = extractExpression(source_code, keys_node);

    return std::make_unique<Scores>(keys_list);
}

std::unique_ptr<BaseRule> RulesExtraction::extractExtend(const ts::Node& node, const std::string_view& source_code) {
    std::vector<StringExpressionTypePair> name_list;
    std::vector<StringExpressionTypePair> list_expression;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "expression") {
            list_expression = extractExpression(source_code, child);
        } else if (child.getType() == "qualified_identifier") {
            name_list = extractExpression(source_code, child);
        }
    }

    return std::make_unique<Extend>(name_list, list_expression);
}

std::unique_ptr<BaseRule> RulesExtraction::extractAssignment(const ts::Node& node, const std::string_view& source_code) {
    std::vector<StringExpressionTypePair> target;
    std::vector<StringExpressionTypePair> expression;

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
        const ts::Node child = node.getNamedChild(i);

        if (child.getType() == "expression") {
            expression = extractExpression(source_code, child);
        } else if (child.getType() == "qualified_identifier") {
            target = extractExpression(source_code, child);
        }
    }

    return std::make_unique<Extend>(target, expression);
}
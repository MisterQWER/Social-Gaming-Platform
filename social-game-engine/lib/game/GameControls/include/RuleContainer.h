#ifndef RULE_CONTAINER_H
#define RULE_CONTAINER_H

#include "BaseRule.h"

class RuleContainer {
    public:
        void addRule(std::shared_ptr<BaseRule> rule) {
            sub_rules.emplace_back(rule);
        }

        const std::vector<std::shared_ptr<BaseRule>>& getSubRules() const { return sub_rules; }

    private:
        std::vector<std::shared_ptr<BaseRule>> sub_rules;
};

#endif
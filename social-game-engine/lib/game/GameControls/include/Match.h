#include "BaseRule.h"
#include "RuleContainer.h"

class MatchCase {
    public:
        MatchCase(const std::vector<StringExpressionTypePair>& case_condition, RuleContainer sub_rules);

        void executeMatch() const;

        std::vector<StringExpressionTypePair> case_condition;
        RuleContainer sub_rules;
};

class Match : public BaseRule {
    public:
        Match(const std::vector<StringExpressionTypePair>& match_expression, const std::vector<MatchCase>& match_cases);

        void addEventsTo(GameInstance& game) override;

        // For type checking 
        uintptr_t GetType() override {
            return Type();
        } 

        static uintptr_t Type() {
            return reinterpret_cast<uintptr_t>(&type);
        }

        void accept(RuleVisitor& visitor) const override {
            visitor.visit(*this);
        }

        std::optional<std::vector<std::shared_ptr<BaseRule>>> GetSubRules() const override {
            return {};
        }
    private:
        std::vector<StringExpressionTypePair> match_expression;     // field after "match"
        std::vector<MatchCase> match_cases;
        static constexpr int type = 0;
};

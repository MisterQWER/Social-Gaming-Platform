#include "BaseRule.h"

class AssignmentAction : public Action{
    public:
        AssignmentAction(){};
        bool performAction();
};

class Assignment : public BaseRule {
    public:
        Assignment(const std::vector<StringExpressionTypePair>& target, const std::vector<StringExpressionTypePair>& expression);

        
        // For type checking 
        uintptr_t GetType() override {
            return Type();
        } 

        void addEventsTo(GameInstance& game) override;


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
        std::vector<StringExpressionTypePair> target;
        std::vector<StringExpressionTypePair> expression;
        static constexpr int type = 0;
};

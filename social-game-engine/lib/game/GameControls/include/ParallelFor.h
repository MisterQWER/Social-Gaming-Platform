#include "BaseRule.h"
#include "RuleContainer.h"

class ParallelFor : public BaseRule {
    public:
        ParallelFor(const std::string_view& list_element, const std::vector<StringExpressionTypePair>& list, RuleContainer sub_rules);

        void addEventsTo(GameInstance& game) override;

        void printParallelFor();
        
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
            return sub_rules.getSubRules();
        }
    private:
        std::string_view list_element;
        std::vector<StringExpressionTypePair> list;
        RuleContainer sub_rules;
        static constexpr int type = 0;
};

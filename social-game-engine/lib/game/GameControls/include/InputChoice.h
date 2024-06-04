#include "BaseRule.h"

class InputChoice : public BaseRule {
    public:
        InputChoice(const std::string_view& to, const std::vector<StringExpressionTypePair>& prompt, const std::vector<StringExpressionTypePair>& choices, const std::vector<StringExpressionTypePair>& target, int timer);

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
        std::string_view to;    // field after "to"
        std::vector<StringExpressionTypePair> prompt;
        std::vector<StringExpressionTypePair> choice;
        std::vector<StringExpressionTypePair> target;
        int timer;
        static constexpr int type = 0;
};

#include "BaseRule.h"

class Scores : public BaseRule {
    public:
        Scores(const std::vector<StringExpressionTypePair> keys_list);
        
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
        void addEventsTo(GameInstance& game) override;

    private:
        std::vector<StringExpressionTypePair> keys_list;
        static constexpr int type = 0;
};

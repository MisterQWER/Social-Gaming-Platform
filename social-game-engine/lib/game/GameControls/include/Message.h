#include "BaseRule.h"

class Message : public BaseRule {
    public:
        Message(const std::vector<StringExpressionTypePair>& player_set, const std::string_view& message);        
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
        std::vector<StringExpressionTypePair> player_set;
        std::string_view message;
        static constexpr int type = 0;
};

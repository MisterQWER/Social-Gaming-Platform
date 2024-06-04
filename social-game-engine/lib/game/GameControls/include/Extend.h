#include "BaseRule.h"

class Extend : public BaseRule {
    public:
        Extend(const std::vector<StringExpressionTypePair>& name_list, const std::vector<StringExpressionTypePair>& list_expression);
        //remove later. only for testing
        Extend(){
            std::cout << "This constructor is only used for testing purposes" << std::endl;
        };

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
        std::vector<StringExpressionTypePair> name_list;
        std::vector<StringExpressionTypePair> list_expression;
        static constexpr int type = 0;
};

class ExtendAction : public Action{
    public:
        ExtendAction(){};
        bool performAction();
};


struct ExtendVisit{
	template <typename T>
	bool operator()(std::vector<T>& originalList, std::vector<T>& toAddList){
		std::cout << "now extending the list" << std::endl;
        std::cout << "Before length: " << originalList.size() << std::endl;
        originalList.insert(originalList.end(), toAddList.begin(), toAddList.end());
        std::cout << "After length: " << originalList.size() << std::endl;
		return true;
	}
	template <typename A, typename B>
	bool operator()([[maybe_unused]] A a, [[maybe_unused]]B b){
		std::cout << "Invalid arguments passed to Discard" << std::endl;
		return false;
	}
    //not sure if this is needed
	template <typename A>
	bool operator()([[maybe_unused]]A a,[[maybe_unused]] A b){
		std::cout << "Invalid arguments passed to Discard" << std::endl;
		return false;
	}
};
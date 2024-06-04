#pragma once
#include "BaseRule.h"
#include <vector>

class DiscardAction : public Action{
    public:
        DiscardAction(){};
        bool performAction();
};

class Discard : public BaseRule {
    public:
        Discard(const std::vector<StringExpressionTypePair>& int_expression, const std::vector<StringExpressionTypePair>& name_list);
		//TODO: Remove temporary constructor used for testing
		Discard(){
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
        std::vector<StringExpressionTypePair> int_expression;
        std::vector<StringExpressionTypePair> name_list;
        static constexpr int type = 0;
};



struct DiscardVisit{
	template <typename T>
	bool operator()(std::vector<T>& list, int discardNum){
		std::cout << "now performing the discard from a list" << std::endl;
		for(int i=0;i<discardNum;i++){
			if(!list.empty()){
				list.pop_back();
			}
			else{
				std::cout << "Cannot discard from an empty list" << std::endl;
				return false;
			}
		}
		return true;
	}
	template <typename A, typename B>
	bool operator()([[maybe_unused]]A a,[[maybe_unused]] B b){
		std::cout << "Invalid arguments passed to Discard" << std::endl;
		return false;
	}
};

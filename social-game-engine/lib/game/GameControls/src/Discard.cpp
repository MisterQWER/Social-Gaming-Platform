#include "Discard.h"
#include "Value.h"
#include <variant>

bool DiscardAction::performAction(){
	std::cout << "now performing discard" << std::endl;
	//retrieve list from variable resolution interface
	//temporary values for testing:
	using var = std::variant<std::vector<int>, std::vector<double>, int>;
	var list = std::vector<int>{1, 2, 3, 4};
	var notList = 5;
	var num = 2;
	//Actual calling of the execution
	auto result = std::visit(DiscardVisit{}, notList, num);
	return result;
}


Discard::Discard(const std::vector<StringExpressionTypePair>& int_expression, const std::vector<StringExpressionTypePair>& name_list) :
    int_expression(int_expression), name_list(name_list) {};


void Discard::addEventsTo(GameInstance& game) {
	// Create action object and pass it to the gameInstance
	std::unique_ptr<Action> actionPtr = std::make_unique<DiscardAction>();
	game.addGameEvent(std::move(actionPtr));
}

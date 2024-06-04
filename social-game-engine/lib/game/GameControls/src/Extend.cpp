#include "Extend.h"

bool ExtendAction::performAction(){
    std::cout << "Now performing Extend" << std::endl;

    using var = std::variant<std::vector<int>, std::vector<double>, int>;
    var list1 = std::vector<int>{1,2,3,4};
    var list2 = std::vector<int>{1,2};
    var notList = 5;
    auto result = std::visit(ExtendVisit{}, list1, list2);
    std::vector<int> intList = std::get<std::vector<int>>(list1);
    for(int i=0;i<intList.size();i++){
        std::cout << intList[i];
    }
    std::cout << std::endl;
    return result;
}
Extend::Extend(const std::vector<StringExpressionTypePair>& name_list, const std::vector<StringExpressionTypePair>& list_expression) :
    name_list(name_list),
    list_expression(list_expression) {}

void Extend::addEventsTo(GameInstance& game) {
    std::cout << "Create Extend Event" << std::endl;
    std::unique_ptr<Action> actionPtr = std::make_unique<ExtendAction>();
    game.addGameEvent(std::move(actionPtr));
}

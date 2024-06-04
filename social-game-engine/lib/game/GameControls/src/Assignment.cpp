#include "Assignment.h"

bool AssignmentAction::performAction(){
    std::cout << "Now performing Assignment action" << std::endl;

    //resolve expression value

    //place in target location

}

Assignment::Assignment(const std::vector<StringExpressionTypePair>& target, const std::vector<StringExpressionTypePair>& expression) :
    target(target),
    expression(expression) {}

void Assignment::addEventsTo(GameInstance& game) {
    std::cout << "execute assignment" << std::endl;
}

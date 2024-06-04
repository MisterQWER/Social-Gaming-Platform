#include "InputChoice.h"

InputChoice::InputChoice(const std::string_view& to, const std::vector<StringExpressionTypePair>& prompt, const std::vector<StringExpressionTypePair>& choices, const std::vector<StringExpressionTypePair>& target, int timer) :
    to(to), prompt(prompt), choice(choices), target(target), timer(timer) {}

void InputChoice::addEventsTo(GameInstance& game) {
    std::cout << "execute input choice" << std::endl;
}

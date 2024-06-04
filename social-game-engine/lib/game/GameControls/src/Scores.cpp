#include "Scores.h"

Scores::Scores(const std::vector<StringExpressionTypePair> keys_list) :
    keys_list(keys_list) {}

void Scores::addEventsTo(GameInstance& game) {
    std::cout << "execute scores" << std::endl;
}

#include "Message.h"

Message::Message(const std::vector<StringExpressionTypePair>& player_set, const std::string_view& message) :
    player_set(player_set), message(message) {}

void Message::addEventsTo(GameInstance& game) {
    std::cout << "execute message" << std::endl;
}

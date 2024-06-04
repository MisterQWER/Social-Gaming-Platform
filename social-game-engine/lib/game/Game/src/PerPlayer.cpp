#include "PerPlayer.h"


Player::Player(ts::Node *player_node, const std::string &source_code)
        : VariableResolution(player_node, source_code) {
    playerMap = createValueMap();
}

std::map<std::string_view, Value> Player::getPlayerMap() {
    return playerMap;
}
#include "GameMessage.h"

Message::Message(const Connection &playerID, const std::vector<std::string> &cmdList)
    : playerID(playerID), cmdList(cmdList){}

const Connection& Message::getPlayerID() const {
    return playerID;
}

const std::vector<std::string>& Message::getCmdList() const {
    return cmdList;
}


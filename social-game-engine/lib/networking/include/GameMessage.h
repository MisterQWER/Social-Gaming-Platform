#ifndef MESSAGE_H
#define MESSAGE_H

#include "Server.h"

#include <iostream>
#include <vector>
#include <string>


using networking::Connection;

class Message{
private:
    Connection playerID;
    std::vector<std::string> cmdList;
public:
    Message(const Connection &playerID, const std::vector<std::string> &cmdList);
    
    const Connection &getPlayerID() const;
    const std::vector<std::string> &getCmdList() const;
};

#endif
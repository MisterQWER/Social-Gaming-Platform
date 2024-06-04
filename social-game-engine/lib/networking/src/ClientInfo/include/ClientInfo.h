//Header file for client info
#include "Server.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <stack>

using networking::Connection;

class ClientInfo
{
private:
    std::string data;
    time_t currentTime;
    Connection client;
public:
    ClientInfo(const std::string &data, const time_t &time, const Connection &client);
    const std::string getData() const;
    const time_t getTime() const;
    const long unsigned int getConnectionID() const;
    void display() const;
    std::time_t getCurrentTime();
};

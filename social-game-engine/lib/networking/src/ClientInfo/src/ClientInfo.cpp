//Custom object used to keep track of the information sent by the client

#include "ClientInfo.h"

using networking::Connection;


ClientInfo::ClientInfo(const std::string &data, const time_t &time, const Connection &client)
 : data(data), currentTime(time), client(client){ 
 }

const std::string ClientInfo::getData() const {
  return data;
}

const time_t ClientInfo::getTime() const {
  return currentTime;
}

const long unsigned int ClientInfo::getConnectionID() const{
  return client.id;
}

void ClientInfo::display() const{
  std::cout << "Data" << data << "\n";
  std::cout << "Time" << currentTime << "\n";
}

std::time_t ClientInfo::getCurrentTime(){
  auto chronoTime = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(chronoTime);
  return time;
}

#include "timed_event.hpp"
#include <memory>

TimedEvent::TimedEvent(int priorityNumber, int gameId, time_t arrivalTime, std::unique_ptr<Action> action, int gameType)
        : priorityNumber_(priorityNumber),
          gameID_(gameId),
		  action{std::move(action)},
          arrivalTime_(arrivalTime),
          gameType_(gameType) {}

int TimedEvent::GetPriorityNumber() const{
    return priorityNumber_;
}

int TimedEvent::GetGameID() const{
    return gameID_;
}

time_t TimedEvent::GetArrivalTime() const{
    return arrivalTime_;
}

bool TimedEvent::RunEvent() const{
	return action->performAction();
}


int TimedEvent::GetGameType() const{
    return gameType_;
}

TimedEvent::~TimedEvent(){
    
}

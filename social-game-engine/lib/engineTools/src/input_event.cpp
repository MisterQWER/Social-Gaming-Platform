#include "input_event.hpp"

InputEvent::InputEvent(int priorityNumber, int gameId, time_t arrivalTime, Action* action, int gameType)
        : priorityNumber_(priorityNumber),
          gameID_(gameId),
		  action{action},
          arrivalTime_(arrivalTime),
          gameType_(gameType) {}

int InputEvent::GetPriorityNumber() const{
    return priorityNumber_;
}

int InputEvent::GetGameID() const{
    return gameID_;
}

bool InputEvent::RunEvent() const{
	return action->performAction();
}

time_t InputEvent::GetArrivalTime() const{
    return arrivalTime_;
}

int InputEvent::GetGameType() const{
    return gameType_;
}

InputEvent::~InputEvent(){
    
}


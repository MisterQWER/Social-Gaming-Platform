#include "game_event.hpp"

GameEvent::GameEvent(int priorityNumber, int gameId, time_t arrivalTime, std::unique_ptr<Action> action, int gameType)
        : priorityNumber_(priorityNumber),
          gameID_(gameId),
		  action{std::move(action)},
          arrivalTime_(arrivalTime),
          gameType_(gameType) {}

int GameEvent::GetPriorityNumber() const{
    return priorityNumber_;
}

int GameEvent::GetGameID() const{
    return gameID_;
}

bool GameEvent::RunEvent() const{
	return action->performAction();
}

time_t GameEvent::GetArrivalTime() const{
    return arrivalTime_;
}


int GameEvent::GetGameType() const{
    return gameType_;
}

GameEvent::~GameEvent(){
    
}

#pragma once
#include "event.hpp"
#include <memory>

class GameEvent : public Event{
    public:
        GameEvent(int priorityNumber, int gameId, time_t arrivalTime, std::unique_ptr<Action> action, int gameType);

        int GetPriorityNumber() const override;
		int GetGameID() const override;
		bool RunEvent() const override;
		time_t GetArrivalTime() const override;
		int GetGameType() const override;   

        ~GameEvent();
    private:
		int priorityNumber_;
		int gameID_;
		std::unique_ptr<Action> action;
		time_t arrivalTime_;
		int gameType_;
};

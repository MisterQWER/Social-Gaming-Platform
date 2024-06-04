#pragma once
#include "event.hpp"
#include <memory>

class TimedEvent : public Event{
    public:
        TimedEvent(int priorityNumber, int gameId, time_t arrivalTime, std::unique_ptr<Action> action, int gameType);
        int GetPriorityNumber() const override;
		int GetGameID() const override;
		bool RunEvent() const override;
		time_t GetArrivalTime() const override;
		int GetGameType() const override; 

        ~TimedEvent();
    private:
		int priorityNumber_;
		int gameID_;
		std::unique_ptr<Action> action;
		time_t arrivalTime_;
		int gameType_;
};

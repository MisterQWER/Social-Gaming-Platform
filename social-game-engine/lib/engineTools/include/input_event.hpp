#pragma once
#include "event.hpp"

class InputEvent : public Event{
    public:
        InputEvent(int priorityNumber, int gameId, time_t arrivalTime, Action* action, int gameType);
        int GetPriorityNumber() const override;
		int GetGameID() const override;
		bool RunEvent() const override;
		time_t GetArrivalTime() const override;
		int GetGameType() const override; 

        ~InputEvent();
    private:
		int priorityNumber_;
		int gameID_;
		Action* action;
		time_t arrivalTime_;
		int gameType_;
};

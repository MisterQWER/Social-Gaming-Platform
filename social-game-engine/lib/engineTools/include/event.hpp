#pragma once
#include <ctime>
#include <string>
#include "Action.hpp"

class Event{
	public:
		virtual time_t GetArrivalTime() const = 0;
		virtual int GetPriorityNumber() const = 0;
		virtual int GetGameID() const = 0;
		virtual bool RunEvent() const = 0;
		virtual int GetGameType() const = 0; 
		virtual ~Event() = default;
};

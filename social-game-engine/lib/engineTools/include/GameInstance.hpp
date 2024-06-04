#pragma once
#include "event.hpp"
#include "timed_event.hpp"
#include "game_event.hpp"
#include "Action.hpp"
#include <vector> 
#include <memory>
#include <ctime>
#include <string_view>
#include "Variables.h"


class GameInstance{
    private:
        //std::vector<std::unique_ptr<Event>> eventQueue;
        std::map<std::string, Value> variables;
        std::string_view game_path;
    public:
        GameInstance(const std::string_view& file_path) : game_path(file_path) 
        {
            // initiliaze the game_spec given file_path
        }
        //for testing purposes
        GameInstance(std::map<std::string, Value> variables);
        void addTimedEvent(std::unique_ptr<Action> a, time_t delay);
        void addGameEvent(std::unique_ptr<Action> a);
        int getQueueSize(){
            return 1;
        }
};
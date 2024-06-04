#include "GameInstance.hpp"

//TODO Implement the other paramters important to the event
// Priority number, gameId (probably from gameInstance), and gameType

//constructor
GameInstance::GameInstance(std::map<std::string, Value> variables): variables(variables){};

/*
    The unique pointers here are necessary beacuse once we exit the scope, the events created here will be cleaned unless
    emplaced into the vector or a unique pointer is created.
*/
void GameInstance::addTimedEvent(std::unique_ptr<Action> a, time_t delay){
    //Takes a generic action and creates an TimedEvent based on it
    time_t currentTime = std::time(nullptr);
    time_t finalTime = currentTime + delay;
    std::unique_ptr<Event> eventPtr = std::make_unique<TimedEvent>(0, 1, finalTime, std::move(a), 2);
    //eventQueue.push_back(std::move(eventPtr));
}

void GameInstance::addGameEvent(std::unique_ptr<Action> a){
    //Takes a generic action and creates a GameEvent event based on it. 
    time_t currentTime = std::time(nullptr);
    std::unique_ptr<Event> eventPtr = std::make_unique<GameEvent>(0, 1, currentTime, std::move(a), 2);
    //eventQueue.push_back(std::move(eventPtr));
}
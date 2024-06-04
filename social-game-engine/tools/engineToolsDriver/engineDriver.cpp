#include "game_event.hpp"
#include "event.hpp"
#include "input_event.hpp"
#include "timed_event.hpp"
#include "Discard.h"
#include "Extend.h"
#include <iostream>
#include <queue>
#include <memory>
#include <map>
void displayEvent(const std::shared_ptr<Event>& e) {
    std::cout << "Priority number: \t" << e->GetPriorityNumber() << "\n";
    std::cout << "GameID: \t\t" << e->GetGameID() << "\n";
    std::cout << "Arrival Time:\t\t" << e->GetArrivalTime() << "\n";
    std::cout << "Game type: \t\t" << e->GetGameType() << "\n";
	e->RunEvent();
}

void processInputItems(std::queue<std::shared_ptr<Event>> &inputQueue, std::map<int, 
    std::queue<std::shared_ptr<Event>> > &gameQueueMap, int n) {
    /*
     * Things needed to be done here:
     * Process input (change it into code for rules)
     * Enqueue events into game queues
     */
    for (int i = 0; i < n; i++) {
        if (inputQueue.empty()) {
            break;
        }

        std::shared_ptr<Event> event = inputQueue.front();
        inputQueue.pop();
        int key = event->GetGameID();
        if (gameQueueMap.count(key)) {
            std::queue<std::shared_ptr<Event>>& gameQueue = gameQueueMap.at(key);
            gameQueue.push(event);
        } else {
            std::queue<std::shared_ptr<Event>> gameQueue;
            gameQueue.push(event);
            gameQueueMap.insert(std::pair<int, std::queue<std::shared_ptr<Event>> >(key, gameQueue));
        }
    }
}


void AddItemsIntoGameQueue(std::map<int, std::queue<std::shared_ptr<Event>> > &gameQueueMap, 
    std::queue<std::shared_ptr<Event>> &gameServerQueue, int n) {
    //Add n items from the gameServerQueues to the gameQueue
    std::map<int, std::queue<std::shared_ptr<Event>> >::iterator it = gameQueueMap.begin();
    while(it != gameQueueMap.end()) {
        // Accessing the value
        std::queue<std::shared_ptr<Event>>& gameQueue = it->second;

        for (int i = 0; i < n; i++) {
            if (!gameQueue.empty()) {
                std::shared_ptr<Event> event = gameQueue.front();
                gameQueue.pop();
                gameServerQueue.push(event);
            } else {
                break;
            }
        }
        it++;
    }
}

void testEvent(){
	std::unique_ptr<Action> a = std::make_unique<DiscardAction>();
    std::shared_ptr<Event> eventPtr = std::make_shared<GameEvent>(1, 2, 3, std::move(a), 4);
    //Variables used during the loop
    bool running = true;
    std::queue<std::shared_ptr<Event>> inputQueue;
    //process stuff in input queue and enqueue other events
    std::queue<std::shared_ptr<Event>> mainGameQueue;
    std::queue<std::shared_ptr<Event>> timedQueue;
    //define some structure to hold all eventQueues for each server
    std::map<int, std::queue<std::shared_ptr<Event>> > gameQueueMap;
    displayEvent(eventPtr);
    int c = 0;
    
    //Main loop for executing events
    while (running) {
        
        inputQueue.push(eventPtr);
        processInputItems(inputQueue, gameQueueMap, 1);
        AddItemsIntoGameQueue(gameQueueMap, mainGameQueue, 1);

        if (c > 10) {
            running = false;
        }

        c++;
    }

}

void baseEnvironmentExample(){
	// BaseEnvironment example
	GlobalVariables g = GlobalVariables();
	std::map<std::string, std::string> temp;
	temp["ha"] = "hoo";
	temp["test"] = "hard";
	BaseEnvironment base = BaseEnvironment(temp, g);
	std::cout << base.getVariable("hax").value_or("not here!") << "\n";
	base.addVariable("hax", "no hax!");
	std::cout << base.getVariable("hax").value_or("not here!") << "\n";
}

void testRule(){
    //Simulating the iterator invoking the addNewEvents() function
    Extend rule = Extend();
    std::map<std::string, Value> vars;
    GameInstance i = GameInstance(vars);
    std::cout << "Number of items in gameQueue: " << i.getQueueSize() << std::endl;
    rule.addEventsTo(i);
    rule.addEventsTo(i);
    rule.addEventsTo(i);
    std::cout << "Number of items in gameQueue: " << i.getQueueSize() << std::endl;
}
void testAction(){
    ExtendAction action = ExtendAction();
    action.performAction();
}

int main() {
    //testEvent();
    //baseEnvironmentExample();
    testRule();
    testAction();
    return 1;
}


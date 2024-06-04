#include "BaseEnvironment.hpp"
#include "game_event.hpp"
#include <map>
#include <string>

BaseEnvironment::BaseEnvironment(std::map<std::string, std::string> map, GlobalVariables g) : 
	programCounter(0), 
	variables(map),
	global(g)
	{}

void BaseEnvironment::getNextEvent(){
	//can use Event as an interface for now
	//Placeholder gam event object
}
void BaseEnvironment::incrementProgramCounter(){
	programCounter++;
}

std::optional<std::string> BaseEnvironment::getVariable(const std::string& key){
	auto it = variables.find(key);
	if(it != variables.end()){
		return it->second;
	}
	return std::nullopt;
}

GlobalVariables BaseEnvironment::getGlobalVariables(){
	return global;
}
void BaseEnvironment::addVariable(const std::string &name, const std::string& value){
	variables[name] = value;
}

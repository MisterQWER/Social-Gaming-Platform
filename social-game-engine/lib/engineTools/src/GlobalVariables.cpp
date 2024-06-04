#include <string>
#include "GlobalVariables.hpp"
#include <map>
#include <optional>


GlobalVariables::GlobalVariables(){
	//wait for parser to come in with format first
}
std::optional<std::string> GlobalVariables::getConstant(std::string key){
	auto it = constants.find(key);
	if(it != constants.end()){
		return it->second;
	}
	return std::nullopt;
}
std::optional<std::string> GlobalVariables::getPerPlayer(std::string key){
	auto it = constants.find(key);
	if(it != constants.end()){
		return it->second;
	}
	return std::nullopt;
}
std::optional<std::string> GlobalVariables::getPerAudience(std::string key){
	auto it = constants.find(key);
	if(it != constants.end()){
		return it->second;
	}
	return std::nullopt;
}
void GlobalVariables::addConstant(std::string key, std::string toAdd){
	constants[key] = toAdd;

}

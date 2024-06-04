#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <string>
#include <map>
#include <optional>

//A mutable class that can be passed around and referenced in more specific scopes
class GlobalVariables{
	private:
		std::map<std::string, std::string> constants;	
		std::map<std::string, std::string> perPlayer;
		std::map<std::string, std::string> perAudience;
	public:
		GlobalVariables();
		std::optional<std::string> getConstant(std::string key);
		std::optional<std::string> getPerPlayer(std::string key);
		std::optional<std::string> getPerAudience(std::string key);
		void addConstant(std::string key, std::string toAdd);
};


#endif
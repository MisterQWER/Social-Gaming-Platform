#include <map>
#include <string>
#include "GlobalVariables.hpp"
#include <optional>

class BaseEnvironment{
	private:
		int programCounter;
		std::map<std::string, std::string> variables;
		GlobalVariables global;
	public:
		BaseEnvironment(std::map<std::string, std::string> map, GlobalVariables g);
		//TODO: replace void with generic instruction type
		void getNextEvent();
		void incrementProgramCounter();
		std::optional<std::string> getVariable(const std::string& key);
		GlobalVariables getGlobalVariables();
		void addVariable(const std::string& name, const std::string& value);
};

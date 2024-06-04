#ifndef ENGINE_ACTION_H
#define ENGINE_ACTION_H
#include <iostream>
#include "Variables.h"

class Action{
	public:
		virtual bool performAction(){
			std::cout << "Performing action on a default action. This should never happen" << std::endl;
			return false;
		}
	protected:
		std::map<std::string, Value>* vars;
};


#endif


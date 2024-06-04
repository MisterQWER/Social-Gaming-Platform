#pragma once
#include <tuple>
#include <map>
#include "Value.h"
#include "VariableResolution.h"

class Variables : public VariableResolution {
public:
    Variables(ts::Node *variables_node, const std::string &source_code);
    std::map<std::string_view, Value> getVariablesMap();
    void changeVariable(std::string_view key, Value newVal);

private:
    std::map<std::string_view, Value> variablesMap;
};


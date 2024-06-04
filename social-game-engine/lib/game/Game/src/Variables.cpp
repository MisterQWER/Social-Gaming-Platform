#include "Variables.h"

Variables::Variables(ts::Node *constants_node, const std::string &source_code)
        : VariableResolution(constants_node, source_code) {
    variablesMap = createValueMap();
}

std::map<std::string_view, Value> Variables::getVariablesMap() {
    return variablesMap;
}
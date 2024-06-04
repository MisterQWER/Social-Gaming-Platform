#include "Constants.h"

Constants::Constants(ts::Node *constants_node, const std::string &source_code)
        : VariableResolution(constants_node, source_code) {
    constantsMap = createValueMap();
}

std::map<std::string_view, Value> Constants::getConstantsMap() {
    return constantsMap;
}


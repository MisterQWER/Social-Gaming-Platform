#include <tuple>
#include "VariableResolution.h"


class Constants : public VariableResolution {
public:
    Constants(ts::Node * constants_node, const std::string &source_code);
    std::map<std::string_view, Value> getConstantsMap();

private:
    std::map<std::string_view, Value> constantsMap;
};
#pragma once

#include <tuple>
#include <map>
#include <variant>
#include "Common.h"
#include "Value.h"

class VariableResolution {
public:
    VariableResolution(ts::Node * main_node, const std::string &source_code);

    Value resolveMainNode();
    Value resolveNode(ts::Node node);


    Value resolveInt(ts::Node node);
    Value resolveString(ts::Node node);
    Value resolveBoolean(ts::Node node);
    void resolveExpression(ts::Node map_node);
    void resolveExpressionList(ts::Node node);
    Value resolveListLiteral(ts::Node node);
    Value resolveMap(ts::Node node);

    std::map<std::string_view, Value> createValueMap();

private:
    std::map<std::string_view, Value> valueMap;
    ts::Node * main_node;
    const std::string &source_code;
};


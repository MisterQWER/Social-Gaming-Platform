#include "VariableResolution.h"

VariableResolution::VariableResolution(ts::Node *main_node, const std::string &source_code)
        : main_node(main_node),
          source_code(source_code) {}

Value VariableResolution::resolveMainNode() {
    const ts::Node map = main_node->getChildByFieldName("value_map");
    return resolveMap(map);
}

Value VariableResolution::resolveNode(ts::Node node) {
    std::string_view expressionType = node.getType();
    if (expressionType == "expression_list") { resolveExpressionList(node); }
    if (expressionType == "expression") { resolveExpression(node); }
    if (expressionType == "list_literals") { return resolveListLiteral(node); }
    if (expressionType == "value_map") { return resolveMap(node); }
    if (expressionType == "quoted_string") { return resolveString(node); }
    if (expressionType == "number") { return resolveInt(node); }
    if (expressionType == "boolean") { return resolveBoolean(node); }

    throw std::runtime_error(std::string("Unknown rule reached: ") + std::string(expressionType));
}

Value VariableResolution::resolveBoolean(ts::Node node) {
    Value boolValue;
    boolValue.variantValue = extract_boolean(&node, source_code);
    return boolValue;
}

Value VariableResolution::resolveInt(ts::Node node) {
    Value intValue;
    intValue.variantValue = extract_int(&node, source_code);
    return intValue;
}

Value VariableResolution::resolveMap(ts::Node node) {
    Value map;
    std::map<std::string_view, Value> valMap;

    for (uint32_t child = 0; child < node.getNumChildren(); child++) {
        const ts::Node childExpr = node.getChild(child);
        if (childExpr.getType() == "map_entry") {
            const ts::Node identifier = childExpr.getChildByFieldName("identifier");
            std::string key = extract_string(&identifier, source_code);
            Value exprVal = resolveNode(childExpr.getChildByFieldName("expression"));
            valMap.insert(std::make_pair(key, exprVal));
        }
    }
    map.variantValue = valMap;
    if(node.getParent().getID() == main_node->getID()) {
        map.valueMap = valMap;
        valueMap = valMap;
    }
    return map;
}

Value VariableResolution::resolveString(ts::Node node) {
    Value stringValue;
    stringValue.variantValue = extract_string(&node, source_code);
    return stringValue;
}

void VariableResolution::resolveExpression(ts::Node node) {
    for (uint32_t child = 0; child < node.getNumChildren(); child++) {
        resolveNode(node.getChild(child));
    }
}

Value VariableResolution::resolveListLiteral(ts::Node node) {
    Value listValue;
    vector<Value> listOfExpressions;
    for (uint32_t child = 0; child < node.getNumChildren(); child++) {
        Value childExpr = resolveNode(node.getChild(child));
        listOfExpressions.push_back(childExpr);
    }
    listValue.variantValue = listOfExpressions;
    return listValue;
}

void VariableResolution::resolveExpressionList(ts::Node node) {
    for (uint32_t child = 0; child < node.getNumChildren(); child++) {
        resolveNode(node.getChild(child));
    }
}

std::map<std::string_view, Value> VariableResolution::createValueMap() {
    std::map<std::string_view, Value> map;
    Value mapValue = resolveMainNode();
    map = valueMap;
    return map;
}
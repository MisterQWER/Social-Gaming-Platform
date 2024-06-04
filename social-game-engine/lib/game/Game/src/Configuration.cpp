#include "Configuration.h"
#include "nlohmann/json.hpp"


void printConfigPreorder(std::string sourcecode, ts::Node node, std::string indent) {

  for (uint32_t i = 0; i < node.getNumChildren(); i++) {
    ts::Node child = node.getChild(i);
    std::cout << indent << child.getType() << std::endl;
  
    printConfigPreorder(sourcecode ,child, indent + "  ");
  }
}

template <typename T>
RuleSetup<T>::RuleSetup(ts::Node * setup_node, const std::string& source_code) {

}

Configuration::Configuration(ts::Node * config_node, const std::string& source_code) {
    /*
        Extract:
        - name
        - player range
        - audience
        - rules

        structure infererred from:
        https://github.com/nsumner/tree-sitter-socialgaming/blob/main/src/grammar.json
    */ 

    // printConfigPreorder(source_code ,*config_node, "");
    const ts::Node name_node = config_node->getChildByFieldName("name");
    const ts::Node range_node = config_node->getChildByFieldName("player_range");
    const ts::Node audience_node = config_node->getChildByFieldName("has_audience");

    if (name_node.isNull() == false) 
        name = extract_string(&name_node, source_code);
    if (range_node.isNull() == false) {
        std::vector<int> range = NumberRange::getRange(&range_node, source_code);
        if (range.size() == 2)
            player_range = std::make_pair(range[0], range[1]);
    }
    if (audience_node.isNull() == false) {
        has_audience = extract_boolean(&audience_node, source_code);
    }

    ts::Node* setup_rule_temp = nullptr;
    for (uint32_t i = 0; i < config_node->getNumChildren(); i++) {
        ts::Node child = config_node->getChild(i);
        if (child.getType() == "setup_rule") {
            setup_rule_temp = &child;
            break;
        }
  }
    ts::Node setup_rule = *setup_rule_temp;
    ts::Node rule_name = setup_rule.getNamedChild(0);
    
    // std::cout << extract_string(&setup_rule, source_code) << std::endl;

    const ts::Node kinds = setup_rule.getChildByFieldName("kind");
    std::string tempKind = extract_string(&kinds, source_code);
    // std::cout << tempKind << std::endl;
    if(tempKind == "integer") {
        std::pair<int, int> rangeText;
        std::string identifierName = extract_string(&rule_name, source_code);
        IntegerKind intKind;
        const ts::Node Prompt = setup_rule.getChildByFieldName("prompt");
        std::string promptText = extract_string(&Prompt, source_code);
        intKind.prompt = promptText;
        const ts::Node range = setup_rule.getChildByFieldName("range");
        std::vector<int> rangeForSetup = NumberRange::getRange(&range, source_code);
        if (rangeForSetup.size() == 2)
            rangeText = std::make_pair(rangeForSetup[0], rangeForSetup[1]);
        intKind.minVal = rangeText.first;
        intKind.maxVal = rangeText.second;
        setup.insert(std::make_pair(identifierName, intKind));
    }
    else if(tempKind == "boolean") {
        std::string identifierName = extract_string(&rule_name, source_code);
        BooleanKind boolKind;
        const ts::Node Prompt = setup_rule.getChildByFieldName("prompt");
        std::string promptText = extract_string(&Prompt, source_code);
        boolKind.prompt = promptText;
        setup.insert(std::make_pair(identifierName, boolKind));
    }
    else if(tempKind == "enum") {
        // TODO:  create a structure for enum type
        std::string identifierName = extract_string(&rule_name, source_code);
        EnumKind enumKind;
        const ts::Node prompt = setup_rule.getChildByFieldName("prompt");
        std::string promptText = extract_string(&prompt, source_code);
        enumKind.prompt = promptText;
    }
  }

void Configuration::printDebugInfo() {
    std::cout << "Name = " << name << '\n'
    << "Player Range = (" << player_range.first << "," << player_range.second << ")\n"
    << "Has Audience = " << (has_audience ? "true" : "false") << '\n';  
    
     for (const auto& pair : setup) {
        std::cout << pair.first << " : " << '\n';
        if (holds_alternative<IntegerKind>(pair.second)) { 
            IntegerKind intKind = get<IntegerKind>(pair.second); 
            std::cout << "\tPrompt = " << intKind.prompt << std::endl;
            std::cout << "\tmin =  " << intKind.minVal << std::endl;
            std::cout << "\tmax =  " << intKind.maxVal << std::endl;
        } 
        if (holds_alternative<BooleanKind>(pair.second)) { 
                BooleanKind booleanKind = get<BooleanKind>(pair.second); 
                std::cout << "\tPrompt = " << booleanKind.prompt << std::endl;
            } 
    }
}

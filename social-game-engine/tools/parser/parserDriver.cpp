#include <cstdio>
#include <unordered_map>
#include "Rules.h"
#include <variant>
#include <functional>

#include "GameSpecification.h"
#include "Value.h"
#include "BaseRule.h"
#include "RuleContainer.h"

using namespace std; using namespace ts;
using std::cout; 

int main() {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    // Open file containing RPS configurations
    std::ifstream ifs("games/rock-paper-scissors.game");
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string sourcecode = buffer.str();
    ifs.close();

    // Parse the provided string into a syntax tree.
    ts::Tree tree = parser.parseString(sourcecode);

    // Get the root and config node of the syntax tree. 
    ts::Node root = tree.getRootNode();
    ts::Node config_node = root.getChildByFieldName("configuration");

    // hardcode config map for test
    ts::Node first_for_root = root.getChildByFieldName("rules").getChild(1).getChild(1).getChild(0);
    ts::Node second_for_root = first_for_root.getChild(4).getChild(4).getChild(0);

    // Test section
    cout <<"\n\nRule # of direct children: " <<second_for_root.getNumChildren() << endl;
    cout <<"type: " << second_for_root.getType() << endl << endl;
    cout <<"name : " << second_for_root.getType() << endl;

    cout <<"=========== PRINTING SubTree :\n"<< NodeOutputDecorator::getNodeString(&second_for_root, sourcecode) <<"\n================="<< endl;
    NodeOutputDecorator::printChildren(&second_for_root);
    
    cout << "**********************\n****************\n For Rule Object Usage\n";

    // std::string_view identifier_view = "weapon";
    // vector<pair<string_view, ExpressionType>> demo_expression_list = {
    //     {"weapons", ExpressionType::identifier}
    // }; 
    // RuleContainer empty_subrule;

    std::string_view identifier_view = "round";
    
    vector<pair<string_view, ExpressionType>> demo_expression_list = {
        {"weapons", ExpressionType::identifier}
    }; 

    vector<pair<string_view, ExpressionType>> demo_expression_list2 = {
    {"configuration", ExpressionType::identifier},
    {"rounds", ExpressionType::identifier},
    {"upfrom", ExpressionType::builtin},
    {"(", ExpressionType::beginArg},
    {"9", ExpressionType::number},
    {")", ExpressionType::endArg}
    };

    RuleContainer demoContainer;

    // Data Table test section
    cout << "**********************\n****************\n Table Usage \n******** \n";

    GameSpecification table(&root, sourcecode);

    // Custom nested map<string_view, Value> to test nested insert
    map<string_view, Value> inner {
        {"int", Value{10}} 
    };
    map<string_view,Value> outer {
        {"inner", Value{inner}}
    };

    vector<pair<string_view, ExpressionType>> nested_demo_expression1 = {
        {"outer", ExpressionType::identifier}
    };

    vector<pair<string_view, ExpressionType>> nested_demo_expression2 = {
        {"outer", ExpressionType::identifier},
        {"inner", ExpressionType::identifier},
        {"string", ExpressionType::identifier}
    };

    // Inserting to table
    table.insert_entry_by_keychain(Value{outer}, nested_demo_expression1);
    table.insert_entry_by_keychain(Value{string_view("SomeThing")}, nested_demo_expression2);

    // Modifying test
    table.modify_entry_by_keychain(Value{string_view("MODIFIED")}, nested_demo_expression2);

    table.print();

    // Retrieving entries from the Table
    std::optional<Value> val = table.get_entry_by_keychain(demo_expression_list);
    std::optional<Value> val2 = table.get_entry_by_keychain(demo_expression_list2);

    // Example of Processing a value after retrieving from the table:
    if (val.has_value()) 
        val.value().print();
    else 
        cout << "[TABLE_ACCESS_ERROR] No value found for the current keychain" << endl;

    return 0;
}
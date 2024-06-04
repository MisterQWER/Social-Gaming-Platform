#include "GameExecutor.h"
#include <memory>
#include <string>
#include <utility>
#include <algorithm>


using Table = std::map<std::string, int>;
using Execution::ExecutionPolicy;
using Execution::Output;
using Execution::ExecutorPointer;
using Execution::RuntimeExecutioner;
using Execution::ForLoopPolicy;
using Execution::PolicyType;

// Example implementations on Executors (Also serves as a Sanity check)
class SetTable : public ExecutionPolicy {
private:
    std::string var;
    int val;
public:
    SetTable(std::string _var, int _val) : var(_var), val(_val) {}
    void Execute(GameInstance * game, RuntimeExecutioner * executioner) override {
        // table[var] = val;
    }
    PolicyType GetPolicyType() const override { return PolicyType::Default_Policy; }
};

class OutputTable : public ExecutionPolicy {
    std::string var;
public:
    OutputTable(std::string _var) : var(_var) {}
    void Execute(GameInstance * game, RuntimeExecutioner * executioner) override {
        // printf("var = %d\n", table[var]);
    }
    PolicyType GetPolicyType() const override { return PolicyType::Default_Policy; }
};

int main() {

    std::unique_ptr<GameInstance> game = make_unique<GameInstance>("");

    std::unique_ptr<Table> demo_map = make_unique<Table>();
    demo_map->insert({"A", 20});
    demo_map->insert({"B", 40});


    // Loop A
    // Creating some executors

    std::unique_ptr<SetTable> set_var_a = make_unique<SetTable>("A", 45);
    std::unique_ptr<OutputTable> print_a = make_unique<OutputTable>("A");
    std::unique_ptr<SetTable> set_var_a_1 = make_unique<SetTable>("A", 35);    
    std::unique_ptr<OutputTable> print_a_1 = make_unique<OutputTable>("A");
    

    // Creating a body of statements for our list loop (IN backwards order)
    std::vector<ExecutorPointer> list_statements;
    list_statements.push_back(print_a.get());
    list_statements.push_back(set_var_a.get());
    list_statements.push_back(print_a_1.get());
    list_statements.push_back(set_var_a_1.get());
    // Creating executor
    std::unique_ptr<RuntimeExecutioner> executor = make_unique<RuntimeExecutioner>();

//    executor->InsertStatement(parallel_execution.get());
    // executor->RunExecutioner(*demo_map);

    // std::string_view identifier_view = "round";

    // vector<pair<string_view, ExpressionType>> demo_expression_list = {
    // {"configuration", ExpressionType::identifier},
    // {"rounds", ExpressionType::identifier},
    // {"upfrom", ExpressionType::builtin},
    // {"(", ExpressionType::beginArg},
    // {"9", ExpressionType::number},
    // {")", ExpressionType::endArg}
    // };
    // RuleContainer empty_subrule;

    std::string_view identifier_view = "weapon";
    vector<pair<string_view, ExpressionType>> demo_expression_list = {
        {"weapons", ExpressionType::identifier}
    }; 
    RuleContainer empty_subrule;

    For for_rule(identifier_view, demo_expression_list, empty_subrule);
    for_rule.printForObject();

    executor->RunExecutioner(game.get());
}
#include "GameExecutor.h"

namespace Execution {

    // ---- EXECUTION POLICIES ---- //

    /*
        For Loop Policy
    */
    void ForLoopPolicy::Execute(GameInstance * game, RuntimeExecutioner* executioner){
        std::cout << "iterating\n";
        if(!for_rule.isFinished())  {
            for_rule.step();
            // Inserting for loop to check again
            executioner->InsertStatement(this);
            // Inserting body
            executioner->InsertStatements(body);
        }  
    }

    /*
        Parallel For and In Parallel
    */
    MultiExecutionerEnvironment::MultiExecutionerEnvironment(const size_t& num_executioners) {
        executioners.reserve(num_executioners);
    }

    // Insert the same body of statements on all executioners (For For Parallel)
    void MultiExecutionerEnvironment::InsertOnAllExecutioners(std::vector<ExecutionPolicy * > statement_list) {
        int num_executioners = executioners.size();

        for(int i = 0; i < num_executioners; i++) {
            executioners.push_back(
                RuntimeExecutioner {statement_list}
            );
        }
    }

    // Insert and assign a statement to each executioner (For InParallel)
    void MultiExecutionerEnvironment::InsertEachPerExecutioner(std::vector<ExecutionPolicy * > statement_list) {

        assert(statement_list.size() == executioners.size());

        std::transform(statement_list.begin(), statement_list.end(), executioners.begin(), [](ExecutionPolicy * policy) {
            RuntimeExecutioner executioner {
                std::vector<ExecutionPolicy *> {policy}
            };
            return executioner;
        });
    }

    void MultiExecutionerEnvironment::Reset() {
        executioners.clear();
    }

    void MultiExecutionerEnvironment::Resize(const size_t& new_size) {
        executioners.reserve(new_size);
    }

    std::vector<RuntimeExecutioner>::iterator MultiExecutionerEnvironment::begin() {
        return executioners.begin();
    }

    std::vector<RuntimeExecutioner>::iterator MultiExecutionerEnvironment::end() {
        return executioners.end();
    }

    bool MultiExecutionerEnvironment::AreExecutionersComplete() {
        return std::any_of(executioners.begin(), executioners.end(), [](const RuntimeExecutioner& parallel_executioner) {
            return parallel_executioner.IsFinished() == false;
        });
    }

    void InParallelPolicy::Execute(GameInstance * game, RuntimeExecutioner* executioner) {

        std::for_each(executioner_environment.begin(), executioner_environment.end(), [game](RuntimeExecutioner& parallel_statement) {
            ExecutorPointer statement = parallel_statement.GetNextStatement(); // InputOutput()

            // TODO: Do we check I/O here as well?
            if(statement != nullptr) {
                statement->Execute(game, &parallel_statement);
            }
        });

        // If any of the children processes are not finished
        if(executioner_environment.AreExecutionersComplete() == false) {
            executioner->InsertStatement(this);
        }
    }

    void ParallelForPolicy::Execute(GameInstance * game, RuntimeExecutioner* executioner) {
        // Step through each child statment once

        /*
            For every Execute() / 1 unit of work we:
            - Access each element once!
            - Execute one unite of work in the body once per element!
        */

        // - copy players = getValue(key (some sort of string));
        // parallelFor.RefreshElements();

        // ParallelFor body of statements
        std::for_each(executioner_environment.begin(), executioner_environment.end(), [game](RuntimeExecutioner& parallel_statement) {
            ExecutorPointer statement = parallel_statement.GetNextStatement(); // InputOutput()

            // - player = players.begin()
            // parallelFor.NextElement();

            // TODO: handle execution in the Parallel statement or main RuntimeExecutioner?
            if(statement != nullptr) {
                statement->Execute(game, &parallel_statement);
            }

            // - players.erase(at first index)
            // parallelFor.Next();
        });

        // If any of the children processes are not finished
        if(executioner_environment.AreExecutionersComplete() == false) {
            executioner->InsertStatement(this);
        }
    }


// ---- RUNTIME EXECUTIONER ---- //
    void RuntimeExecutioner::InsertStatements(std::vector<ExecutorPointer> new_executors) {
        executors.insert(executors.end(), new_executors.begin(), new_executors.end());
    }   


    void RuntimeExecutioner::InsertStatement(ExecutorPointer new_executor) {
        InsertStatements(std::vector<ExecutorPointer> {new_executor});
    }


    RuntimeExecutioner::RuntimeExecutioner()
     : executors(std::vector<ExecutorPointer> {}) {}
    

    RuntimeExecutioner::RuntimeExecutioner(std::vector<ExecutorPointer> statements)
     : executors(statements) {
    }

    void RuntimeExecutioner::RunExecutioner(GameInstance * game) {
        while(!executors.empty()) {
            ExecutorPointer statement = executors[executors.size() - 1];
            executors.pop_back();
            statement->Execute(game, this);
        }
    }


    bool RuntimeExecutioner::IsFinished() const {
        return executors.empty();
    }


    ExecutorPointer RuntimeExecutioner::GetNextStatement() {
        if(!IsFinished()) {
            ExecutorPointer statement = executors.back();
            executors.pop_back();
            return statement;
        }
        return nullptr;
    }


    // ExecutionerConstructor

    ExecutionerConstructor::ExecutionerConstructor(Rules * rules) {
        std::vector<unique_ptr<BaseRule>>::const_iterator rules_begin = rules->getGameRules().begin();
        std::vector<unique_ptr<BaseRule>>::const_iterator rules_end = rules->getGameRules().end();
        std::for_each(rules_begin, rules_end, [this](const auto& rule) {
            rule->accept(*this);
        });
    }

    void ExecutionerConstructor::DefaultRuleConstruction(std::unique_ptr<BaseRule> rule) {
        std::unique_ptr<ExecutionPolicy> execution_policy = make_unique<DefaultPolicy>(rule);
        executioner_storage.push_back(std::move(execution_policy));
        recent_rule = executioner_storage.back().get();
    }

    std::vector<ExecutionPolicy *> ExecutionerConstructor::ToExecutionPolicyVector(std::vector<std::shared_ptr<BaseRule>> sub_rules) {
            std::vector<ExecutionPolicy *> execution_policy_body;
            std::for_each(sub_rules.begin(), sub_rules.end(), [this, &execution_policy_body](auto& sub_rule) {
                sub_rule->accept(*this);
                execution_policy_body.push_back(this->recent_rule);
            });
            return execution_policy_body;
    }

    void ExecutionerConstructor::visit(Assignment assignment_rule) {
        DefaultRuleConstruction(std::make_unique<Assignment>(assignment_rule));
    }
    void ExecutionerConstructor::visit(Discard discard_rule) {
        DefaultRuleConstruction(std::make_unique<Discard>(discard_rule));
    }
    void ExecutionerConstructor::visit(Extend extend_rule) {
        DefaultRuleConstruction(std::make_unique<Extend>(extend_rule));
    }
    void ExecutionerConstructor::visit(For for_rule) {
        std::optional<std::vector<std::shared_ptr<BaseRule>>> sub_rules_opt = for_rule.GetSubRules();
        if(sub_rules_opt.has_value()) {
            std::vector<std::shared_ptr<BaseRule>> sub_rules = sub_rules_opt.value();
            std::unique_ptr<ForLoopPolicy> for_loop_policy = make_unique<ForLoopPolicy>(for_rule, ToExecutionPolicyVector(sub_rules));
            executioner_storage.push_back(std::move(for_loop_policy));
            recent_rule = executioner_storage.back().get();
        }
    }
    void ExecutionerConstructor::visit(InputChoice input_choice_rule) {
        DefaultRuleConstruction(std::make_unique<InputChoice>(input_choice_rule));
    }
    void ExecutionerConstructor::visit(Match match_rule) {
        DefaultRuleConstruction(std::make_unique<Match>(match_rule));
    }
    void ExecutionerConstructor::visit(Message message_rule) {
        DefaultRuleConstruction(std::make_unique<Message>(message_rule));
    }
    void ExecutionerConstructor::visit(ParallelFor parallel_for_rule) {
        std::optional<std::vector<std::shared_ptr<BaseRule>>> sub_rules_opt = parallel_for_rule.GetSubRules();
        if(sub_rules_opt.has_value()) {
            std::vector<std::shared_ptr<BaseRule>> sub_rules = sub_rules_opt.value();
            std::unique_ptr<ParallelForPolicy> parallel_loop_policy = make_unique<ParallelForPolicy>(ToExecutionPolicyVector(sub_rules), parallel_for_rule);
            executioner_storage.push_back(std::move(parallel_loop_policy));
            recent_rule = executioner_storage.back().get();  
        }
    }
    void ExecutionerConstructor::visit(Scores scores_rule) {
        DefaultRuleConstruction(std::make_unique<Scores>(scores_rule));
    }
   
}

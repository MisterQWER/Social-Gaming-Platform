// Use an iterator to incrementally add execution statements during runtime
// Should apply to rules and such
// TODO: Invest in making a custom iterator

#ifndef GAME_EXECUTOR
#define GAME_EXECUTOR

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <map>
#include <span>
#include <memory>

// Need to include all the Rules here
#include "Rules.h"

// TODO: Uncomment when GameInstance is ready
// #include "GameInstance.hpp"

/*
    TODO
    - Implement ParallelFor, InParallel, and For
    - Draft/Mock User Input, Timers, Match and Default Executions
    - Implement GetRuntimeType() on BaseRules and deriving subclasses (will need to rely ex6 code)
    - Test RuleToExecutionPolicy code to transform Rules to ExecutionPolicies

    TODO: 

    Make a special ExecutionPolicy for input and output

    // This one is meant to be executed immediately
    class SendPrompt : ExecutionPolicy {
        start_immediately = true;
    };

    // This one waits for a response
    class OnPromptReceived : ExecutionPolicy {
        start_immediately = false;
    };
    
*/

class GameInstance;

namespace Execution {
    

    class RuntimeExecutioner;
    class ExecutionPolicy;
    class ExecutionerConstructor;

    enum PolicyType {
        Default_Policy,
        ForLoop_Policy,
        ParallelFor_Policy,
        InParallel_Policy
    };

    // TODO, need to store the Policies somewhere, if they are all unique_ptr    
    class ExecutionPolicy { // Decorator over a BaseRule
        // TODO: add reference to RuntimeExecutioner here or 
        // as an parameter in Execute?
        public:
            virtual ~ExecutionPolicy() = default;
            virtual void Execute(GameInstance * game, RuntimeExecutioner* executioner) = 0;
            virtual PolicyType GetPolicyType() const = 0;
    };

    // Type Aliases
    using ExecutorPointer = ExecutionPolicy *;

    // Non Control Flow statements
    class DefaultPolicy : public ExecutionPolicy {
    private:

        // Use std::unique_ptr to achieve polymorphism
        std::unique_ptr<BaseRule> rule;
    public:
        DefaultPolicy(std::unique_ptr<BaseRule>& _rule) : rule(std::move(_rule)) {}
        void Execute(GameInstance * game, RuntimeExecutioner* executioner) override {rule->addEventsTo(*game);}
        PolicyType GetPolicyType() const override { return Default_Policy; }
    }; 
    /*
        For Loop Policy
        - Will develop it's own scope as we go down
    */
    class ForLoopPolicy : public ExecutionPolicy {
    private:
        For for_rule;
        std::vector<ExecutorPointer> body;
    public:
        ForLoopPolicy(For _for_rule, std::vector<ExecutorPointer> _body) : for_rule(_for_rule), body(_body) {}
        void Execute(GameInstance * game, RuntimeExecutioner* executioner) override;
        PolicyType GetPolicyType() const override { return ForLoop_Policy; }
    };

    /*
        MultiExecutionerEnvironment

        Used for Parallel For and In Parallel

        Pre Conditions:
        - Must provide an initial size
        - If usage is changed, call Reset(), and Resize() with a new size
        - Can't call InsertOnAllExecutioners and InsertEachPerExecutioners one after the other,
        refer to previous point

        Post Conditions:
        - Management is handled by governing object, with begin(), end() and AreExecutionersComplete()

    */
    class MultiExecutionerEnvironment {
    private:
        std::vector<RuntimeExecutioner> executioners;
    public:
        MultiExecutionerEnvironment() = default;
        MultiExecutionerEnvironment(const size_t& num_executioners);

        // Insert the same body of statements on all executioners (For For Parallel)
        void InsertOnAllExecutioners(std::vector<ExecutorPointer> statement_list);

        // Insert and assign a statement to each executioner (For InParallel)
        void InsertEachPerExecutioner(std::vector<ExecutorPointer> statement_list);

        void Reset();

        void Resize(const size_t& new_size);

        std::vector<RuntimeExecutioner>::iterator begin();

        std::vector<RuntimeExecutioner>::iterator end();

        bool AreExecutionersComplete();
    };
    /*
        InParallelPolicy:

        Invariants:
        - The number of enclosed statements DO NOT change
        
        Pre-Condition
        - Existing, non null parent executioner
        - Existing, non null Game Instance

        Post-Condition
        - Execution is handled within InParallel, that is one unit of execution of enclosed
        statements is handled by the governing InParallel, and parent executioner handles
        when ParallelFor can execute it's enclosed statements again
    */
        
    class InParallelPolicy : public ExecutionPolicy {
            // TODO: May not need pointers to RuntimeExecutioners if they only exist 
            // in the Parallel Class
            MultiExecutionerEnvironment executioner_environment;
        public:
            InParallelPolicy(std::vector<ExecutorPointer> statements) {
                executioner_environment.Resize(statements.size());
                executioner_environment.InsertEachPerExecutioner(statements);
            }
            void Execute(GameInstance * game, RuntimeExecutioner* executioner) override;
            PolicyType GetPolicyType() const override { return InParallel_Policy; }
    };

    /*
        ParallelForPolicy:

        Invariants:
        - List does not change size (TODO: Need to handle when list does change size!)
        
        Pre-Condition
        - Existing, non null parallelFor
        - Existing, non null parent executioner
        - Existing, non null Game Instance

        Post-Condition
        - Execution is handled within ParallelFor, that is one unit of execution of enclosed
        statements is handled by the governing ParallelFor, and parent executioner handles
        when ParallelFor can execute it's enclosed statements again 
    */
    class ParallelForPolicy : public ExecutionPolicy {
        private:
            ParallelFor for_parallel_rule;
            MultiExecutionerEnvironment executioner_environment;
        public:
        
        // TODO: Provide a way to query the list element size for ParallelFor.
        // TODO: We also need to resize the ParallelFor accordingly (it might change!)
            ParallelForPolicy(std::vector<ExecutorPointer> statements, ParallelFor parallelFor) 
            : for_parallel_rule(parallelFor) {

                // Some arbitrary number for now.
                executioner_environment.Resize(10);
                executioner_environment.InsertOnAllExecutioners(statements);
            }
            void Execute(GameInstance * game, RuntimeExecutioner* executioner) override;
            PolicyType GetPolicyType() const override { return ParallelFor_Policy; }
    };



 

  /*
    MainRuntimeExecutioner {

    public:
        checkInputs() {
            for all objects in I/O vector {
                check message queue in server
                    flip bool if not empty
                    // This now allows the onReceivePrompt ExecutionPolicy to Execute()
            }
        }

    private:
        RuntimeExecutioner main_executioner;
        GameServer * server;
        vector<InputOutput> I/O;
    }

    Unsures:
    - Some tag to mark the correct vector for the player?
*/


    
    class RuntimeExecutioner {
        
        private:
            std::vector<ExecutorPointer> executors;
        public:
            void InsertStatements(std::vector<ExecutorPointer> new_ExecutionPolicys);
            void InsertStatement(ExecutorPointer new_ExecutionPolicy);
            
            RuntimeExecutioner();
            RuntimeExecutioner(std::vector<ExecutorPointer> statements);
            
            void RunExecutioner(GameInstance * game);

            bool IsFinished() const;
            ExecutorPointer GetNextStatement();
    };


    
    class Output : public ExecutionPolicy {
        public:
            void Execute(GameInstance * game, RuntimeExecutioner* executioner) override { 
                printf("Hello, I am output!\n"); 
        }
    };
    

    // A Visitor class that constructs and stores ExecutionPolicies
    class ExecutionerConstructor : public RuleVisitor {
    public:

        ExecutionerConstructor(Rules * rules);

        void visit(Assignment assignment_rule) override;
        void visit(Discard discard_rule) override;
        void visit(Extend extend_rule) override;
        void visit(For for_rule) override;
        void visit(InputChoice input_choice_rule) override;
        void visit(Match match_rule) override;
        void visit(Message message_rule) override;
        void visit(ParallelFor parallel_for_rule) override;
        void visit(Scores scores_rule) override;

        std::vector<std::unique_ptr<ExecutionPolicy>>::const_iterator StorageBegin() { return executioner_storage.begin();}
        std::vector<std::unique_ptr<ExecutionPolicy>>::const_iterator StorageEnd() { return executioner_storage.end();}

        // Debugging purposes
        size_t GetStorageSize() {return executioner_storage.size();}
    private:
        ExecutionPolicy * recent_rule;
        void DefaultRuleConstruction(std::unique_ptr<BaseRule> rule);
        std::vector<ExecutionPolicy *> ToExecutionPolicyVector(std::vector<std::shared_ptr<BaseRule>> sub_rules);
        std::vector<std::unique_ptr<ExecutionPolicy>> executioner_storage;
    };


    class MainRuntimeExecutioner {
    private:
        RuntimeExecutioner main_executioner;
        ExecutionerConstructor executioner_constructor;
    public:
        MainRuntimeExecutioner(Rules * rules) : executioner_constructor{rules} {
            main_executioner = RuntimeExecutioner();
            std::vector<ExecutorPointer> statements;
            std::for_each(executioner_constructor.StorageBegin(), executioner_constructor.StorageEnd(), [&statements](const auto& pointer) {
                statements.push_back(pointer.get());
            });
            main_executioner.InsertStatements(statements);
        }


        void Run(GameInstance * instance) {
            main_executioner.RunExecutioner(instance);
        }
    };

   
}

#endif
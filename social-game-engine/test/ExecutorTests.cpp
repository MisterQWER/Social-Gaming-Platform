#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Rules.h"
#include "GameExecutor.h"
#include <memory>

using Execution::RuntimeExecutioner;
using Execution::ExecutionerConstructor;
using Execution::MainRuntimeExecutioner;

class GameSpecification {

    public:
        Value get(std::string key);
        void set(std::string key, Value value_to_set);
};

class ExecutorFixture : public ::testing::Test {

    public:
        void CreateRules(std::string file_name) {
            // Create a language and parser.
            ts::Language language = tree_sitter_socialgaming();
            ts::Parser parser{language};

            // Open file containing RPS configurations
            std::ifstream ifs(file_name);
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            std::string sourcecode = buffer.str();
            ifs.close();

            // Parse the provided string into a syntax tree.
            ts::Tree tree = parser.parseString(sourcecode);

            // Get the root node of the syntax tree. 
            ts::Node root = tree.getRootNode();
            for (uint32_t i = 0; i < root.getNumChildren(); i++) {
                ts::Node child = root.getNamedChild(i);


                if (child.getType() == "rules") {
                    rules = make_unique<Rules>(child, sourcecode);
                    const std::vector<std::unique_ptr<BaseRule>>& rules_list = rules->getGameRules();
                    std::cout << "Rules: " << rules_list.size() << std::endl;
                }
            }
        }
    protected:
        void SetUp() override {}
        void TearDown() override {}

        std::unique_ptr<Rules> rules;
};

TEST_F(ExecutorFixture, TestEmptyExecutionerConstruction) {
    CreateRules("InputTestFiles/empty.game");
    std::unique_ptr<ExecutionerConstructor> executioner_constructor = make_unique<ExecutionerConstructor>(rules.get());
    ASSERT_EQ(executioner_constructor->GetStorageSize(), 0);
};

TEST_F(ExecutorFixture, TestNestedLoopConstruction) {
    CreateRules("InputTestFiles/nested_loop.game");
    std::unique_ptr<ExecutionerConstructor> executioner_constructor = make_unique<ExecutionerConstructor>(rules.get());
    ASSERT_EQ(executioner_constructor->GetStorageSize(), 5);

    size_t num_loops = std::count_if(executioner_constructor->StorageBegin(), executioner_constructor->StorageEnd(), [](const auto& executioner) {
        return executioner->GetPolicyType() == Execution::PolicyType::ForLoop_Policy;
    });

    ASSERT_EQ(num_loops, 3);

    size_t num_default_policies = std::count_if(executioner_constructor->StorageBegin(), executioner_constructor->StorageEnd(), [](const auto& executioner) {
        return executioner->GetPolicyType() == Execution::PolicyType::Default_Policy;
    });

    ASSERT_EQ(num_default_policies, 2);
}

TEST_F(ExecutorFixture, TestExecutionFor) {
    CreateRules("InputTestFiles/loop_test.game");
    std::cout << "Normal Loop\n";
    MainRuntimeExecutioner main_runner {rules.get()};
    GameInstance instance("");
    main_runner.Run(&instance);
}

TEST_F(ExecutorFixture, TestExecutionForNested) {
    // CreateRules("InputTestFiles/nested_loop.game");
    // std::cout << "Nested Loop\n";
    // MainRuntimeExecutioner main_runner {rules.get()};
    // GameInstance instance;
    // main_runner.Run(&instance);
}
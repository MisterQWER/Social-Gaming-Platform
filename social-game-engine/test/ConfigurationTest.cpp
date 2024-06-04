#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Configuration.h"

using ::testing::Return;

class ConfigurationTest : public ::testing::Test {
protected:
    void SetUp() override{

    }
    void TearDown() override{

    }
    // Create a RPS Configuration and set up its internal state for testing
    Configuration createConfigForTesting() {
        std::ifstream ifs("games/rock-paper-scissors.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();
        //std::cout << sourceCode << std::endl;

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node configNode = root.getChildByFieldName("configuration");

        Configuration config{&configNode, sourceCode};
        return config;
    }
    // Use an incomplete game file to make a Configuration
    Configuration createIncompleteGameFile(){       
        std::stringstream buffer;
        //Incomplete game file
        buffer << R"(
            configuration {}
        })";
        // Get the contents of the stringstream as a std::string
        std::string sourceCode = buffer.str();
        //std::cout << sourceCode << std::endl;

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node configNode = root.getChildByFieldName("configuration");
        Configuration config{&configNode, sourceCode};
        return config;
    }
};

TEST_F(ConfigurationTest, checkGameName){
    Configuration config = createConfigForTesting();
    EXPECT_EQ(config.getName(), "\"Rock, Paper, Scissors\"");
}

TEST_F(ConfigurationTest, checkPlayerRange) {
    Configuration config = createConfigForTesting();
    // Access the configuration object and check if player range is as expected
    auto playerRange = config.getPlayerRange();
    EXPECT_EQ(playerRange.first, 2);
    EXPECT_EQ(playerRange.second, 4);
}

TEST_F(ConfigurationTest, checkHasAudience) {
    Configuration config = createConfigForTesting();
    ASSERT_FALSE(config.hasAudience());
}

TEST_F(ConfigurationTest, checkPrompt){
    Configuration config = createConfigForTesting();
    for (const auto& pair : config.getSetup()) {
        if (holds_alternative<IntegerKind>(pair.second)) { 
            IntegerKind intKind = get<IntegerKind>(pair.second); 
            EXPECT_EQ(intKind.prompt, "\"The number of rounds to play\"");
        } 
    }
}

TEST_F(ConfigurationTest, checkRounds){
    Configuration config = createConfigForTesting();
    for (const auto& pair : config.getSetup()) {
        if (holds_alternative<IntegerKind>(pair.second)) { 
            IntegerKind intKind = get<IntegerKind>(pair.second); 
            EXPECT_EQ(intKind.minVal, 1);
            EXPECT_EQ(intKind.maxVal, 20);
        } 
    }
}

TEST_F(ConfigurationTest, checkBooleanKind){
    Configuration config = createConfigForTesting();
        for (const auto& pair : config.getSetup()) {
            if (holds_alternative<BooleanKind>(pair.second)) { 
                BooleanKind booleanKind = get<BooleanKind>(pair.second); 
                EXPECT_EQ(booleanKind.prompt, "\"The number of rounds to play\"");
            } 
        }
}

TEST_F(ConfigurationTest, checkEnumKind){
    Configuration config = createConfigForTesting();
    for (const auto& pair : config.getSetup()) {
        if (holds_alternative<EnumKind>(pair.second)) { 
            const EnumKind& enumKind = std::get<EnumKind>(pair.second); 
            EXPECT_EQ(enumKind.prompt, "\"The number of rounds to play\"");
        } 
    }
}

TEST_F(ConfigurationTest, checkSegFault){
    // The ASSERT_DEATH macro checks whether the provided code snippet leads to a crash
    ASSERT_DEATH({Configuration config = createIncompleteGameFile();
    }, ".*");
}




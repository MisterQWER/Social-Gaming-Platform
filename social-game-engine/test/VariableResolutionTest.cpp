#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <map>
#include <variant>
#include "Value.h"
#include "VariableResolution.h"
#include "Constants.h"


using ::testing::Return;

class VariableResolutionTest : public ::testing::Test {
protected:
    void SetUp() override {

    }
    void TearDown() override {

    }

    Constants createEmptyConstants() {
        std::ifstream ifs("~/test/gameTests/empty.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createIdentifierConstants1() {
        std::ifstream ifs("~/test/gameTests/id1.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createIdentifierConstants2() {
        std::ifstream ifs("~/test/gameTests/id2.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createIdentifierConstants3() {
        std::ifstream ifs("~/test/gameTests/id3.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createListConstants() {
        std::ifstream ifs("~/test/gameTests/list.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createNumConstants() {
        std::ifstream ifs("~/test/gameTests/num.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createStringConstants() {
        std::ifstream ifs("~/test/gameTests/string.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createBoolConstants() {
        std::ifstream ifs("~/test/gameTests/bool.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }

    Constants createMultipleEntriesConstants() {
        std::ifstream ifs("~/test/gameTests/multipleEntries.game");
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string sourceCode = buffer.str();
        ifs.close();

        //Tree setup
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        ts::Tree tree = parser.parseString(sourceCode);
        ts::Node root = tree.getRootNode();
        ts::Node constantsNode = root.getChildByFieldName("constants");

        Constants constants{&constantsNode, sourceCode};
        return constants;
    }
};

TEST_F(VariableResolutionTest, checkEmptyConstantsMap) {
    Constants consts = createEmptyConstants();
    EXPECT_EQ(consts.getConstantsMap().size(), 0);
}

TEST_F(VariableResolutionTest, checkListConstantsMap) {
    Constants consts = createListConstants();
    Value emptyList;
    emptyList.variantValue = std::vector<Value>{};
    std::map<string_view, Value> expectedMap = {
            {"a", emptyList}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    std::vector<Value> expectedList = std::get<std::vector<Value>>(emptyList.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        std::vector<Value> actualList = std::get<std::vector<Value>>(it->second.variantValue);
        ASSERT_EQ(expectedList.size(), 0);
        ASSERT_EQ(actualList.size(), 0);
        EXPECT_EQ(actualList.size(), expectedList.size());
    }
}

TEST_F(VariableResolutionTest, checkID1ConstantsMap) {
    Constants consts = createIdentifierConstants1();
    Value num;
    num.variantValue = 0;
    std::map<string_view, Value> expectedMap = {
            {"a", num}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    int expectedNum = std::get<int>(num.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        int actualNum = std::get<int>(it->second.variantValue);
        EXPECT_EQ(actualNum, expectedNum);
    }
}

TEST_F(VariableResolutionTest, checkID2ConstantsMap) {
    Constants consts = createIdentifierConstants2();
    Value num;
    num.variantValue = 0;
    std::map<string_view, Value> expectedMap = {
            {"AbC__23", num}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    int expectedNum = std::get<int>(num.variantValue);
    auto it = constsMap.find("AbC__23");
    if(it != constsMap.end()) {
        int actualNum = std::get<int>(it->second.variantValue);
        EXPECT_EQ(actualNum, expectedNum);
    }
}

TEST_F(VariableResolutionTest, checkID3ConstantsMap) {
    Constants consts = createIdentifierConstants3();
    Value num;
    num.variantValue = 0;
    std::map<string_view, Value> expectedMap = {
            {"_", num}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    int expectedNum = std::get<int>(num.variantValue);
    auto it = constsMap.find("_");
    if(it != constsMap.end()) {
        int actualNum = std::get<int>(it->second.variantValue);
        EXPECT_EQ(actualNum, expectedNum);
    }
}

TEST_F(VariableResolutionTest, checkNumConstantsMap) {
    Constants consts = createNumConstants();
    Value num;
    num.variantValue = 123456;
    std::map<string_view, Value> expectedMap = {
            {"a", num}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    int expectedNum = std::get<int>(num.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        int actualNum = std::get<int>(it->second.variantValue);
        EXPECT_EQ(actualNum, expectedNum);
    }
}

TEST_F(VariableResolutionTest, checkStringConstantsMap) {
    Constants consts = createStringConstants();
    Value string;
    string.variantValue = std::string_view("Hello");
    std::map<string_view, Value> expectedMap = {
            {"a", string}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    std::string expectedString = std::get<std::string>(string.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        std::string actualString = std::get<std::string>(it->second.variantValue);
        EXPECT_EQ(actualString, expectedString);
    }
}

TEST_F(VariableResolutionTest, checkBoolConstantsMap) {
    Constants consts = createBoolConstants();
    Value boolean;
    boolean.variantValue = false;
    std::map<string_view, Value> expectedMap = {
            {"a", boolean}
    };
    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    bool expectedBool = std::get<bool>(boolean.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        bool actualBool = std::get<bool>(it->second.variantValue);
        EXPECT_EQ(actualBool, expectedBool);
    }
}

TEST_F(VariableResolutionTest, checkMultipleEntriesConstantsMap) {
    Constants consts = createMultipleEntriesConstants();
    Value e1;
    e1.variantValue = 0;
    Value e2;
    e2.variantValue = 1;
    std::map<string_view, Value> expectedMap = {
            {"a", e1},
            {"b", e2}
    };

    std::map<string_view, Value> constsMap = consts.getConstantsMap();
    EXPECT_EQ(constsMap.size(), expectedMap.size());

    bool expectedBool = std::get<bool>(e1.variantValue);
    auto it = constsMap.find("a");
    if(it != constsMap.end()) {
        bool actualBool = std::get<bool>(it->second.variantValue);
        EXPECT_EQ(actualBool, expectedBool);
    }

    expectedBool = std::get<bool>(e2.variantValue);
    it = constsMap.find("b");
    if(it != constsMap.end()) {
        bool actualBool = std::get<bool>(it->second.variantValue);
        EXPECT_EQ(actualBool, expectedBool);
    }
}
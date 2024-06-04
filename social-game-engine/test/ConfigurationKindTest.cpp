#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "configuration_kinds.hpp"

TEST(ConfigurationKindsTest, BooleanKindTest) {
    BooleanKind booleanKind;
    booleanKind.prompt = "Prompt";
    ASSERT_EQ("Prompt", booleanKind.prompt);
}

TEST(ConfigurationKindsTest, IntegerKindTest) {
    IntegerKind integerKind;
    integerKind.minVal = 2;
    integerKind.maxVal = 6;
    integerKind.prompt = "Prompt";
    ASSERT_EQ(2, integerKind.minVal);
    ASSERT_EQ(6, integerKind.maxVal);
    ASSERT_EQ("Prompt", integerKind.prompt);
}

TEST(ConfigurationKindsTest, StringKindTest) {
    StringKind stringKind;
    stringKind.str = "String";
    stringKind.prompt = "Prompt";
    ASSERT_EQ("String", stringKind.str);
    ASSERT_EQ("Prompt", stringKind.prompt);
}

TEST(ConfigurationKindsTest, EnumKindTest) {
    EnumKind enumKind;
    enumKind.choices = {
        {"fast", "A quick round with friends"},
        {"standard", "Standard play"},
        {"long", "A marathon battle against former friends"}
    };
    enumKind.prompt = "Test Prompt";
    ASSERT_EQ("Test Prompt", enumKind.prompt);
    
    // Check the choices
    ASSERT_EQ(3, enumKind.choices.size());
    ASSERT_EQ("A quick round with friends", enumKind.choices["fast"]);
    ASSERT_EQ("Standard play", enumKind.choices["standard"]);
    ASSERT_EQ("A marathon battle against former friends", enumKind.choices["long"]);
}

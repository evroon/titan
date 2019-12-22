#include "gtest/gtest.h"

#include "core/titanscript/ScriptApp.h"
#include "core/Definitions.h"
#include "core/platform/Linux.h"

const char* file = "scripts/tests/titanscript.ts";

Variant get_output(const String& p_filename, const String& p_functionname) {
	ScriptApp scriptapp(new Linux);
	return scriptapp.execute(Array<String>(p_filename, p_functionname));
}

TEST(Arithmetic, Add) {
    int result = get_output(file, "arithmetic_add");
    ASSERT_EQ(result, 42 + 3);
}

TEST(Arithmetic, Subtract) {
    int result = get_output(file, "arithmetic_subtract");
    ASSERT_EQ(result, 42 - 3);
}

void conditional_test(const String& p_functionname) {
    String result = get_output(file, p_functionname).ToString();
    ASSERT_EQ(String("success"), result);
}

TEST(Conditional, If) {
    conditional_test("conditional_if");
}

TEST(Conditional, NegateIf) {
    conditional_test("conditional_negate_if");
}

TEST(Conditional, IfElse) {
    conditional_test("conditional_if_else");
}

TEST(Conditional, IfElseIf) {
    conditional_test("conditional_if_elseif");
}

TEST(Conditional, IfElseIfElse) {
    conditional_test("conditional_if_elseifelse");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
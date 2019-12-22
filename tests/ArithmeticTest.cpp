#include "gtest/gtest.h"

#include "core/titanscript/ScriptApp.h"
#include "core/Definitions.h"
#include "core/platform/Linux.h"

Variant get_output(const String& p_filename, const String& p_functionname) {
	ScriptApp scriptapp(new Linux);
	return scriptapp.execute(Array<String>(p_filename, p_functionname));
}

TEST(Arithmetic, Add) {
    int result = get_output("scripts/tests/arithmetic.ts", "add").operator int();
    ASSERT_EQ(result, 42 + 3);
}

TEST(Arithmetic, Subtract) {
    int result = get_output("scripts/tests/arithmetic.ts", "subtract").operator int();
    ASSERT_EQ(result, 42 - 3);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
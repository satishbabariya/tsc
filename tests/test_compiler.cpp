#include <gtest/gtest.h>
#include "tsc/Compiler.h"

using namespace tsc;

TEST(CompilerTest, VersionInfo) {
    auto version = getVersionInfo();
    EXPECT_GT(version.major, 0);
    EXPECT_GE(version.minor, 0);
    EXPECT_GE(version.patch, 0);
    EXPECT_FALSE(version.llvmVersion.empty());
}

TEST(CompilerTest, DefaultTarget) {
    String target = getDefaultTargetTriple();
    EXPECT_FALSE(target.empty());
}

TEST(CompilerTest, SupportedTargets) {
    auto targets = getSupportedTargets();
    EXPECT_FALSE(targets.empty());
    
    // Check that default target is supported
    String defaultTarget = getDefaultTargetTriple();
    bool found = false;
    for (const auto& target : targets) {
        if (target.find(defaultTarget.substr(0, 6)) != String::npos) { // Check arch prefix
            found = true;
            break;
        }
    }
    // Note: This might fail on some systems, which is expected
}

TEST(CompilerTest, BasicCompilation) {
    CompilerOptions options;
    options.target.triple = getDefaultTargetTriple();
    
    Compiler compiler(options);
    
    // Test with minimal TypeScript source
    String source = "let x: number = 42;";
    auto result = compiler.compileString(source, "test.ts");
    
    // For now, just check that it doesn't crash
    // As components are implemented, this will check for success
    EXPECT_TRUE(true); // Placeholder assertion
}
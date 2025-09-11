# TSC Tests
find_package(GTest QUIET)

if(GTest_FOUND)
    # Unit tests with GTest
    add_executable(tsc_tests
        test_lexer.cpp
        test_parser.cpp
        test_ast.cpp
        test_compiler.cpp
        test_semantic_analyzer.cpp
        test_closure_semantic_analyzer.cpp
        test_closure_llvm_codegen.cpp
        test_closure_integration.cpp
        test_irbuilder_context_management.cpp
        # New comprehensive tests for critical fixes
        test_parser_comparison_operators.cpp
        test_codegen_control_flow.cpp
        test_regression_critical_fixes.cpp
        test_ast_validation.cpp
        test_integration_control_flow.cpp
        test_system_end_to_end.cpp
    )
    
    target_link_libraries(tsc_tests
        tsc_lib
        GTest::gtest
        GTest::gtest_main
        ${llvm_libs}
    )
    
    # Add tests to CTest
    add_test(NAME TSCTests COMMAND tsc_tests)
else()
    message(STATUS "GTest not found, using custom test framework")
endif()

# Custom semantic analyzer tests (no external dependencies)
# add_executable(tsc_semantic_tests
#     test_semantic_analyzer_simple.cpp
# )

# Custom closure tests (no external dependencies)
# add_executable(tsc_closure_tests
#     test_closure_validation.cpp
# )

# target_link_libraries(tsc_semantic_tests
#     tsc_lib
#     ${llvm_libs}
# )

# target_link_libraries(tsc_closure_tests
#     tsc_lib
#     ${llvm_libs}
# )

# Add custom tests to CTest
# add_test(NAME SemanticAnalyzerTests COMMAND tsc_semantic_tests)
# add_test(NAME ClosureTests COMMAND tsc_closure_tests)

# Integration tests for generic classes and LLVM bug fix validation
# add_executable(tsc_integration_tests
#     test_integration.cpp
# )

# target_link_libraries(tsc_integration_tests
#     tsc_lib
#     ${llvm_libs}
# )

# add_test(NAME IntegrationTests COMMAND tsc_integration_tests)

# Class-level generics unit tests
# add_executable(tsc_class_generics_tests
#     test_class_level_generics.cpp
# )

# target_link_libraries(tsc_class_generics_tests
#     tsc_lib
#     ${llvm_libs}
# )

# add_test(NAME ClassLevelGenericsTests COMMAND tsc_class_generics_tests)

# Integration tests using the compiler binary
add_test(NAME BasicCompilation 
         COMMAND tsc ${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ts)

# Copy test files to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ts 
               ${CMAKE_CURRENT_BINARY_DIR}/examples/hello.ts COPYONLY)
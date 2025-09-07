# TSC Tests
find_package(GTest QUIET)

if(GTest_FOUND)
    # Unit tests
    add_executable(tsc_tests
        test_lexer.cpp
        test_parser.cpp
        test_ast.cpp
        test_compiler.cpp
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
    message(STATUS "GTest not found, skipping unit tests")
endif()

# Integration tests using the compiler binary
add_test(NAME BasicCompilation 
         COMMAND tsc ${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ts)

# Copy test files to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ts 
               ${CMAKE_CURRENT_BINARY_DIR}/examples/hello.ts COPYONLY)
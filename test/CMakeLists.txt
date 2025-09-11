# CMakeLists.txt for IR Generation Tests

# Find required packages
find_package(GTest REQUIRED)
find_package(LLVM REQUIRED CONFIG)

# Include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add LLVM definitions
add_definitions(${LLVM_DEFINITIONS})

# Unit tests
add_executable(ir_generation_unit_tests
    test_ir_generation_unit.cpp
)

target_link_libraries(ir_generation_unit_tests
    GTest::gtest
    GTest::gtest_main
    ${LLVM_LIBRARIES}
)

# Integration tests
add_executable(ir_generation_integration_tests
    test_ir_generation_integration.cpp
)

target_link_libraries(ir_generation_integration_tests
    GTest::gtest
    GTest::gtest_main
    ${LLVM_LIBRARIES}
)

# System tests
add_executable(ir_generation_system_tests
    test_ir_generation_system.cpp
)

target_link_libraries(ir_generation_system_tests
    GTest::gtest
    GTest::gtest_main
    ${LLVM_LIBRARIES}
)

# Add tests to CTest
enable_testing()

add_test(NAME IRGenerationUnitTests COMMAND ir_generation_unit_tests)
add_test(NAME IRGenerationIntegrationTests COMMAND ir_generation_integration_tests)
add_test(NAME IRGenerationSystemTests COMMAND ir_generation_system_tests)

# Set test properties
set_tests_properties(IRGenerationUnitTests PROPERTIES
    TIMEOUT 60
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set_tests_properties(IRGenerationIntegrationTests PROPERTIES
    TIMEOUT 120
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set_tests_properties(IRGenerationSystemTests PROPERTIES
    TIMEOUT 300
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
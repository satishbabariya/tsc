cmake_minimum_required(VERSION 3.20)
project(TSC VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable parallel compilation (this is handled by make -j, not compiler flags)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set LLVM definitions and include directories
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# Find required LLVM libraries
# Use monolithic LLVM library if available, otherwise use components
find_library(LLVM_MONOLITHIC_LIB LLVM-20 PATHS ${LLVM_LIBRARY_DIRS})
if(LLVM_MONOLITHIC_LIB)
    set(llvm_libs ${LLVM_MONOLITHIC_LIB})
    message(STATUS "Using monolithic LLVM library: ${LLVM_MONOLITHIC_LIB}")
else()
    # Fallback to component libraries
    llvm_map_components_to_libnames(llvm_libs
        Core
        Support
        Analysis
        ExecutionEngine
        InstCombine
        Object
        RuntimeDyld
        ScalarOpts
    )
endif()

# Compiler flags for performance and debugging
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

# AddressSanitizer support
set(CMAKE_CXX_FLAGS_ASAN "-g -O1 -fsanitize=address -fno-omit-frame-pointer -DDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address")

# Warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Project directories
set(TSC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TSC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TSC_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Include directories
include_directories(${TSC_INCLUDE_DIR})

# Add subdirectories
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Create main executable
add_executable(tsc
    src/main.cpp
)

target_link_libraries(tsc
    tsc_lib
    ${llvm_libs}
)

# Install targets
install(TARGETS tsc
    RUNTIME DESTINATION bin
)
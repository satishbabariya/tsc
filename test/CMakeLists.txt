# Unit tests for generic LLVM code generation
cmake_minimum_required(VERSION 3.16)

# Find required packages
find_package(GTest REQUIRED)
find_package(LLVM REQUIRED CONFIG)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${LLVM_INCLUDE_DIRS})

# Add LLVM definitions
add_definitions(${LLVM_DEFINITIONS})

# Create the test executable
add_executable(generic_llvm_tests
    test_generic_basic_unit.cpp
)

# Link libraries
target_link_libraries(generic_llvm_tests
    tsc_lib
    ${LLVM_LIBRARIES}
    GTest::GTest
    GTest::Main
)

# Add LLVM libraries
target_link_libraries(generic_llvm_tests
    LLVMCore
    LLVMSupport
    LLVMIRReader
    LLVMAsmParser
    LLVMBitReader
    LLVMBitWriter
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMCodeGen
    LLVMSelectionDAG
    LLVMAsmPrinter
    LLVMMCDisassembler
    LLVMExecutionEngine
    LLVMRuntimeDyld
    LLVMOrcJIT
    LLVMPasses
    LLVMObjCARCOpts
    LLVMScalarOpts
    LLVMInstCombine
    LLVMVectorize
    LLVMInstrumentation
    LLVMipo
    LLVMIRReader
    LLVMAsmParser
    LLVMBitReader
    LLVMBitWriter
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMCodeGen
    LLVMSelectionDAG
    LLVMAsmPrinter
    LLVMMCDisassembler
    LLVMExecutionEngine
    LLVMRuntimeDyld
    LLVMOrcJIT
    LLVMPasses
    LLVMObjCARCOpts
    LLVMScalarOpts
    LLVMInstCombine
    LLVMVectorize
    LLVMInstrumentation
    LLVMipo
)

# Enable testing
enable_testing()

# Add test to CTest
add_test(NAME GenericLLVMCodeGenTests COMMAND generic_llvm_tests)
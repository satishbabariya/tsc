# TSC Compiler Library
add_library(tsc_lib STATIC
    # Core utilities
    utils/DiagnosticEngine.cpp
    utils/ASTPrinter.cpp
    
    # Lexer
    lexer/Lexer.cpp
    
    # Parser  
    parser/Parser.cpp
    parser/VectorTokenStream.cpp
    
    # Semantic Analysis
    semantic/TypeChecker.cpp
    semantic/SemanticAnalyzer.cpp
    semantic/SymbolTable.cpp
    semantic/TypeSystem.cpp
    
    # Code Generation
    codegen/LLVMCodeGen.cpp
    
    # AST implementation
    AST.cpp
    Token.cpp
    
    # Main compiler driver
    Compiler.cpp
)

# Link against LLVM
target_link_libraries(tsc_lib ${llvm_libs})

# Include directories
target_include_directories(tsc_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${LLVM_INCLUDE_DIRS}
)

# Compiler definitions
target_compile_definitions(tsc_lib PUBLIC ${LLVM_DEFINITIONS})
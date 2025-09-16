# TSC Compiler Library
add_library(tsc_lib STATIC
        # Core utilities
        utils/DiagnosticEngine.cpp
        utils/EnhancedDiagnosticEngine.cpp
        utils/EnhancedErrorReporting.cpp
        utils/ASTPrinter.cpp

        # Lexer
        lexer/Lexer.cpp
        
        # Refactored Lexer Modules
        lexer/LexerRefactored.cpp
        lexer/core/LexerCore.cpp
        lexer/tokens/TokenScanner.cpp
        lexer/keywords/KeywordMatcher.cpp
        lexer/scanners/CharacterClassifier.cpp

        # Parser
        parser/Parser.cpp
        parser/VectorTokenStream.cpp
        
        # Refactored Parser Modules
        parser/ParserRefactored.cpp
        parser/core/ParserCore.cpp
        parser/expressions/ExpressionParser.cpp
        parser/statements/StatementParser.cpp
        parser/declarations/DeclarationParser.cpp
        parser/grammar/GrammarRules.cpp

        # Semantic Analysis
        semantic/TypeChecker.cpp
        semantic/SemanticAnalyzer.cpp
        semantic/SymbolTable.cpp
        semantic/TypeSystem.cpp
        semantic/GenericConstraintChecker.cpp
        semantic/ModuleResolver.cpp
        semantic/DependencyScanner.cpp
        semantic/ModuleSymbolTable.cpp
        semantic/CycleDetector.cpp
        
        # Refactored Semantic Analysis Modules
        semantic/SemanticAnalyzerRefactored.cpp
        semantic/resolution/SymbolResolutionEngine.cpp
        semantic/checking/TypeCheckingEngine.cpp
        semantic/flow/FlowAnalysisEngine.cpp
        semantic/resolution/InheritanceResolver.cpp

        # Code Generation
        codegen/LLVMCodeGen.cpp
        codegen/ObjectFileGenerator.cpp
        codegen/ExecutableLinker.cpp
        
        # Refactored Code Generation Modules
        codegen/LLVMCodeGenRefactored.cpp
        codegen/context/CodeGenContext.cpp
        codegen/types/TypeGenerator.cpp
        codegen/expressions/ExpressionGenerator.cpp
        codegen/statements/StatementGenerator.cpp
        codegen/functions/FunctionGenerator.cpp
        codegen/classes/ClassGenerator.cpp
        codegen/builtins/BuiltinGenerator.cpp
        codegen/optimization/OptimizationEngine.cpp
        codegen/memory/MemoryManager.cpp

        # AST implementation
        AST.cpp
        Token.cpp

        # Main compiler driver
        Compiler.cpp
        TargetRegistry.cpp
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
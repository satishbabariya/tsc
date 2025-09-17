#pragma once

#include "tsc/Common.h"

namespace tsc {
namespace ast {
namespace declarations {
    class FunctionDeclaration;
}
namespace expressions {
    class CallExpression;
    class ArrowFunction;
}
}

namespace codegen {

class LLVMCodeGen; // Forward declaration

/**
 * FunctionGenerator handles the generation of function declarations, calls, and arrow functions.
 * It manages function creation, parameter handling, and call generation.
 */
class FunctionGenerator {
public:
    explicit FunctionGenerator(LLVMCodeGen* codeGen);
    
    // Function declaration generation
    llvm::Function* generateFunctionDeclaration(const ast::declarations::FunctionDeclaration& funcDecl);
    void generateFunctionBody(llvm::Function* function, const ast::declarations::FunctionDeclaration& funcDecl);
    
    // Function call generation
    llvm::Value* generateCallExpression(const ast::expressions::CallExpression& callExpr);
    llvm::Value* generateDirectFunctionCall(llvm::Function* function, const ast::expressions::CallExpression& callExpr);
    llvm::Value* generateIndirectFunctionCall(llvm::Value* callee, const ast::expressions::CallExpression& callExpr);
    
    // Arrow function generation
    llvm::Function* generateArrowFunction(const ast::expressions::ArrowFunction& arrowFunc);
    void generateArrowFunctionBody(llvm::Function* function, const ast::expressions::ArrowFunction& arrowFunc);
    
    // Utility functions
    bool hasReturnStatements(const ast::declarations::FunctionDeclaration& funcDecl) const;
    bool hasReturnStatementsWithValues(const ast::declarations::FunctionDeclaration& funcDecl) const;
    llvm::Value* createDefaultReturnValue(llvm::Type* returnType);
    void ensureBlockTerminators(llvm::Function* function);
    
private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc
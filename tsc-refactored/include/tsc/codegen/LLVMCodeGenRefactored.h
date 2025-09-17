#pragma once

#include "tsc/Common.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/ast/visitors/ASTVisitor.h"

namespace tsc {

// Forward declarations
class SymbolTable;
struct CompilerOptions;

namespace ast {
    class Module;
    class Declaration;
    class Expression;
    class Statement;
    
    namespace declarations {
        class FunctionDeclaration;
        class VariableDeclaration;
        class ClassDeclaration;
    }
    
    namespace expressions {
        class NumericLiteral;
        class StringLiteral;
        class BooleanLiteral;
        class NullLiteral;
        class Identifier;
        class BinaryExpression;
        class UnaryExpression;
        class CallExpression;
    }
    
    namespace statements {
        class ExpressionStatement;
        class BlockStatement;
        class ReturnStatement;
        class IfStatement;
        class WhileStatement;
    }
}

namespace codegen {

// Forward declarations
class CodeGenContext;
class TypeGenerator;
class ExpressionGenerator;
class StatementGenerator;
class FunctionGenerator;
class BuiltinGenerator;

/**
 * LLVMCodeGenRefactored is the main orchestrator for LLVM code generation.
 * It coordinates all the specialized generators and provides a unified interface
 * for generating LLVM IR from TypeScript AST nodes.
 */
class LLVMCodeGenRefactored : public ast::visitors::ASTVisitor {
public:
    LLVMCodeGenRefactored(DiagnosticEngine& diagnostics, const CompilerOptions& options);
    ~LLVMCodeGenRefactored();
    
    // Main code generation interface
    bool generateCode(ast::Module& astModule);
    bool writeOutput(const String& filename);
    
    // High-level generation methods
    void generateDeclaration(ast::Declaration& decl);
    llvm::Value* generateExpression(ast::Expression& expr);
    void generateStatement(ast::Statement& stmt);
    
    // Visitor interface for declarations
    void visit(ast::declarations::FunctionDeclaration& node) override;
    void visit(ast::declarations::VariableDeclaration& node) override;
    void visit(ast::declarations::ClassDeclaration& node) override;
    
    // Visitor interface for expressions
    void visit(ast::expressions::NumericLiteral& node) override;
    void visit(ast::expressions::StringLiteral& node) override;
    void visit(ast::expressions::BooleanLiteral& node) override;
    void visit(ast::expressions::NullLiteral& node) override;
    void visit(ast::expressions::Identifier& node) override;
    void visit(ast::expressions::BinaryExpression& node) override;
    void visit(ast::expressions::UnaryExpression& node) override;
    void visit(ast::expressions::CallExpression& node) override;
    
    // Visitor interface for statements
    void visit(ast::statements::ExpressionStatement& node) override;
    void visit(ast::statements::BlockStatement& node) override;
    void visit(ast::statements::ReturnStatement& node) override;
    void visit(ast::statements::IfStatement& node) override;
    void visit(ast::statements::WhileStatement& node) override;
    
    // Utility methods
    llvm::Value* createDefaultValue(llvm::Type* type);
    bool verifyModule();
    void reportError(const String& message, const SourceLocation& location);
    bool hasErrors() const;
    
    // Accessors for generators and context
    llvm::LLVMContext* getLLVMContext();
    llvm::Module* getModule();
    llvm::IRBuilder<>* getBuilder();
    CodeGenContext* getCodeGenContext();
    TypeGenerator* getTypeGenerator();
    ExpressionGenerator* getExpressionGenerator();
    StatementGenerator* getStatementGenerator();
    FunctionGenerator* getFunctionGenerator();
    BuiltinGenerator* getBuiltinGenerator();
    
    // Symbol table access
    SymbolTable* getSymbolTable();
    void setSymbolTable(SymbolTable* symbolTable);
    
    // Current value tracking (for visitor pattern)
    llvm::Value* getCurrentValue() const;
    void setCurrentValue(llvm::Value* value);
    
private:
    // Core LLVM components
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    
    // Code generation components
    std::unique_ptr<CodeGenContext> codeGenContext_;
    std::unique_ptr<TypeGenerator> typeGenerator_;
    std::unique_ptr<ExpressionGenerator> expressionGenerator_;
    std::unique_ptr<StatementGenerator> statementGenerator_;
    std::unique_ptr<FunctionGenerator> functionGenerator_;
    std::unique_ptr<BuiltinGenerator> builtinGenerator_;
    
    // External dependencies
    DiagnosticEngine& diagnostics_;
    const CompilerOptions& options_;
    SymbolTable* symbolTable_ = nullptr;
    
    // Current value for visitor pattern
    llvm::Value* currentValue_;
};

} // namespace codegen
} // namespace tsc
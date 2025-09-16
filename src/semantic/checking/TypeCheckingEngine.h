#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {
namespace semantic {

// Forward declarations
class SemanticContext;

// Type checking engine for performing type analysis
class TypeCheckingEngine {
public:
    explicit TypeCheckingEngine(SemanticContext& context);
    
    // Main type checking interface
    void performTypeChecking(Module& module);
    
    // Expression type checking
    shared_ptr<Type> checkExpression(ASTNode& expression);
    shared_ptr<Type> checkNumericLiteral(const NumericLiteral& node);
    shared_ptr<Type> checkStringLiteral(const StringLiteral& node);
    shared_ptr<Type> checkBooleanLiteral(const BooleanLiteral& node);
    shared_ptr<Type> checkNullLiteral(const NullLiteral& node);
    shared_ptr<Type> checkIdentifier(const Identifier& node);
    shared_ptr<Type> checkBinaryExpression(const BinaryExpression& node);
    shared_ptr<Type> checkUnaryExpression(const UnaryExpression& node);
    shared_ptr<Type> checkCallExpression(const CallExpression& node);
    shared_ptr<Type> checkPropertyAccess(const PropertyAccess& node);
    shared_ptr<Type> checkArrayLiteral(const ArrayLiteral& node);
    shared_ptr<Type> checkObjectLiteral(const ObjectLiteral& node);
    
    // Statement type checking
    void checkStatement(ASTNode& statement);
    void checkVariableDeclaration(const VariableDeclaration& node);
    void checkFunctionDeclaration(const FunctionDeclaration& node);
    void checkReturnStatement(const ReturnStatement& node);
    void checkIfStatement(const IfStatement& node);
    void checkWhileStatement(const WhileStatement& node);
    void checkForStatement(const ForStatement& node);
    
    // Class and interface type checking
    void checkClassDeclaration(const ClassDeclaration& node);
    void checkInterfaceDeclaration(const InterfaceDeclaration& node);
    void checkMethodDeclaration(const MethodDeclaration& node);
    void checkPropertyDeclaration(const PropertyDeclaration& node);
    
    // Type compatibility checking
    bool isTypeCompatible(shared_ptr<Type> from, shared_ptr<Type> to);
    bool isAssignable(shared_ptr<Type> from, shared_ptr<Type> to);
    bool isCallable(shared_ptr<Type> type);
    
    // Type inference
    shared_ptr<Type> inferType(ASTNode& node);
    shared_ptr<Type> inferBinaryExpressionType(const BinaryExpression& node);
    shared_ptr<Type> inferCallExpressionType(const CallExpression& node);

private:
    SemanticContext& context_;
    
    // Helper methods
    void checkTypeAnnotation(shared_ptr<Type> annotation, shared_ptr<Type> inferred);
    void reportTypeError(const String& message, const SourceLocation& location);
    void reportTypeWarning(const String& message, const SourceLocation& location);
};

} // namespace semantic
} // namespace tsc
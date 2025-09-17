#include "tsc/semantic/checking/TypeCheckingEngine.h"
#include "tsc/semantic/SemanticAnalyzerRefactored.h"

namespace tsc {
namespace semantic {

TypeCheckingEngine::TypeCheckingEngine(SemanticContext& context) 
    : context_(context) {
}

void TypeCheckingEngine::performTypeChecking(Module& module) {
    // TODO: Implement module type checking
}

shared_ptr<Type> TypeCheckingEngine::checkExpression(ASTNode& expression) {
    // TODO: Implement expression type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkNumericLiteral(const NumericLiteral& node) {
    return context_.getTypeSystem().getNumberType();
}

shared_ptr<Type> TypeCheckingEngine::checkStringLiteral(const StringLiteral& node) {
    return context_.getTypeSystem().getStringType();
}

shared_ptr<Type> TypeCheckingEngine::checkBooleanLiteral(const BooleanLiteral& node) {
    return context_.getTypeSystem().getBooleanType();
}

shared_ptr<Type> TypeCheckingEngine::checkNullLiteral(const NullLiteral& node) {
    return context_.getTypeSystem().getNullType();
}

shared_ptr<Type> TypeCheckingEngine::checkIdentifier(const Identifier& node) {
    // TODO: Implement identifier type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkBinaryExpression(const BinaryExpression& node) {
    // TODO: Implement binary expression type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkUnaryExpression(const UnaryExpression& node) {
    // TODO: Implement unary expression type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkCallExpression(const CallExpression& node) {
    // TODO: Implement call expression type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkPropertyAccess(const PropertyAccess& node) {
    // TODO: Implement property access type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkArrayLiteral(const ArrayLiteral& node) {
    // TODO: Implement array literal type checking
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::checkObjectLiteral(const ObjectLiteral& node) {
    // TODO: Implement object literal type checking
    return nullptr;
}

void TypeCheckingEngine::checkStatement(ASTNode& statement) {
    // TODO: Implement statement type checking
}

void TypeCheckingEngine::checkVariableDeclaration(const VariableDeclaration& node) {
    // TODO: Implement variable declaration type checking
}

void TypeCheckingEngine::checkFunctionDeclaration(const FunctionDeclaration& node) {
    // TODO: Implement function declaration type checking
}

void TypeCheckingEngine::checkReturnStatement(const ReturnStatement& node) {
    // TODO: Implement return statement type checking
}

void TypeCheckingEngine::checkIfStatement(const IfStatement& node) {
    // TODO: Implement if statement type checking
}

void TypeCheckingEngine::checkWhileStatement(const WhileStatement& node) {
    // TODO: Implement while statement type checking
}

void TypeCheckingEngine::checkForStatement(const ForStatement& node) {
    // TODO: Implement for statement type checking
}

void TypeCheckingEngine::checkClassDeclaration(const ClassDeclaration& node) {
    // TODO: Implement class declaration type checking
}

void TypeCheckingEngine::checkInterfaceDeclaration(const InterfaceDeclaration& node) {
    // TODO: Implement interface declaration type checking
}

void TypeCheckingEngine::checkMethodDeclaration(const MethodDeclaration& node) {
    // TODO: Implement method declaration type checking
}

void TypeCheckingEngine::checkPropertyDeclaration(const PropertyDeclaration& node) {
    // TODO: Implement property declaration type checking
}

bool TypeCheckingEngine::isTypeCompatible(shared_ptr<Type> from, shared_ptr<Type> to) {
    // TODO: Implement type compatibility checking
    return false;
}

bool TypeCheckingEngine::isAssignable(shared_ptr<Type> from, shared_ptr<Type> to) {
    // TODO: Implement assignability checking
    return false;
}

bool TypeCheckingEngine::isCallable(shared_ptr<Type> type) {
    // TODO: Implement callable type checking
    return false;
}

shared_ptr<Type> TypeCheckingEngine::inferType(ASTNode& node) {
    // TODO: Implement type inference
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::inferBinaryExpressionType(const BinaryExpression& node) {
    // TODO: Implement binary expression type inference
    return nullptr;
}

shared_ptr<Type> TypeCheckingEngine::inferCallExpressionType(const CallExpression& node) {
    // TODO: Implement call expression type inference
    return nullptr;
}

void TypeCheckingEngine::checkTypeAnnotation(shared_ptr<Type> annotation, shared_ptr<Type> inferred) {
    // TODO: Implement type annotation checking
}

void TypeCheckingEngine::reportTypeError(const String& message, const SourceLocation& location) {
    context_.reportError(message, location);
}

void TypeCheckingEngine::reportTypeWarning(const String& message, const SourceLocation& location) {
    context_.reportWarning(message, location);
}

void TypeCheckingEngine::checkAssignment(const Expression& left, const Expression& right, const SourceLocation& location) {
    auto leftType = checkExpression(const_cast<Expression&>(left));
    auto rightType = checkExpression(const_cast<Expression&>(right));

    // Special handling for array literal assignments
    if (auto arrayLiteral = dynamic_cast<const ArrayLiteral*>(&right)) {
        if (leftType->getKind() == TypeKind::Array && rightType->getKind() == TypeKind::Array) {
            auto leftArrayType = static_cast<const ArrayType*>(leftType.get());
            auto rightArrayType = static_cast<const ArrayType*>(rightType.get());

            // If the right side is an empty array (any[]), and the left side is a generic array (T[]),
            // allow the assignment since empty arrays can be assigned to any array type
            if (arrayLiteral->getElements().empty() &&
                rightArrayType->getElementType()->getKind() == TypeKind::Any &&
                (leftArrayType->getElementType()->getKind() == TypeKind::Generic ||
                 leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                 leftArrayType->getElementType()->getKind() == TypeKind::Unresolved)) {
                // This is a valid assignment: [] can be assigned to T[]
                return;
            }

            // If both sides are generic arrays with the same generic type parameter,
            // allow the assignment even if they're different instances
            if ((leftArrayType->getElementType()->getKind() == TypeKind::Generic ||
                 leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                 leftArrayType->getElementType()->getKind() == TypeKind::Unresolved) &&
                (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                 rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                 rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) {
                // For TypeParameter types, we can directly compare the toString() values
                if (leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter &&
                    rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter) {
                    if (leftArrayType->getElementType()->toString() == rightArrayType->getElementType()->toString()) {
                        // This is a valid assignment: T[] can be assigned to T[]
                        return;
                    }
                }
                // Handle mixed Generic, TypeParameter, and Unresolved types
                if ((leftArrayType->getElementType()->getKind() == TypeKind::Generic &&
                     (rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                      rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) ||
                    (leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter &&
                     (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                      rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) ||
                    (leftArrayType->getElementType()->getKind() == TypeKind::Unresolved &&
                     (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                      rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter))) {
                    // This is a valid assignment: mixed generic types can be assigned
                    return;
                }
            }
        }
    }

    // General type compatibility check
    if (!isAssignable(rightType, leftType)) {
        reportTypeError(
            "Type mismatch: cannot assign " + rightType->toString() + " to " + leftType->toString(),
            location
        );
    }
}

} // namespace semantic
} // namespace tsc
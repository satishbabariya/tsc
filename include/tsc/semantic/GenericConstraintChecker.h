#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {

class DiagnosticEngine;

/**
 * GenericConstraintChecker handles type constraint validation and variance checking
 * for advanced generics in the TypeScript compiler.
 */
class GenericConstraintChecker {
public:
    explicit GenericConstraintChecker(DiagnosticEngine& diagnostics, TypeSystem& typeSystem);
    ~GenericConstraintChecker() = default;

    // Type constraint checking
    bool checkTypeConstraint(const TypeParameter& typeParam, shared_ptr<Type> typeArgument, 
                           const SourceLocation& location);
    bool checkExtendsConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument, 
                              const SourceLocation& location);
    bool checkVarianceConstraint(const TypeParameter& typeParam, shared_ptr<Type> typeArgument, 
                               const SourceLocation& location);

    // Variance checking
    bool checkCovariance(shared_ptr<Type> from, shared_ptr<Type> to, const SourceLocation& location);
    bool checkContravariance(shared_ptr<Type> from, shared_ptr<Type> to, const SourceLocation& location);
    bool checkInvariance(shared_ptr<Type> from, shared_ptr<Type> to, const SourceLocation& location);

    // Generic instantiation validation
    bool validateGenericInstantiation(shared_ptr<Type> genericType, 
                                    const std::vector<shared_ptr<Type>>& typeArguments,
                                    const SourceLocation& location);

    // Type parameter substitution
    shared_ptr<Type> substituteTypeParameters(shared_ptr<Type> type, 
                                            const std::unordered_map<String, shared_ptr<Type>>& substitutions);

    // Constraint satisfaction checking
    bool satisfiesConstraint(shared_ptr<Type> type, shared_ptr<Type> constraint);
    bool isSubtypeOf(shared_ptr<Type> subtype, shared_ptr<Type> supertype);

    // Error reporting
    void reportConstraintError(const String& message, const SourceLocation& location);
    void reportVarianceError(const String& message, const SourceLocation& location);

private:
    DiagnosticEngine& diagnostics_;
    TypeSystem& typeSystem_;

    // Helper methods for constraint checking
    bool checkPrimitiveConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument);
    bool checkClassConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument);
    bool checkInterfaceConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument);
    bool checkUnionConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument);
    bool checkIntersectionConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument);

    // Helper methods for variance checking
    bool checkGenericVariance(shared_ptr<Type> from, shared_ptr<Type> to, Variance variance);
    bool checkArrayVariance(shared_ptr<Type> from, shared_ptr<Type> to, Variance variance);
    bool checkFunctionVariance(shared_ptr<Type> from, shared_ptr<Type> to, Variance variance);
    bool checkObjectVariance(shared_ptr<Type> from, shared_ptr<Type> to, Variance variance);

    // Type substitution helpers
    shared_ptr<Type> substituteInType(shared_ptr<Type> type, 
                                    const std::unordered_map<String, shared_ptr<Type>>& substitutions);
    std::vector<shared_ptr<Type>> substituteInTypeList(const std::vector<shared_ptr<Type>>& types,
                                                      const std::unordered_map<String, shared_ptr<Type>>& substitutions);
};

} // namespace tsc
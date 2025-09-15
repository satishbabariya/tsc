#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// DESTRUCTURING SEMANTIC ANALYZER
// =============================================================================

class DestructuringSemanticAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main analysis methods
    bool analyzeRightHandSide(Expression* rightHandSide);
    bool analyzeDestructuringPattern(DestructuringPattern* pattern);
    
    // Pattern analysis methods
    bool analyzeArrayDestructuringPattern(ArrayDestructuringPattern* pattern);
    bool analyzeObjectDestructuringPattern(ObjectDestructuringPattern* pattern);
    
    // Element and property analysis methods
    bool analyzeArrayDestructuringElement(ASTNode* element, size_t index);
    bool analyzeObjectDestructuringProperty(ASTNode* property);
    bool analyzeDestructuringProperty(DestructuringProperty* property);
    bool analyzeDestructuringValue(ASTNode* value);
    
    // Specific pattern analysis methods
    bool analyzeIdentifierDestructuring(Identifier* identifier);
    bool analyzeNestedArrayDestructuring(ArrayDestructuringPattern* pattern);
    bool analyzeNestedObjectDestructuring(ObjectDestructuringPattern* pattern);
    bool analyzeAssignmentPattern(AssignmentPattern* pattern);
    
    // Rest element analysis methods
    bool analyzeArrayRestElement(ASTNode* restElement);
    bool analyzeObjectRestElement(ASTNode* restElement);
    
    // Property key analysis methods
    bool analyzePropertyKey(ASTNode* key);
    bool analyzeIdentifierPropertyKey(Identifier* identifier);
    bool analyzeLiteralPropertyKey(Literal* literal);
    bool analyzeComputedPropertyKey(ComputedPropertyName* computedProperty);
    
    // Semantic checking methods
    bool performSemanticChecks(DestructuringPattern* pattern, Expression* rightHandSide);
    bool checkForPotentialIssues(DestructuringPattern* pattern, Expression* rightHandSide);
    bool checkPerformanceImplications(DestructuringPattern* pattern, Expression* rightHandSide);
    bool checkBestPractices(DestructuringPattern* pattern, Expression* rightHandSide);
    
    // Helper methods
    bool isValidIdentifierName(const std::string& name);
    bool isReservedKeyword(const std::string& name);
    bool addIdentifierToSymbolTable(Identifier* identifier);
    bool isExpressionIterable(Expression* expr);
    bool isExpressionObjectLike(Expression* expr);
    bool isValidPropertyKeyLiteral(Literal* literal);
    
    // Issue detection methods
    bool isPotentiallyNull(Expression* expr);
    bool isEmptyDestructuringPattern(DestructuringPattern* pattern);
    bool hasUnusedDestructuredVariables(DestructuringPattern* pattern);
    bool isExpensiveExpression(Expression* expr);
    bool isLargeDestructuringPattern(DestructuringPattern* pattern);
    bool hasConsistentNaming(DestructuringPattern* pattern);
    bool hasAppropriateRestElements(DestructuringPattern* pattern);
    
public:
    DestructuringSemanticAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis method
    bool analyzeDestructuringAssignment(DestructuringPattern* pattern, Expression* rightHandSide);
};

} // namespace semantic
} // namespace tsc
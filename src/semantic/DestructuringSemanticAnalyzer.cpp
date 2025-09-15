#include "tsc/semantic/DestructuringSemanticAnalyzer.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

// =============================================================================
// DESTRUCTURING SEMANTIC ANALYZER IMPLEMENTATION
// =============================================================================

DestructuringSemanticAnalyzer::DestructuringSemanticAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool DestructuringSemanticAnalyzer::analyzeDestructuringAssignment(DestructuringPattern* pattern, Expression* rightHandSide) {
    if (!pattern || !rightHandSide) {
        std::cerr << "Destructuring pattern or right-hand side is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing destructuring assignment" << std::endl;
    
    // Analyze the right-hand side expression
    if (!analyzeRightHandSide(rightHandSide)) {
        std::cerr << "Failed to analyze right-hand side" << std::endl;
        return false;
    }
    
    // Analyze the destructuring pattern
    if (!analyzeDestructuringPattern(pattern)) {
        std::cerr << "Failed to analyze destructuring pattern" << std::endl;
        return false;
    }
    
    // Perform semantic checks
    if (!performSemanticChecks(pattern, rightHandSide)) {
        std::cerr << "Semantic checks failed" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeRightHandSide(Expression* rightHandSide) {
    if (!rightHandSide) {
        std::cerr << "Right-hand side is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing right-hand side expression" << std::endl;
    
    // Check if the expression is valid
    if (!analyzer_->checkExpression(rightHandSide)) {
        std::cerr << "Right-hand side expression is invalid" << std::endl;
        return false;
    }
    
    // Check if the expression is iterable (for array destructuring)
    if (rightHandSide->getType() == ASTNodeType::ArrayDestructuringPattern) {
        if (!isExpressionIterable(rightHandSide)) {
            std::cerr << "Right-hand side is not iterable" << std::endl;
            return false;
        }
    }
    
    // Check if the expression is object-like (for object destructuring)
    if (rightHandSide->getType() == ASTNodeType::ObjectDestructuringPattern) {
        if (!isExpressionObjectLike(rightHandSide)) {
            std::cerr << "Right-hand side is not object-like" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeDestructuringPattern(DestructuringPattern* pattern) {
    if (!pattern) {
        std::cerr << "Destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing destructuring pattern" << std::endl;
    
    switch (pattern->getType()) {
        case ASTNodeType::ArrayDestructuringPattern:
            return analyzeArrayDestructuringPattern(static_cast<ArrayDestructuringPattern*>(pattern));
        case ASTNodeType::ObjectDestructuringPattern:
            return analyzeObjectDestructuringPattern(static_cast<ObjectDestructuringPattern*>(pattern));
        default:
            std::cerr << "Unknown destructuring pattern type" << std::endl;
            return false;
    }
}

bool DestructuringSemanticAnalyzer::analyzeArrayDestructuringPattern(ArrayDestructuringPattern* pattern) {
    if (!pattern) {
        std::cerr << "Array destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing array destructuring pattern" << std::endl;
    
    // Analyze each element in the pattern
    auto elements = pattern->getElements();
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!analyzeArrayDestructuringElement(elements[i], i)) {
            std::cerr << "Failed to analyze array destructuring element at index " << i << std::endl;
            return false;
        }
    }
    
    // Analyze rest element if present
    if (pattern->hasRestElement()) {
        if (!analyzeArrayRestElement(pattern->getRestElement())) {
            std::cerr << "Failed to analyze array rest element" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeObjectDestructuringPattern(ObjectDestructuringPattern* pattern) {
    if (!pattern) {
        std::cerr << "Object destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing object destructuring pattern" << std::endl;
    
    // Analyze each property in the pattern
    auto properties = pattern->getProperties();
    for (auto& property : properties) {
        if (!analyzeObjectDestructuringProperty(property)) {
            std::cerr << "Failed to analyze object destructuring property" << std::endl;
            return false;
        }
    }
    
    // Analyze rest element if present
    if (pattern->hasRestElement()) {
        if (!analyzeObjectRestElement(pattern->getRestElement())) {
            std::cerr << "Failed to analyze object rest element" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeArrayDestructuringElement(ASTNode* element, size_t index) {
    if (!element) {
        // Empty slot in array destructuring (e.g., [a, , c])
        return true;
    }
    
    std::cout << "Analyzing array destructuring element at index " << index << std::endl;
    
    switch (element->getType()) {
        case ASTNodeType::Identifier:
            return analyzeIdentifierDestructuring(static_cast<Identifier*>(element));
        case ASTNodeType::ArrayDestructuringPattern:
            return analyzeNestedArrayDestructuring(static_cast<ArrayDestructuringPattern*>(element));
        case ASTNodeType::ObjectDestructuringPattern:
            return analyzeNestedObjectDestructuring(static_cast<ObjectDestructuringPattern*>(element));
        case ASTNodeType::AssignmentPattern:
            return analyzeAssignmentPattern(static_cast<AssignmentPattern*>(element));
        default:
            std::cerr << "Unknown array destructuring element type" << std::endl;
            return false;
    }
}

bool DestructuringSemanticAnalyzer::analyzeObjectDestructuringProperty(ASTNode* property) {
    if (!property) {
        std::cerr << "Object destructuring property is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing object destructuring property" << std::endl;
    
    if (property->getType() == ASTNodeType::DestructuringProperty) {
        auto destructuringProperty = static_cast<DestructuringProperty*>(property);
        return analyzeDestructuringProperty(destructuringProperty);
    }
    
    std::cerr << "Unknown object destructuring property type" << std::endl;
    return false;
}

bool DestructuringSemanticAnalyzer::analyzeDestructuringProperty(DestructuringProperty* property) {
    if (!property) {
        std::cerr << "Destructuring property is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing destructuring property" << std::endl;
    
    // Analyze the property key
    ASTNode* key = property->getKey();
    if (!key) {
        std::cerr << "Destructuring property key is null" << std::endl;
        return false;
    }
    
    if (!analyzePropertyKey(key)) {
        std::cerr << "Failed to analyze property key" << std::endl;
        return false;
    }
    
    // Analyze the property value (the variable being destructured)
    ASTNode* value = property->getValue();
    if (!value) {
        std::cerr << "Destructuring property value is null" << std::endl;
        return false;
    }
    
    return analyzeDestructuringValue(value);
}

bool DestructuringSemanticAnalyzer::analyzeDestructuringValue(ASTNode* value) {
    if (!value) {
        std::cerr << "Destructuring value is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing destructuring value" << std::endl;
    
    switch (value->getType()) {
        case ASTNodeType::Identifier:
            return analyzeIdentifierDestructuring(static_cast<Identifier*>(value));
        case ASTNodeType::ArrayDestructuringPattern:
            return analyzeNestedArrayDestructuring(static_cast<ArrayDestructuringPattern*>(value));
        case ASTNodeType::ObjectDestructuringPattern:
            return analyzeNestedObjectDestructuring(static_cast<ObjectDestructuringPattern*>(value));
        case ASTNodeType::AssignmentPattern:
            return analyzeAssignmentPattern(static_cast<AssignmentPattern*>(value));
        default:
            std::cerr << "Unknown destructuring value type" << std::endl;
            return false;
    }
}

bool DestructuringSemanticAnalyzer::analyzeIdentifierDestructuring(Identifier* identifier) {
    if (!identifier) {
        std::cerr << "Identifier is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing identifier destructuring: " << identifier->getName() << std::endl;
    
    // Check if identifier name is valid
    if (!isValidIdentifierName(identifier->getName())) {
        std::cerr << "Invalid identifier name: " << identifier->getName() << std::endl;
        return false;
    }
    
    // Check if identifier is a reserved keyword
    if (isReservedKeyword(identifier->getName())) {
        std::cerr << "Identifier is a reserved keyword: " << identifier->getName() << std::endl;
        return false;
    }
    
    // Check if identifier is already declared in current scope
    auto symbolTable = analyzer_->getSymbolTable();
    if (symbolTable->lookupSymbol(identifier->getName())) {
        std::cerr << "Identifier already declared: " << identifier->getName() << std::endl;
        return false;
    }
    
    // Add identifier to symbol table
    if (!addIdentifierToSymbolTable(identifier)) {
        std::cerr << "Failed to add identifier to symbol table" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeNestedArrayDestructuring(ArrayDestructuringPattern* pattern) {
    if (!pattern) {
        std::cerr << "Nested array destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing nested array destructuring" << std::endl;
    
    // Recursively analyze the nested pattern
    return analyzeArrayDestructuringPattern(pattern);
}

bool DestructuringSemanticAnalyzer::analyzeNestedObjectDestructuring(ObjectDestructuringPattern* pattern) {
    if (!pattern) {
        std::cerr << "Nested object destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing nested object destructuring" << std::endl;
    
    // Recursively analyze the nested pattern
    return analyzeObjectDestructuringPattern(pattern);
}

bool DestructuringSemanticAnalyzer::analyzeAssignmentPattern(AssignmentPattern* pattern) {
    if (!pattern) {
        std::cerr << "Assignment pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing assignment pattern" << std::endl;
    
    // Analyze the left side (destructuring pattern)
    ASTNode* left = pattern->getLeft();
    if (!left) {
        std::cerr << "Assignment pattern left side is null" << std::endl;
        return false;
    }
    
    if (!analyzeDestructuringValue(left)) {
        std::cerr << "Failed to analyze assignment pattern left side" << std::endl;
        return false;
    }
    
    // Analyze the right side (default value)
    ASTNode* right = pattern->getRight();
    if (!right) {
        std::cerr << "Assignment pattern right side is null" << std::endl;
        return false;
    }
    
    if (!analyzer_->checkExpression(static_cast<Expression*>(right))) {
        std::cerr << "Failed to analyze assignment pattern right side" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeArrayRestElement(ASTNode* restElement) {
    if (!restElement) {
        std::cerr << "Array rest element is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing array rest element" << std::endl;
    
    if (restElement->getType() == ASTNodeType::RestElement) {
        auto rest = static_cast<RestElement*>(restElement);
        ASTNode* argument = rest->getArgument();
        
        if (!argument) {
            std::cerr << "Rest element argument is null" << std::endl;
            return false;
        }
        
        // Check if argument is an identifier
        if (argument->getType() == ASTNodeType::Identifier) {
            auto identifier = static_cast<Identifier*>(argument);
            return analyzeIdentifierDestructuring(identifier);
        }
    }
    
    std::cerr << "Invalid array rest element" << std::endl;
    return false;
}

bool DestructuringSemanticAnalyzer::analyzeObjectRestElement(ASTNode* restElement) {
    if (!restElement) {
        std::cerr << "Object rest element is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing object rest element" << std::endl;
    
    if (restElement->getType() == ASTNodeType::RestElement) {
        auto rest = static_cast<RestElement*>(restElement);
        ASTNode* argument = rest->getArgument();
        
        if (!argument) {
            std::cerr << "Rest element argument is null" << std::endl;
            return false;
        }
        
        // Check if argument is an identifier
        if (argument->getType() == ASTNodeType::Identifier) {
            auto identifier = static_cast<Identifier*>(argument);
            return analyzeIdentifierDestructuring(identifier);
        }
    }
    
    std::cerr << "Invalid object rest element" << std::endl;
    return false;
}

bool DestructuringSemanticAnalyzer::analyzePropertyKey(ASTNode* key) {
    if (!key) {
        std::cerr << "Property key is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing property key" << std::endl;
    
    switch (key->getType()) {
        case ASTNodeType::Identifier:
            return analyzeIdentifierPropertyKey(static_cast<Identifier*>(key));
        case ASTNodeType::Literal:
            return analyzeLiteralPropertyKey(static_cast<Literal*>(key));
        case ASTNodeType::ComputedPropertyName:
            return analyzeComputedPropertyKey(static_cast<ComputedPropertyName*>(key));
        default:
            std::cerr << "Unknown property key type" << std::endl;
            return false;
    }
}

bool DestructuringSemanticAnalyzer::analyzeIdentifierPropertyKey(Identifier* identifier) {
    if (!identifier) {
        std::cerr << "Identifier property key is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing identifier property key: " << identifier->getName() << std::endl;
    
    // Check if identifier name is valid
    if (!isValidIdentifierName(identifier->getName())) {
        std::cerr << "Invalid identifier property key name: " << identifier->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeLiteralPropertyKey(Literal* literal) {
    if (!literal) {
        std::cerr << "Literal property key is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing literal property key" << std::endl;
    
    // Check if literal value is valid for property key
    if (!isValidPropertyKeyLiteral(literal)) {
        std::cerr << "Invalid literal property key" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::analyzeComputedPropertyKey(ComputedPropertyName* computedProperty) {
    if (!computedProperty) {
        std::cerr << "Computed property key is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing computed property key" << std::endl;
    
    // Analyze the expression inside the computed property
    ASTNode* expression = computedProperty->getExpression();
    if (!expression) {
        std::cerr << "Computed property expression is null" << std::endl;
        return false;
    }
    
    if (!analyzer_->checkExpression(static_cast<Expression*>(expression))) {
        std::cerr << "Failed to analyze computed property expression" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::performSemanticChecks(DestructuringPattern* pattern, Expression* rightHandSide) {
    std::cout << "Performing semantic checks" << std::endl;
    
    // Check for potential issues
    if (!checkForPotentialIssues(pattern, rightHandSide)) {
        std::cerr << "Potential issues check failed" << std::endl;
        return false;
    }
    
    // Check for performance implications
    if (!checkPerformanceImplications(pattern, rightHandSide)) {
        std::cerr << "Performance implications check failed" << std::endl;
        return false;
    }
    
    // Check for best practices
    if (!checkBestPractices(pattern, rightHandSide)) {
        std::cerr << "Best practices check failed" << std::endl;
        return false;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::checkForPotentialIssues(DestructuringPattern* pattern, Expression* rightHandSide) {
    std::cout << "Checking for potential issues" << std::endl;
    
    // Check for null/undefined right-hand side
    if (isPotentiallyNull(rightHandSide)) {
        std::cout << "Warning: Right-hand side might be null or undefined" << std::endl;
    }
    
    // Check for empty destructuring patterns
    if (isEmptyDestructuringPattern(pattern)) {
        std::cout << "Warning: Destructuring pattern is empty" << std::endl;
    }
    
    // Check for unused destructured variables
    if (hasUnusedDestructuredVariables(pattern)) {
        std::cout << "Warning: Some destructured variables might be unused" << std::endl;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::checkPerformanceImplications(DestructuringPattern* pattern, Expression* rightHandSide) {
    std::cout << "Checking performance implications" << std::endl;
    
    // Check for expensive right-hand side expressions
    if (isExpensiveExpression(rightHandSide)) {
        std::cout << "Warning: Right-hand side expression might be expensive" << std::endl;
    }
    
    // Check for large destructuring patterns
    if (isLargeDestructuringPattern(pattern)) {
        std::cout << "Warning: Destructuring pattern is large" << std::endl;
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::checkBestPractices(DestructuringPattern* pattern, Expression* rightHandSide) {
    std::cout << "Checking best practices" << std::endl;
    
    // Check for consistent naming
    if (!hasConsistentNaming(pattern)) {
        std::cout << "Warning: Inconsistent naming in destructuring pattern" << std::endl;
    }
    
    // Check for appropriate use of rest elements
    if (!hasAppropriateRestElements(pattern)) {
        std::cout << "Warning: Rest elements might not be appropriate" << std::endl;
    }
    
    return true;
}

// =============================================================================
// HELPER METHODS
// =============================================================================

bool DestructuringSemanticAnalyzer::isValidIdentifierName(const std::string& name) {
    if (name.empty()) return false;
    
    // Check first character
    if (!std::isalpha(name[0]) && name[0] != '_' && name[0] != '$') {
        return false;
    }
    
    // Check remaining characters
    for (size_t i = 1; i < name.length(); ++i) {
        char c = name[i];
        if (!std::isalnum(c) && c != '_' && c != '$') {
            return false;
        }
    }
    
    return true;
}

bool DestructuringSemanticAnalyzer::isReservedKeyword(const std::string& name) {
    static const std::vector<std::string> reservedKeywords = {
        "break", "case", "catch", "class", "const", "continue", "debugger", "default",
        "delete", "do", "else", "enum", "export", "extends", "finally", "for", "function",
        "if", "import", "in", "instanceof", "let", "new", "return", "super", "switch",
        "this", "throw", "try", "typeof", "var", "void", "while", "with", "yield",
        "async", "await", "static", "private", "protected", "public", "readonly",
        "abstract", "interface", "namespace", "module", "declare", "type", "as"
    };
    
    return std::find(reservedKeywords.begin(), reservedKeywords.end(), name) != reservedKeywords.end();
}

bool DestructuringSemanticAnalyzer::addIdentifierToSymbolTable(Identifier* identifier) {
    if (!identifier) return false;
    
    auto symbolTable = analyzer_->getSymbolTable();
    
    // Create symbol for the identifier
    auto symbol = new Symbol(identifier->getName(), SymbolType::Variable);
    symbol->setNode(identifier);
    
    // Add to symbol table
    return symbolTable->addSymbol(symbol);
}

bool DestructuringSemanticAnalyzer::isExpressionIterable(Expression* expr) {
    if (!expr) return false;
    
    // This is a simplified check - in a real implementation, this would check
    // if the expression type implements the iterable protocol
    
    return true; // Assume iterable for now
}

bool DestructuringSemanticAnalyzer::isExpressionObjectLike(Expression* expr) {
    if (!expr) return false;
    
    // This is a simplified check - in a real implementation, this would check
    // if the expression type is object-like
    
    return true; // Assume object-like for now
}

bool DestructuringSemanticAnalyzer::isValidPropertyKeyLiteral(Literal* literal) {
    if (!literal) return false;
    
    // Check if literal type is valid for property key
    switch (literal->getValueType()) {
        case LiteralType::String:
        case LiteralType::Number:
            return true;
        default:
            return false;
    }
}

bool DestructuringSemanticAnalyzer::isPotentiallyNull(Expression* expr) {
    if (!expr) return false;
    
    // This is a simplified check - in a real implementation, this would analyze
    // the expression to determine if it might be null or undefined
    
    return false; // Assume not null for now
}

bool DestructuringSemanticAnalyzer::isEmptyDestructuringPattern(DestructuringPattern* pattern) {
    if (!pattern) return true;
    
    return pattern->getPatternElements().empty();
}

bool DestructuringSemanticAnalyzer::hasUnusedDestructuredVariables(DestructuringPattern* pattern) {
    if (!pattern) return false;
    
    // This is a simplified check - in a real implementation, this would analyze
    // the usage of destructured variables in the surrounding code
    
    return false; // Assume all variables are used for now
}

bool DestructuringSemanticAnalyzer::isExpensiveExpression(Expression* expr) {
    if (!expr) return false;
    
    // This is a simplified check - in a real implementation, this would analyze
    // the expression to determine if it might be expensive to evaluate
    
    return false; // Assume not expensive for now
}

bool DestructuringSemanticAnalyzer::isLargeDestructuringPattern(DestructuringPattern* pattern) {
    if (!pattern) return false;
    
    // Check if pattern has many elements
    return pattern->getPatternElements().size() > 10;
}

bool DestructuringSemanticAnalyzer::hasConsistentNaming(DestructuringPattern* pattern) {
    if (!pattern) return true;
    
    // This is a simplified check - in a real implementation, this would analyze
    // the naming conventions used in the destructuring pattern
    
    return true; // Assume consistent for now
}

bool DestructuringSemanticAnalyzer::hasAppropriateRestElements(DestructuringPattern* pattern) {
    if (!pattern) return true;
    
    // This is a simplified check - in a real implementation, this would analyze
    // whether the use of rest elements is appropriate
    
    return true; // Assume appropriate for now
}

} // namespace semantic
} // namespace tsc
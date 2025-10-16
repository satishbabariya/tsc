#pragma once

namespace tsc {

// Comprehensive AST Node Type enumeration
// This enum defines all possible AST node types in the TSC compiler
// Each AST node class should have a corresponding entry here
enum class ASTNodeType {
    // Base types
    ASTNode,
    Expression,
    Statement,
    Declaration,
    
    // Literals
    NumericLiteral,
    StringLiteral,
    BooleanLiteral,
    NullLiteral,
    TemplateLiteral,
    
    // Identifiers and references
    Identifier,
    ThisExpression,
    
    // Binary expressions
    BinaryExpression,
    UnaryExpression,
    AssignmentExpression,
    ArrayAssignmentExpression,  // New for array assignments
    
    // Member access
    MemberExpression,
    CallExpression,
    
    // Array and object literals
    ArrayExpression,
    ObjectLiteral,
    ObjectProperty,
    
    // Control flow statements
    IfStatement,
    WhileStatement,
    DoWhileStatement,
    ForStatement,
    ForOfStatement,
    ForInStatement,
    SwitchStatement,
    CaseClause,
    DefaultClause,
    
    // Function declarations
    FunctionDeclaration,
    ArrowFunctionExpression,
    MethodDeclaration,
    ConstructorDeclaration,
    Parameter,
    
    // Variable declarations
    VariableDeclaration,
    VariableDeclarator,
    
    // Block and scope
    BlockStatement,
    ExpressionStatement,
    
    // Return and control
    ReturnStatement,
    BreakStatement,
    ContinueStatement,
    
    // Error handling
    TryStatement,
    CatchClause,
    FinallyClause,
    ThrowStatement,
    PanicStatement,
    AbortStatement,
    ErrorPropagationExpression,
    
    // Destructuring patterns
    DestructuringPattern,
    ArrayDestructuringPattern,
    ObjectDestructuringPattern,
    DestructuringProperty,
    RestElement,
    AssignmentPattern,
    
    // Module system
    ImportDeclaration,
    ExportDeclaration,
    ImportClause,
    ExportClause,
    ImportSpec,
    ExportSpec,
    Module,
    
    // Type system (if applicable)
    TypeAnnotation,
    TypeParameter,
    GenericDeclaration,
    
    // Unknown/Error
    Unknown
};

// Helper function to convert ASTNodeType to string for debugging
const char* astNodeTypeToString(ASTNodeType type);

// Helper function to check if an ASTNodeType is an expression
bool isExpressionType(ASTNodeType type);

// Helper function to check if an ASTNodeType is a statement
bool isStatementType(ASTNodeType type);

// Helper function to check if an ASTNodeType is a declaration
bool isDeclarationType(ASTNodeType type);

} // namespace tsc
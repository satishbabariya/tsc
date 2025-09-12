#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"
#include "tsc/utils/EnhancedDiagnosticEngine.h"
#include "tsc/utils/ASTAllocator.h"
#include <vector>

namespace tsc {

class DiagnosticEngine;
class TokenStream;
class TypeSystem;

// Parsing context for better disambiguation
enum class ParsingContext {
    Expression,    // Normal expression context
    Type,         // Type annotation context  
    Template,     // Template literal context
};

// Parser for TypeScript syntax using recursive descent parsing
class Parser {
public:
    explicit Parser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
    explicit Parser(utils::EnhancedDiagnosticEngine& enhancedDiagnostics, const TypeSystem& typeSystem);
    ~Parser();
    
    // Main parsing interface
    unique_ptr<Module> parse(const std::vector<Token>& tokens, const String& filename);
    unique_ptr<Module> parse(unique_ptr<TokenStream> tokenStream, const String& filename);
    
    // Individual parsing methods for testing
    unique_ptr<Expression> parseExpression();
    unique_ptr<Statement> parseStatement();
    unique_ptr<Declaration> parseDeclaration();
    unique_ptr<Type> parseType();

private:
    DiagnosticEngine& diagnostics_;
    utils::EnhancedDiagnosticEngine* enhancedDiagnostics_;
    const TypeSystem& typeSystem_;
    unique_ptr<TokenStream> tokens_;
    String filename_;
    
    // Memory management
    utils::ASTAllocator astAllocator_;
    
    // Context management
    ParsingContext currentContext_ = ParsingContext::Expression;
    
    // Lookahead cache system
    struct LookaheadCache {
        std::vector<Token> tokens_;
        size_t currentIndex_ = 0;
    };
    mutable LookaheadCache lookaheadCache_;
    
    // Core parsing methods
    unique_ptr<Module> parseModule();
    std::vector<unique_ptr<Statement>> parseStatementList();
    
    // Statements
    unique_ptr<Statement> parseVariableStatement();
    unique_ptr<Statement> parseFunctionDeclaration();
    unique_ptr<Statement> parseClassDeclaration();
    unique_ptr<Statement> parseInterfaceDeclaration();
    unique_ptr<Statement> parseEnumDeclaration();
    unique_ptr<Statement> parseTypeAliasDeclaration();
    unique_ptr<Statement> parseIfStatement();
    unique_ptr<Statement> parseWhileStatement();
    unique_ptr<Statement> parseDoWhileStatement();
    unique_ptr<Statement> parseForStatement();
    unique_ptr<Statement> parseForOfStatement();
    unique_ptr<Statement> parseSwitchStatement();
    unique_ptr<CaseClause> parseCaseClause();
    unique_ptr<Statement> parseBlockStatement();
    unique_ptr<Statement> parseExpressionStatement();
    unique_ptr<Statement> parseReturnStatement();
    unique_ptr<Statement> parseBreakStatement();
    unique_ptr<Statement> parseContinueStatement();
    unique_ptr<Statement> parseTryStatement();
    unique_ptr<Statement> parseThrowStatement();
    
    // Expressions (with precedence climbing)
    unique_ptr<Expression> parseAssignmentExpression();
    unique_ptr<Expression> parseConditionalExpression();
    unique_ptr<Expression> parseBinaryExpression(int minPrecedence = 0);
    unique_ptr<Expression> parseUnaryExpression();
    unique_ptr<Expression> parsePostfixExpression();
    unique_ptr<Expression> parsePrimaryExpression();
    unique_ptr<Expression> parseTemplateLiteral();
    unique_ptr<Expression> parseCallExpression(unique_ptr<Expression> callee);
    unique_ptr<Expression> parseMemberExpression(unique_ptr<Expression> object);
    
    // Literals
    unique_ptr<Expression> parseNumericLiteral();
    unique_ptr<Expression> parseStringLiteral();
    unique_ptr<Expression> parseBooleanLiteral();
    unique_ptr<Expression> parseNullLiteral();
    unique_ptr<Expression> parseArrayLiteral();
    unique_ptr<Expression> parseObjectLiteral();
    unique_ptr<Expression> parseNewExpression();
    unique_ptr<Expression> parseIdentifier();
    
    // Arrow functions
    unique_ptr<Expression> parseArrowFunction();
    bool looksLikeArrowFunction();
    
    // Function expressions
    unique_ptr<Expression> parseFunctionExpression();
    
    // TypeScript-specific
    shared_ptr<Type> parseTypeAnnotation();
    shared_ptr<Type> parseUnionType();
    shared_ptr<Type> parsePrimaryType();
    shared_ptr<Type> parseTupleType();
    shared_ptr<Type> parseObjectType();
    
    // Function and class parsing
    std::vector<FunctionDeclaration::Parameter> parseParameterList();
    std::vector<MethodDeclaration::Parameter> parseMethodParameterList();
    FunctionDeclaration::Parameter parseParameter();
    unique_ptr<BlockStatement> parseFunctionBody();
    
    // Generic type parsing
    std::vector<unique_ptr<TypeParameter>> parseTypeParameterList();
    unique_ptr<TypeParameter> parseTypeParameter();
    
    // Generic type arguments
    std::vector<shared_ptr<Type>> parseTypeArgumentList();
    
    // Utility methods
    Token peek() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const String& errorMessage);
    bool isAtEnd() const;
    
    // Error handling and recovery
    void reportError(const String& message, const SourceLocation& location = {}, 
                    const String& context = "", const String& suggestion = "");
    void reportWarning(const String& message, const SourceLocation& location = {}, 
                      const String& context = "", const String& suggestion = "");
    void reportInfo(const String& message, const SourceLocation& location = {}, 
                   const String& context = "");
    void synchronize();
    void skipToStatementBoundary();
    void skipToDeclarationBoundary();
    void skipUntil(TokenType type);
    void skipUntil(std::initializer_list<TokenType> types);
    
    // Operator precedence
    int getBinaryOperatorPrecedence(TokenType type) const;
    BinaryExpression::Operator tokenToBinaryOperator(TokenType type) const;
    UnaryExpression::Operator tokenToUnaryOperator(TokenType type) const;
    AssignmentExpression::Operator tokenToAssignmentOperator(TokenType type) const;
    
    // Lookahead helpers
    bool isTypeArgumentList() const;
    Token peekAhead(size_t offset) const;
    bool hasAhead(size_t offset) const;
    bool analyzeTypeArgumentPattern() const;
    
    // Type checking helpers
    bool isTypeToken(TokenType type) const;
    bool isStatementStart(TokenType type) const;
    bool isDeclarationStart(TokenType type) const;
    bool isExpressionStart(TokenType type) const;
    bool isAssignmentOperator(TokenType type) const;
    
    // Context management
    void enterScope();
    void exitScope();
    void setContext(ParsingContext context);
    ParsingContext getCurrentContext() const;
    
    // Current parsing context
    SourceLocation getCurrentLocation() const;
};

// Factory functions for creating parser with token stream
unique_ptr<Parser> createParser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
unique_ptr<Parser> createEnhancedParser(utils::EnhancedDiagnosticEngine& diagnostics, const TypeSystem& typeSystem);

} // namespace tsc
#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {
namespace parser {

// Forward declarations
class ParserCore;

// Statement parsing engine - handles all statement types
class StatementParser {
public:
    explicit StatementParser(ParserCore& parser);
    
    // Main statement parsing interface
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseBlockStatement();
    
    // Control flow statements
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseWhileStatement();
    std::unique_ptr<Statement> parseDoWhileStatement();
    std::unique_ptr<Statement> parseForStatement();
    std::unique_ptr<Statement> parseForOfStatement();
    std::unique_ptr<Statement> parseSwitchStatement();
    std::unique_ptr<Statement> parseBreakStatement();
    std::unique_ptr<Statement> parseContinueStatement();
    
    // Exception handling
    std::unique_ptr<Statement> parseTryStatement();
    std::unique_ptr<Statement> parseThrowStatement();
    
    // Other statements
    std::unique_ptr<Statement> parseReturnStatement();
    std::unique_ptr<Statement> parseExpressionStatement();
    std::unique_ptr<Statement> parseVariableStatement();
    
    // Switch-specific parsing
    std::unique_ptr<CaseClause> parseCaseClause();
    std::unique_ptr<Statement> parseDefaultClause();

private:
    ParserCore& parser_;
    
    // Helper methods
    std::vector<std::unique_ptr<Statement>> parseStatementList();
    std::vector<std::unique_ptr<CaseClause>> parseCaseClauses();
    std::unique_ptr<Expression> parseForInitializer();
    std::unique_ptr<Expression> parseForCondition();
    std::unique_ptr<Expression> parseForIncrement();
    std::unique_ptr<VariableDeclaration> parseForVariableDeclaration();
};

} // namespace parser
} // namespace tsc
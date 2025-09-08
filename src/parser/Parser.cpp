#include "tsc/parser/Parser.h"
#include "tsc/parser/VectorTokenStream.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/semantic/TypeSystem.h"
#include <unordered_map>

namespace tsc {

// Operator precedence table (higher number = higher precedence)
static const std::unordered_map<TokenType, int> operatorPrecedence = {
    {TokenType::Equal, 1},
    {TokenType::PlusEqual, 1},
    {TokenType::MinusEqual, 1},
    {TokenType::PipePipe, 3},
    {TokenType::AmpersandAmpersand, 4},
    {TokenType::EqualEqual, 8},
    {TokenType::NotEqual, 8},
    {TokenType::Less, 9},
    {TokenType::Greater, 9},
    {TokenType::LessEqual, 9},
    {TokenType::GreaterEqual, 9},
    {TokenType::Plus, 11},
    {TokenType::Minus, 11},
    {TokenType::Star, 12},
    {TokenType::Slash, 12},
};

Parser::Parser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem) : diagnostics_(diagnostics), typeSystem_(typeSystem) {}

Parser::~Parser() = default;

unique_ptr<Module> Parser::parse(const std::vector<Token>& tokens, const String& filename) {
    // Create a token stream from the vector
    auto tokenStream = make_unique<VectorTokenStream>(tokens);
    return parse(std::move(tokenStream), filename);
}

unique_ptr<Module> Parser::parse(unique_ptr<TokenStream> tokenStream, const String& filename) {
    tokens_ = std::move(tokenStream);
    filename_ = filename;
    
    try {
        return parseModule();
    } catch (const CompilerError& e) {
        reportError(e.what(), e.getLocation());
        return nullptr;
    }
}

unique_ptr<Module> Parser::parseModule() {
    std::vector<unique_ptr<Statement>> statements;
    
    while (!isAtEnd()) {
        try {
            if (auto stmt = parseStatement()) {
                statements.push_back(std::move(stmt));
            }
        } catch (const CompilerError&) {
            synchronize();
        }
    }
    
    return make_unique<Module>(filename_, std::move(statements));
}

unique_ptr<Statement> Parser::parseStatement() {
    // Handle variable declarations
    if (match({TokenType::Var, TokenType::Let, TokenType::Const})) {
        return parseVariableStatement();
    }
    
    // Handle function declarations
    if (check(TokenType::Function)) {
        return parseFunctionDeclaration();
    }
    
    // Handle class declarations
    if (check(TokenType::Class)) {
        return parseClassDeclaration();
    }
    
    // Handle block statements
    if (match(TokenType::LeftBrace)) {
        return parseBlockStatement();
    }
    
    // Handle return statements
    if (match(TokenType::Return)) {
        return parseReturnStatement();
    }
    
    // Handle if statements
    if (match(TokenType::If)) {
        return parseIfStatement();
    }
    
    // Handle while statements
    if (match(TokenType::While)) {
        return parseWhileStatement();
    }
    
    // Handle do-while statements
    if (match(TokenType::Do)) {
        return parseDoWhileStatement();
    }
    
    // Handle for statements
    if (match(TokenType::For)) {
        return parseForStatement();
    }
    
    // Handle switch statements
    if (match(TokenType::Switch)) {
        return parseSwitchStatement();
    }
    
    // Handle break statements
    if (match(TokenType::Break)) {
        return parseBreakStatement();
    }
    
    // Handle continue statements
    if (match(TokenType::Continue)) {
        return parseContinueStatement();
    }
    
    // Handle try statements
    if (match(TokenType::Try)) {
        return parseTryStatement();
    }
    
    // Handle throw statements
    if (match(TokenType::Throw)) {
        return parseThrowStatement();
    }
    
    // Default to expression statement
    return parseExpressionStatement();
}

unique_ptr<Statement> Parser::parseVariableStatement() {
    // Parse identifier
    Token nameToken = consume(TokenType::Identifier, "Expected variable name");
    String name = nameToken.getStringValue();
    
    // Optional type annotation
    shared_ptr<Type> typeAnnotation = nullptr;
    if (match(TokenType::Colon)) {
        typeAnnotation = parseTypeAnnotation();
    }
    
    // Optional initializer
    unique_ptr<Expression> initializer = nullptr;
    if (match(TokenType::Equal)) {
        initializer = parseExpression();
    }
    
    consume(TokenType::Semicolon, "Expected ';' after variable declaration");
    
    return make_unique<VariableDeclaration>(
        VariableDeclaration::Kind::Let, name, std::move(initializer), 
        typeAnnotation, nameToken.getLocation()
    );
}

unique_ptr<Statement> Parser::parseFunctionDeclaration() {
    consume(TokenType::Function, "Expected 'function'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected function name");
    String name = nameToken.getStringValue();
    
    consume(TokenType::LeftParen, "Expected '(' after function name");
    auto parameters = parseParameterList();
    consume(TokenType::RightParen, "Expected ')' after parameters");
    
    // Optional return type
    shared_ptr<Type> returnType = nullptr;
    if (match(TokenType::Colon)) {
        returnType = parseTypeAnnotation();
    }
    
    auto body = parseFunctionBody();
    
    return make_unique<FunctionDeclaration>(
        name, std::move(parameters), returnType, std::move(body), 
        nameToken.getLocation(), false, false
    );
}

unique_ptr<Statement> Parser::parseClassDeclaration() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Class, "Expected 'class'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected class name");
    String name = nameToken.getStringValue();
    
    // Optional base class (extends clause)
    shared_ptr<Type> baseClass = nullptr;
    if (match(TokenType::Extends)) {
        // For now, just parse as identifier - full type resolution happens in semantic analysis
        Token baseToken = consume(TokenType::Identifier, "Expected base class name");
        // Create a placeholder type - will be resolved in semantic analysis
        baseClass = typeSystem_.createClassType(baseToken.getStringValue());
    }
    
    // Optional interfaces (implements clause)
    std::vector<shared_ptr<Type>> interfaces;
    if (match(TokenType::Implements)) {
        do {
            Token interfaceToken = consume(TokenType::Identifier, "Expected interface name");
            // Create placeholder type - will be resolved in semantic analysis
            interfaces.push_back(typeSystem_.createClassType(interfaceToken.getStringValue()));
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::LeftBrace, "Expected '{' after class header");
    
    // Parse class body
    std::vector<unique_ptr<PropertyDeclaration>> properties;
    std::vector<unique_ptr<MethodDeclaration>> methods;
    unique_ptr<MethodDeclaration> constructor = nullptr;
    
    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        // Parse visibility modifiers
        bool isPrivate = false;
        bool isProtected = false;
        bool isStatic = false;
        bool isReadonly = false;
        bool isAbstract = false;
        
        while (check(TokenType::Private) || check(TokenType::Protected) || check(TokenType::Public) || 
               check(TokenType::Static) || check(TokenType::Readonly) || check(TokenType::Abstract)) {
            if (match(TokenType::Private)) isPrivate = true;
            else if (match(TokenType::Protected)) isProtected = true;
            else if (match(TokenType::Public)) {} // Default, do nothing
            else if (match(TokenType::Static)) isStatic = true;
            else if (match(TokenType::Readonly)) isReadonly = true;
            else if (match(TokenType::Abstract)) isAbstract = true;
        }
        
        if (check(TokenType::Constructor)) {
            // Parse constructor
            consume(TokenType::Constructor, "Expected 'constructor'");
            consume(TokenType::LeftParen, "Expected '(' after constructor");
            auto parameters = parseMethodParameterList();
            consume(TokenType::RightParen, "Expected ')' after constructor parameters");
            
            auto body = parseFunctionBody();
            
            constructor = make_unique<MethodDeclaration>(
                "constructor", std::move(parameters), typeSystem_.getVoidType(),
                std::move(body), getCurrentLocation(), isStatic, isPrivate, isProtected, isAbstract
            );
        } else if (check(TokenType::Identifier)) {
            Token memberToken = advance();
            String memberName = memberToken.getStringValue();
            
            if (check(TokenType::LeftParen)) {
                // Method declaration
                consume(TokenType::LeftParen, "Expected '(' after method name");
                auto parameters = parseMethodParameterList();
                consume(TokenType::RightParen, "Expected ')' after method parameters");
                
                // Optional return type
                shared_ptr<Type> returnType = typeSystem_.getVoidType();
                if (match(TokenType::Colon)) {
                    returnType = parseTypeAnnotation();
                }
                
                auto body = parseFunctionBody();
                
                methods.push_back(make_unique<MethodDeclaration>(
                    memberName, std::move(parameters), returnType, std::move(body),
                    memberToken.getLocation(), isStatic, isPrivate, isProtected, isAbstract
                ));
            } else {
                // Property declaration
                shared_ptr<Type> propertyType = nullptr;
                if (match(TokenType::Colon)) {
                    propertyType = parseTypeAnnotation();
                }
                
                unique_ptr<Expression> initializer = nullptr;
                if (match(TokenType::Equal)) {
                    initializer = parseExpression();
                }
                
                consume(TokenType::Semicolon, "Expected ';' after property declaration");
                
                properties.push_back(make_unique<PropertyDeclaration>(
                    memberName, propertyType, std::move(initializer), memberToken.getLocation(),
                    isStatic, isReadonly, isPrivate, isProtected
                ));
            }
        } else {
            reportError("Expected class member", getCurrentLocation());
            synchronize();
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after class body");
    
    return make_unique<ClassDeclaration>(
        name, baseClass, std::move(interfaces), std::move(properties), 
        std::move(methods), std::move(constructor), location
    );
}

unique_ptr<Statement> Parser::parseBlockStatement() {
    std::vector<unique_ptr<Statement>> statements;
    
    while (!isAtEnd() && !check(TokenType::RightBrace)) {
        if (auto stmt = parseStatement()) {
            statements.push_back(std::move(stmt));
        } else {
            synchronize();
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after block");
    return make_unique<BlockStatement>(std::move(statements), getCurrentLocation());
}

unique_ptr<Statement> Parser::parseReturnStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Check if there's a return value
    unique_ptr<Expression> value = nullptr;
    if (!check(TokenType::Semicolon) && !isAtEnd()) {
        value = parseExpression();
    }
    
    consume(TokenType::Semicolon, "Expected ';' after return statement");
    
    if (value) {
        return make_unique<ReturnStatement>(std::move(value), location);
    } else {
        return make_unique<ReturnStatement>(location);
    }
}

unique_ptr<Statement> Parser::parseIfStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse condition
    consume(TokenType::LeftParen, "Expected '(' after 'if'");
    auto condition = parseExpression();
    consume(TokenType::RightParen, "Expected ')' after if condition");
    
    // Parse then statement
    auto thenStmt = parseStatement();
    
    // Optional else clause
    unique_ptr<Statement> elseStmt = nullptr;
    if (match(TokenType::Else)) {
        elseStmt = parseStatement();
    }
    
    if (elseStmt) {
        return make_unique<IfStatement>(std::move(condition), std::move(thenStmt), 
                                       std::move(elseStmt), location);
    } else {
        return make_unique<IfStatement>(std::move(condition), std::move(thenStmt), location);
    }
}

unique_ptr<Statement> Parser::parseWhileStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse condition
    consume(TokenType::LeftParen, "Expected '(' after 'while'");
    auto condition = parseExpression();
    consume(TokenType::RightParen, "Expected ')' after while condition");
    
    // Parse body
    auto body = parseStatement();
    
    return make_unique<WhileStatement>(std::move(condition), std::move(body), location);
}

unique_ptr<Statement> Parser::parseDoWhileStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse body
    auto body = parseStatement();
    
    // Expect 'while'
    consume(TokenType::While, "Expected 'while' after do body");
    
    // Parse condition
    consume(TokenType::LeftParen, "Expected '(' after 'while'");
    auto condition = parseExpression();
    consume(TokenType::RightParen, "Expected ')' after while condition");
    
    // Expect semicolon
    consume(TokenType::Semicolon, "Expected ';' after do-while statement");
    
    return make_unique<DoWhileStatement>(std::move(body), std::move(condition), location);
}

unique_ptr<Statement> Parser::parseForStatement() {
    SourceLocation location = getCurrentLocation();
    
    consume(TokenType::LeftParen, "Expected '(' after 'for'");
    
    // Parse init (can be variable declaration or expression or empty)
    unique_ptr<Statement> init = nullptr;
    if (!check(TokenType::Semicolon)) {
        if (match({TokenType::Var, TokenType::Let, TokenType::Const})) {
            // Parse variable declaration for init
            init = parseVariableStatement();
        } else {
            // Parse expression and wrap in expression statement
            auto expr = parseExpression();
            consume(TokenType::Semicolon, "Expected ';' after for init");
            init = make_unique<ExpressionStatement>(std::move(expr), getCurrentLocation());
        }
    } else {
        consume(TokenType::Semicolon, "Expected ';' after for init");
    }
    
    // Parse condition (optional)
    unique_ptr<Expression> condition = nullptr;
    if (!check(TokenType::Semicolon)) {
        condition = parseExpression();
    }
    consume(TokenType::Semicolon, "Expected ';' after for condition");
    
    // Parse increment (optional)
    unique_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RightParen)) {
        increment = parseExpression();
    }
    consume(TokenType::RightParen, "Expected ')' after for increment");
    
    // Parse body
    auto body = parseStatement();
    
    return make_unique<ForStatement>(std::move(init), std::move(condition), 
                                   std::move(increment), std::move(body), location);
}

unique_ptr<Statement> Parser::parseSwitchStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse discriminant
    consume(TokenType::LeftParen, "Expected '(' after 'switch'");
    auto discriminant = parseExpression();
    consume(TokenType::RightParen, "Expected ')' after switch discriminant");
    
    // Parse case block
    consume(TokenType::LeftBrace, "Expected '{' after switch discriminant");
    
    std::vector<unique_ptr<CaseClause>> cases;
    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        auto caseClause = parseCaseClause();
        if (caseClause) {
            cases.push_back(std::move(caseClause));
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after switch cases");
    
    return make_unique<SwitchStatement>(std::move(discriminant), std::move(cases), location);
}

unique_ptr<CaseClause> Parser::parseCaseClause() {
    SourceLocation location = getCurrentLocation();
    
    unique_ptr<Expression> test = nullptr;
    
    if (match(TokenType::Case)) {
        // Parse case expression
        test = parseExpression();
        consume(TokenType::Colon, "Expected ':' after case expression");
    } else if (match(TokenType::Default)) {
        // Default case (test remains null)
        consume(TokenType::Colon, "Expected ':' after 'default'");
    } else {
        reportError("Expected 'case' or 'default'", getCurrentLocation());
        return nullptr;
    }
    
    // Parse statements until next case/default or end of block
    std::vector<unique_ptr<Statement>> statements;
    while (!check(TokenType::Case) && !check(TokenType::Default) && 
           !check(TokenType::RightBrace) && !isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
    }
    
    if (test) {
        return make_unique<CaseClause>(std::move(test), std::move(statements), location);
    } else {
        return make_unique<CaseClause>(std::move(statements), location);
    }
}

unique_ptr<Statement> Parser::parseBreakStatement() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Semicolon, "Expected ';' after 'break'");
    return make_unique<BreakStatement>(location);
}

unique_ptr<Statement> Parser::parseContinueStatement() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Semicolon, "Expected ';' after 'continue'");
    return make_unique<ContinueStatement>(location);
}

unique_ptr<Statement> Parser::parseTryStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse try block
    consume(TokenType::LeftBrace, "Expected '{' after 'try'");
    auto tryBlock = std::unique_ptr<BlockStatement>(static_cast<BlockStatement*>(parseBlockStatement().release()));
    
    // Parse optional catch clause
    unique_ptr<CatchClause> catchClause = nullptr;
    if (match(TokenType::Catch)) {
        SourceLocation catchLocation = getCurrentLocation();
        
        // Check for optional parameter
        String parameter;
        bool hasParameter = false;
        if (match(TokenType::LeftParen)) {
            if (check(TokenType::Identifier)) {
                Token paramToken = advance();
                parameter = paramToken.getStringValue();
                hasParameter = true;
            }
            consume(TokenType::RightParen, "Expected ')' after catch parameter");
        }
        
        // Parse catch body
        consume(TokenType::LeftBrace, "Expected '{' after catch clause");
        auto catchBody = std::unique_ptr<BlockStatement>(static_cast<BlockStatement*>(parseBlockStatement().release()));
        
        if (hasParameter) {
            catchClause = make_unique<CatchClause>(parameter, std::move(catchBody), catchLocation);
        } else {
            catchClause = make_unique<CatchClause>(std::move(catchBody), catchLocation);
        }
    }
    
    // Parse optional finally block
    unique_ptr<BlockStatement> finallyBlock = nullptr;
    if (match(TokenType::Finally)) {
        consume(TokenType::LeftBrace, "Expected '{' after 'finally'");
        finallyBlock = std::unique_ptr<BlockStatement>(static_cast<BlockStatement*>(parseBlockStatement().release()));
    }
    
    // Must have either catch or finally (or both)
    if (!catchClause && !finallyBlock) {
        reportError("Missing catch or finally after try", location);
        return nullptr;
    }
    
    return make_unique<TryStatement>(std::move(tryBlock), std::move(catchClause), 
                                   std::move(finallyBlock), location);
}

unique_ptr<Statement> Parser::parseThrowStatement() {
    SourceLocation location = getCurrentLocation();
    
    // Parse the expression to throw
    auto expression = parseExpression();
    if (!expression) {
        reportError("Expected expression after 'throw'", location);
        return nullptr;
    }
    
    consume(TokenType::Semicolon, "Expected ';' after throw statement");
    
    return make_unique<ThrowStatement>(std::move(expression), location);
}

unique_ptr<Statement> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    consume(TokenType::Semicolon, "Expected ';' after expression");
    
    return make_unique<ExpressionStatement>(std::move(expr), getCurrentLocation());
}

unique_ptr<Expression> Parser::parseExpression() {
    return parseBinaryExpression();
}

unique_ptr<Expression> Parser::parseBinaryExpression(int minPrecedence) {
    auto left = parseUnaryExpression();
    
    while (!isAtEnd()) {
        TokenType opType = peek().getType();
        int precedence = getBinaryOperatorPrecedence(opType);
        
        if (precedence < minPrecedence) {
            break;
        }
        
        Token opToken = advance();
        
        // Handle assignment operators specially
        if (opType == TokenType::Equal || opType == TokenType::PlusEqual || opType == TokenType::MinusEqual) {
            auto right = parseBinaryExpression(precedence);  // Right-associative
            AssignmentExpression::Operator assignOp = AssignmentExpression::Operator::Assign;
            if (opType == TokenType::PlusEqual) {
                assignOp = AssignmentExpression::Operator::AddAssign;
            } else if (opType == TokenType::MinusEqual) {
                assignOp = AssignmentExpression::Operator::SubtractAssign;
            }
            left = make_unique<AssignmentExpression>(
                std::move(left),
                assignOp,
                std::move(right),
                opToken.getLocation()
            );
        } else {
            auto right = parseBinaryExpression(precedence + 1);
            left = make_unique<BinaryExpression>(
                std::move(left),
                tokenToBinaryOperator(opType),
                std::move(right),
                opToken.getLocation()
            );
        }
    }
    
    return left;
}

unique_ptr<Expression> Parser::parseUnaryExpression() {
    if (check(TokenType::Plus) || check(TokenType::Minus) || 
        check(TokenType::Exclamation)) {
        
        Token opToken = advance();
        auto operand = parseUnaryExpression();
        
        return make_unique<UnaryExpression>(
            tokenToUnaryOperator(opToken.getType()),
            std::move(operand),
            opToken.getLocation(),
            true
        );
    }
    
    return parsePostfixExpression();
}

unique_ptr<Expression> Parser::parsePostfixExpression() {
    auto expr = parsePrimaryExpression();
    
    while (true) {
        if (match(TokenType::LeftBracket)) {
            // Parse array indexing: expr[index]
            auto index = parseExpression();
            consume(TokenType::RightBracket, "Expected ']' after array index");
            
            SourceLocation location = getCurrentLocation();
            expr = make_unique<IndexExpression>(
                std::move(expr),
                std::move(index),
                location
            );
        }
        else if (match(TokenType::LeftParen)) {
            // Parse function call: expr(args...)
            std::vector<unique_ptr<Expression>> arguments;
            
            // Parse arguments if not empty
            if (!check(TokenType::RightParen)) {
                do {
                    auto arg = parseExpression();
                    if (!arg) {
                        reportError("Expected expression in function call argument", getCurrentLocation());
                        return nullptr;
                    }
                    arguments.push_back(std::move(arg));
                } while (match(TokenType::Comma));
            }
            
            consume(TokenType::RightParen, "Expected ')' after function call arguments");
            
            SourceLocation location = getCurrentLocation();
            expr = make_unique<CallExpression>(
                std::move(expr),
                std::move(arguments),
                location
            );
        }
        else if (match(TokenType::Dot)) {
            // Parse property access: expr.property
            if (!check(TokenType::Identifier)) {
                reportError("Expected property name after '.'", getCurrentLocation());
                return nullptr;
            }
            
            Token propertyToken = advance();
            SourceLocation location = getCurrentLocation();
            expr = make_unique<PropertyAccess>(
                std::move(expr),
                propertyToken.getStringValue(),
                location
            );
        }
        else {
            break; // No more postfix operations
        }
    }
    
    return expr;
}

unique_ptr<Expression> Parser::parsePrimaryExpression() {
    if (check(TokenType::NumericLiteral)) {
        Token token = advance();
        return make_unique<NumericLiteral>(token.getNumericValue(), token.getLocation());
    }
    
    if (check(TokenType::StringLiteral)) {
        Token token = advance();
        return make_unique<StringLiteral>(token.getStringValue(), token.getLocation());
    }
    
    if (check(TokenType::True)) {
        advance();
        return make_unique<BooleanLiteral>(true, getCurrentLocation());
    }
    
    if (check(TokenType::False)) {
        advance();
        return make_unique<BooleanLiteral>(false, getCurrentLocation());
    }
    
    if (check(TokenType::Null)) {
        advance();
        return make_unique<NullLiteral>(getCurrentLocation());
    }
    
    if (check(TokenType::Identifier)) {
        Token token = advance();
        return make_unique<Identifier>(token.getStringValue(), token.getLocation());
    }
    
    if (check(TokenType::This)) {
        advance();
        return make_unique<ThisExpression>(getCurrentLocation());
    }
    
    if (check(TokenType::New)) {
        advance();
        return parseNewExpression();
    }
    
    if (match(TokenType::LeftBracket)) {
        return parseArrayLiteral();
    }
    
    if (match(TokenType::LeftBrace)) {
        return parseObjectLiteral();
    }
    
    if (match(TokenType::LeftParen)) {
        auto expr = parseExpression();
        consume(TokenType::RightParen, "Expected ')' after expression");
        return expr;
    }
    
    reportError("Unexpected token in expression", getCurrentLocation());
    return nullptr;
}

unique_ptr<Expression> Parser::parseArrayLiteral() {
    SourceLocation location = getCurrentLocation();
    std::vector<unique_ptr<Expression>> elements;
    
    // Handle empty array
    if (match(TokenType::RightBracket)) {
        return make_unique<ArrayLiteral>(std::move(elements), location);
    }
    
    // Parse elements
    do {
        if (check(TokenType::RightBracket)) {
            break; // Trailing comma case: [1, 2, ]
        }
        
        auto element = parseExpression();
        if (element) {
            elements.push_back(std::move(element));
        }
    } while (match(TokenType::Comma));
    
    consume(TokenType::RightBracket, "Expected ']' after array elements");
    
    return make_unique<ArrayLiteral>(std::move(elements), location);
}

unique_ptr<Expression> Parser::parseObjectLiteral() {
    SourceLocation location = getCurrentLocation();
    std::vector<ObjectLiteral::Property> properties;
    
    // Handle empty object
    if (match(TokenType::RightBrace)) {
        return make_unique<ObjectLiteral>(std::move(properties), location);
    }
    
    // Parse properties
    do {
        if (check(TokenType::RightBrace)) {
            break; // Trailing comma case: {a: 1, }
        }
        
        // Parse property key (for now, only identifiers and string literals)
        String key;
        if (check(TokenType::Identifier)) {
            Token keyToken = advance();
            key = keyToken.getStringValue();
        } else if (check(TokenType::StringLiteral)) {
            Token keyToken = advance();
            key = keyToken.getStringValue();
        } else {
            reportError("Expected property name", getCurrentLocation());
            return nullptr;
        }
        
        // Expect colon
        consume(TokenType::Colon, "Expected ':' after property name");
        
        // Parse property value
        auto value = parseExpression();
        if (!value) {
            reportError("Expected property value", getCurrentLocation());
            return nullptr;
        }
        
        properties.emplace_back(key, std::move(value), getCurrentLocation());
        
    } while (match(TokenType::Comma));
    
    consume(TokenType::RightBrace, "Expected '}' after object properties");
    
    return make_unique<ObjectLiteral>(std::move(properties), location);
}

unique_ptr<Expression> Parser::parseNewExpression() {
    SourceLocation location = getCurrentLocation();
    
    // Parse the constructor expression (e.g., "Person", "MyClass")
    auto constructor = parsePrimaryExpression();
    
    // Parse arguments if present
    std::vector<unique_ptr<Expression>> arguments;
    if (match(TokenType::LeftParen)) {
        // Parse arguments if not empty
        if (!check(TokenType::RightParen)) {
            do {
                auto arg = parseExpression();
                if (arg) {
                    arguments.push_back(std::move(arg));
                }
            } while (match(TokenType::Comma));
        }
        consume(TokenType::RightParen, "Expected ')' after constructor arguments");
    }
    
    return make_unique<NewExpression>(std::move(constructor), std::move(arguments), location);
}

// Utility methods
Token Parser::peek() const {
    return tokens_->peek();
}

Token Parser::advance() {
    return tokens_->advance();
}

bool Parser::check(TokenType type) const {
    return !isAtEnd() && peek().getType() == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const String& errorMessage) {
    if (check(type)) {
        return advance();
    }
    
    reportError(errorMessage, getCurrentLocation());
    throw CompilerError(errorMessage, getCurrentLocation());
}

bool Parser::isAtEnd() const {
    return tokens_->isAtEnd();
}

int Parser::getBinaryOperatorPrecedence(TokenType type) const {
    auto it = operatorPrecedence.find(type);
    return it != operatorPrecedence.end() ? it->second : -1;
}

BinaryExpression::Operator Parser::tokenToBinaryOperator(TokenType type) const {
    switch (type) {
        case TokenType::Plus: return BinaryExpression::Operator::Add;
        case TokenType::Minus: return BinaryExpression::Operator::Subtract;
        case TokenType::Star: return BinaryExpression::Operator::Multiply;
        case TokenType::Slash: return BinaryExpression::Operator::Divide;
        case TokenType::EqualEqual: return BinaryExpression::Operator::Equal;
        case TokenType::NotEqual: return BinaryExpression::Operator::NotEqual;
        case TokenType::Less: return BinaryExpression::Operator::Less;
        case TokenType::Greater: return BinaryExpression::Operator::Greater;
        case TokenType::LessEqual: return BinaryExpression::Operator::LessEqual;
        case TokenType::GreaterEqual: return BinaryExpression::Operator::GreaterEqual;
        case TokenType::AmpersandAmpersand: return BinaryExpression::Operator::LogicalAnd;
        case TokenType::PipePipe: return BinaryExpression::Operator::LogicalOr;
        default:
            return BinaryExpression::Operator::Add;
    }
}

UnaryExpression::Operator Parser::tokenToUnaryOperator(TokenType type) const {
    switch (type) {
        case TokenType::Plus: return UnaryExpression::Operator::Plus;
        case TokenType::Minus: return UnaryExpression::Operator::Minus;
        case TokenType::Exclamation: return UnaryExpression::Operator::LogicalNot;
        default:
            return UnaryExpression::Operator::Plus;
    }
}

void Parser::reportError(const String& message, const SourceLocation& location) {
    SourceLocation loc = location.isValid() ? location : getCurrentLocation();
    diagnostics_.error(message, loc);
}

void Parser::reportWarning(const String& message, const SourceLocation& location) {
    SourceLocation loc = location.isValid() ? location : getCurrentLocation();
    diagnostics_.warning(message, loc);
}

void Parser::synchronize() {
    tokens_->synchronize();
}

SourceLocation Parser::getCurrentLocation() const {
    return peek().getLocation();
}

// Parse type annotations like ": number", ": string", etc.
shared_ptr<Type> Parser::parseTypeAnnotation() {
    // We should already have consumed the ':' token before calling this
    
    
    // Handle TypeScript type keywords
    if (check(TokenType::Number)) {
        advance();
        return typeSystem_.getNumberType();
    } else if (check(TokenType::String)) {
        advance();
        return typeSystem_.getStringType();
    } else if (check(TokenType::Boolean)) {
        advance();
        return typeSystem_.getBooleanType();
    } else if (check(TokenType::Null)) {
        advance();
        return typeSystem_.getNullType();
    } else if (check(TokenType::Undefined)) {
        advance();
        return typeSystem_.getUndefinedType();
    } else if (check(TokenType::Void)) {
        advance();
        return typeSystem_.getVoidType();
    } else if (check(TokenType::Any)) {
        advance();
        return typeSystem_.getAnyType();
    } else if (check(TokenType::Unknown)) {
        advance();
        return typeSystem_.getUnknownType();
    } else if (check(TokenType::Never)) {
        advance();
        return typeSystem_.getNeverType();
    }
    
    // Handle regular identifiers (for custom types, interfaces, etc.)
    if (check(TokenType::Identifier)) {
        Token typeToken = advance();
        String typeName = typeToken.getStringValue();
        
        // Check for generic type syntax: TypeName<Type1, Type2, ...>
        if (check(TokenType::Less)) {
            advance(); // consume '<'
            
            std::vector<shared_ptr<Type>> typeArguments;
            
            // Parse type arguments
            do {
                auto typeArg = parseTypeAnnotation();
                if (!typeArg) {
                    reportError("Expected type in generic type argument", getCurrentLocation());
                    return typeSystem_.getErrorType();
                }
                typeArguments.push_back(typeArg);
            } while (match(TokenType::Comma));
            
            consume(TokenType::Greater, "Expected '>' after generic type arguments");
            
            // Handle built-in generic types
            if (typeName == "Array") {
                if (typeArguments.size() != 1) {
                    reportError("Array type requires exactly one type argument", typeToken.getLocation());
                    return typeSystem_.getErrorType();
                }
                return typeSystem_.createArrayType(typeArguments[0]);
            }
            
            // For other generic types, create a generic type
            // In a full implementation, we'd look up the base type in a symbol table
            auto baseType = typeSystem_.createTypeParameter(typeName);
            return typeSystem_.createGenericType(baseType, std::move(typeArguments));
        }
        
        // Handle built-in non-generic identifiers
        if (typeName == "Array") {
            // Array without type arguments defaults to Array<any>
            return typeSystem_.createArrayType(typeSystem_.getAnyType());
        }
        
        // For now, treat unknown identifiers as error types
        reportError("Unknown type: " + typeName, typeToken.getLocation());
        return typeSystem_.getErrorType();
    }
    
    // Handle array types like "number[]"
    if (check(TokenType::LeftBracket)) {
        // For now, just report error - array type syntax needs more complex parsing
        reportError("Array type syntax not yet implemented", getCurrentLocation());
        return typeSystem_.getErrorType();
    }
    
    reportError("Expected type name after ':'", getCurrentLocation());
    return typeSystem_.getErrorType();
}

std::vector<FunctionDeclaration::Parameter> Parser::parseParameterList() {
    std::vector<FunctionDeclaration::Parameter> parameters;
    
    if (!check(TokenType::RightParen)) {
        do {
            FunctionDeclaration::Parameter param;
            Token nameToken = consume(TokenType::Identifier, "Expected parameter name");
            param.name = nameToken.getStringValue();
            
            if (match(TokenType::Colon)) {
                param.type = parseTypeAnnotation();
            }
            
            parameters.push_back(std::move(param));
        } while (match(TokenType::Comma));
    }
    
    return parameters;
}

std::vector<MethodDeclaration::Parameter> Parser::parseMethodParameterList() {
    std::vector<MethodDeclaration::Parameter> parameters;
    
    if (!check(TokenType::RightParen)) {
        do {
            MethodDeclaration::Parameter param;
            Token nameToken = consume(TokenType::Identifier, "Expected parameter name");
            param.name = nameToken.getStringValue();
            
            if (match(TokenType::Colon)) {
                param.type = parseTypeAnnotation();
            }
            
            parameters.push_back(std::move(param));
        } while (match(TokenType::Comma));
    }
    
    return parameters;
}

unique_ptr<BlockStatement> Parser::parseFunctionBody() {
    consume(TokenType::LeftBrace, "Expected '{' before function body");
    
    std::vector<unique_ptr<Statement>> statements;
    
    while (!isAtEnd() && !check(TokenType::RightBrace)) {
        if (auto stmt = parseStatement()) {
            statements.push_back(std::move(stmt));
        } else {
            synchronize();
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after function body");
    
    return make_unique<BlockStatement>(std::move(statements), getCurrentLocation());
}

// Arrow function parsing
unique_ptr<Expression> Parser::parseArrowFunction() {
    SourceLocation location = getCurrentLocation();
    std::vector<ArrowFunction::Parameter> parameters;
    
    if (check(TokenType::Identifier)) {
        // Single parameter without parentheses: identifier => body
        Token param = advance();
        ArrowFunction::Parameter parameter;
        parameter.name = param.getStringValue();
        parameter.location = param.getLocation();
        parameters.push_back(std::move(parameter));
    } else if (match(TokenType::LeftParen)) {
        // Parameters in parentheses: (param1, param2) => body
        if (!check(TokenType::RightParen)) {
            do {
                if (check(TokenType::Identifier)) {
                    Token param = advance();
                    ArrowFunction::Parameter parameter;
                    parameter.name = param.getStringValue();
                    parameter.location = param.getLocation();
                    
                    // Optional type annotation
                    if (match(TokenType::Colon)) {
                        parameter.type = parseTypeAnnotation();
                    }
                    
                    parameters.push_back(std::move(parameter));
                } else {
                    reportError("Expected parameter name", getCurrentLocation());
                    break;
                }
            } while (match(TokenType::Comma));
        }
        
        consume(TokenType::RightParen, "Expected ')' after parameters");
    } else {
        reportError("Expected parameter or '(' in arrow function", getCurrentLocation());
        return nullptr;
    }
    
    consume(TokenType::Arrow, "Expected '=>' in arrow function");
    
    // Parse body - can be expression or block statement
    unique_ptr<Statement> body;
    if (check(TokenType::LeftBrace)) {
        body = parseBlockStatement();
    } else {
        // Expression body - wrap in return statement
        auto expr = parseExpression();
        if (expr) {
            auto returnStmt = make_unique<ReturnStatement>(std::move(expr), getCurrentLocation());
            std::vector<unique_ptr<Statement>> statements;
            statements.push_back(std::move(returnStmt));
            body = make_unique<BlockStatement>(std::move(statements), getCurrentLocation());
        }
    }
    
    if (!body) {
        reportError("Expected arrow function body", getCurrentLocation());
        return nullptr;
    }
    
    return make_unique<ArrowFunction>(std::move(parameters), std::move(body), nullptr, location);
}

bool Parser::looksLikeArrowFunction() {
    // For now, return false to disable complex lookahead
    // TODO: Implement proper lookahead when TokenStream supports it
    return false;
}

// Factory function
unique_ptr<Parser> createParser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem) {
    return make_unique<Parser>(diagnostics, typeSystem);
}

} // namespace tsc
#include "tsc/parser/Parser.h"
#include "tsc/parser/VectorTokenStream.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedDiagnosticEngine.h"
#include "tsc/utils/ASTAllocator.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/AST.h"
#include <unordered_map>
#include <iostream>

namespace tsc {

// Operator precedence table (higher number = higher precedence)
// Note: Assignment operators (Equal, PlusEqual, MinusEqual) are NOT included here
// because they are handled by parseAssignmentExpression(), not parseBinaryExpression()
static const std::unordered_map<TokenType, int> operatorPrecedence = {
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
    {TokenType::Percent, 12},  // Modulo operator has same precedence as multiplication/division
};

Parser::Parser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem) 
    : diagnostics_(diagnostics), enhancedDiagnostics_(nullptr), typeSystem_(typeSystem) {}

Parser::Parser(utils::EnhancedDiagnosticEngine& enhancedDiagnostics, const TypeSystem& typeSystem) 
    : diagnostics_(enhancedDiagnostics), enhancedDiagnostics_(&enhancedDiagnostics), typeSystem_(typeSystem) {}

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
        std::cout << "DEBUG: Parser found variable declaration" << std::endl;
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
    
    // Handle interface declarations
    if (check(TokenType::Interface)) {
        return parseInterfaceDeclaration();
    }
    
    // Handle enum declarations
    if (check(TokenType::Enum)) {
        return parseEnumDeclaration();
    }
    
    // Handle type alias declarations
    if (check(TokenType::Type)) {
        return parseTypeAliasDeclaration();
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
        // Parse type directly without colon (colon already consumed)
        ParsingContext oldContext = currentContext_;
        setContext(ParsingContext::Type);
        typeAnnotation = parseUnionType();
        setContext(oldContext);
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
    
    // Parse optional type parameters
    std::vector<unique_ptr<TypeParameter>> typeParameters;
    if (check(TokenType::Less)) {
        // Set type context for type parameter parsing to handle union types correctly
        ParsingContext oldContext = currentContext_;
        setContext(ParsingContext::Type);
        typeParameters = parseTypeParameterList();
        setContext(oldContext);
    }
    
    consume(TokenType::LeftParen, "Expected '(' after function name");
    auto parameters = parseParameterList();
    consume(TokenType::RightParen, "Expected ')' after parameters");
    
    // Optional return type
    shared_ptr<Type> returnType = nullptr;
    if (match(TokenType::Colon)) {
        // Parse type directly without colon (colon already consumed)
        ParsingContext oldContext = currentContext_;
        setContext(ParsingContext::Type);
        returnType = parseUnionType();
        setContext(oldContext);
    }
    
    auto body = parseFunctionBody();
    
    return make_unique<FunctionDeclaration>(
        name, std::move(typeParameters), std::move(parameters), returnType, std::move(body), 
        nameToken.getLocation(), false, false
    );
}

unique_ptr<Statement> Parser::parseClassDeclaration() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Class, "Expected 'class'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected class name");
    String name = nameToken.getStringValue();
    
    // Optional type parameters
    std::vector<unique_ptr<TypeParameter>> typeParameters;
    if (match(TokenType::Less)) {
        do {
            // Parse variance annotation (optional)
            Variance variance = Variance::Invariant;
            if (match(TokenType::Out)) {
                variance = Variance::Covariant;
            } else if (match(TokenType::In)) {
                variance = Variance::Contravariant;
            }
            
            Token typeParamToken = consume(TokenType::Identifier, "Expected type parameter name");
            
            // Parse constraint (optional)
            shared_ptr<Type> constraint = nullptr;
            if (match(TokenType::Extends)) {
                // Set type context for constraint parsing to handle union types correctly
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                constraint = parseUnionType();
                if (!constraint) {
                    reportError("Expected constraint type after 'extends'", getCurrentLocation());
                    constraint = typeSystem_.getErrorType();
                }
                setContext(oldContext);
            }
            
            auto typeParam = make_unique<TypeParameter>(
                typeParamToken.getStringValue(),
                constraint,
                variance,
                getCurrentLocation()
            );
            typeParameters.push_back(std::move(typeParam));
        } while (match(TokenType::Comma));
        
        consume(TokenType::Greater, "Expected '>' after type parameters");
    }
    
    // Optional base class (extends clause)
    shared_ptr<Type> baseClass = nullptr;
    if (match(TokenType::Extends)) {
        // Parse type without colon (extends doesn't use colon syntax)
        // Set type context for better disambiguation
        ParsingContext oldContext = currentContext_;
        setContext(ParsingContext::Type);
        
        baseClass = parseUnionType();
        if (!baseClass) {
            reportError("Expected base class type after 'extends'", getCurrentLocation());
            baseClass = typeSystem_.getErrorType();
        }
        
        // Restore previous context
        setContext(oldContext);
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
    unique_ptr<DestructorDeclaration> destructor = nullptr;
    
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
        
        if (check(TokenType::Tilde)) {
            // Parse destructor declaration
            advance(); // consume '~'
            Token classNameToken = consume(TokenType::Identifier, "Expected class name after '~'");
            String destructorClassName = classNameToken.getStringValue();
            
            // Verify the destructor class name matches the current class name
            if (destructorClassName != name) {
                reportError("Destructor class name '" + destructorClassName + "' does not match class name '" + name + "'", 
                           classNameToken.getLocation());
            }
            
            consume(TokenType::LeftParen, "Expected '(' after destructor class name");
            consume(TokenType::RightParen, "Expected ')' after destructor class name (destructors take no parameters)");
            
            auto body = parseFunctionBody();
            
            if (destructor) {
                reportError("Class can only have one destructor", getCurrentLocation());
            } else {
                destructor = make_unique<DestructorDeclaration>(
                    destructorClassName, std::move(body), getCurrentLocation()
                );
            }
        } else if (check(TokenType::Identifier) || check(TokenType::Constructor)) {
            Token memberToken = advance();
            String memberName = memberToken.getStringValue();
            
            if (memberName == "constructor" || memberToken.getType() == TokenType::Constructor) {
                // Parse constructor
                std::cout << "DEBUG: Parser found constructor method" << std::endl;
                consume(TokenType::LeftParen, "Expected '(' after constructor");
                auto parameters = parseMethodParameterList();
                consume(TokenType::RightParen, "Expected ')' after constructor parameters");
                std::cout << "DEBUG: Parser parsed constructor with " << parameters.size() << " parameters" << std::endl;
                
                auto body = parseFunctionBody();
                std::cout << "DEBUG: Parser parsed constructor body" << std::endl;
                
                constructor = make_unique<MethodDeclaration>(
                    "constructor", std::move(parameters), typeSystem_.getVoidType(),
                    std::move(body), getCurrentLocation(), isStatic, isPrivate, isProtected, isAbstract
                );
            } else if (check(TokenType::LeftParen)) {
                // Method declaration with parameters
                std::cout << "DEBUG: Parser found method declaration: " << memberName << std::endl;
                std::vector<MethodDeclaration::Parameter> parameters;
                
                consume(TokenType::LeftParen, "Expected '(' after method name");
                parameters = parseMethodParameterList();
                consume(TokenType::RightParen, "Expected ')' after method parameters");
                
                // Optional return type
                shared_ptr<Type> returnType = typeSystem_.getVoidType();
                if (match(TokenType::Colon)) {
                    // Parse type directly without colon (colon already consumed)
                    ParsingContext oldContext = currentContext_;
                    setContext(ParsingContext::Type);
                    returnType = parseUnionType();
                    setContext(oldContext);
                }
                auto body = parseFunctionBody();
                
                methods.push_back(make_unique<MethodDeclaration>(
                    memberName, std::move(parameters), returnType, std::move(body),
                    memberToken.getLocation(), isStatic, isPrivate, isProtected, isAbstract
                ));
            } else if (check(TokenType::Colon)) {
                // Could be either method declaration with return type or property declaration
                // Look ahead to see if next token is '{' (method body) or something else (property type)
                advance(); // consume ':'
                auto returnType = parseUnionType();
                
                
                if (check(TokenType::LeftBrace)) {
                    // This is a method declaration with return type but no parameters
                    std::vector<MethodDeclaration::Parameter> parameters;
                    
                    auto body = parseFunctionBody();
                    
                    methods.push_back(make_unique<MethodDeclaration>(
                        memberName, std::move(parameters), returnType, std::move(body),
                        memberToken.getLocation(), isStatic, isPrivate, isProtected, isAbstract
                    ));
                } else {
                    // This is a property declaration
                    // Check for optional initializer
                    unique_ptr<Expression> initializer = nullptr;
                    if (match(TokenType::Equal)) {
                        initializer = parseExpression();
                    }
                    
                    // Optional semicolon for property declarations
                    if (match(TokenType::Semicolon)) {
                        // Semicolon consumed
                    }
                    
                    properties.push_back(make_unique<PropertyDeclaration>(
                        memberName, returnType, std::move(initializer), memberToken.getLocation(),
                        isStatic, isPrivate, isProtected, isReadonly
                    ));
                }
            } else {
                // Property declaration without type annotation
                std::cout << "DEBUG: Parser processing property without type: " << memberName << std::endl;
                
                unique_ptr<Expression> initializer = nullptr;
                if (match(TokenType::Equal)) {
                    initializer = parseExpression();
                }
                
                // Optional semicolon for property declarations
                if (match(TokenType::Semicolon)) {
                    // Semicolon consumed
                }
                
                properties.push_back(make_unique<PropertyDeclaration>(
                    memberName, nullptr, std::move(initializer), memberToken.getLocation(),
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
        name, std::move(typeParameters), baseClass, std::move(interfaces), std::move(properties), 
        std::move(methods), std::move(constructor), std::move(destructor), location
    );
}

unique_ptr<Statement> Parser::parseInterfaceDeclaration() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Interface, "Expected 'interface'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected interface name");
    String name = nameToken.getStringValue();
    
    // Optional type parameters
    std::vector<unique_ptr<TypeParameter>> typeParameters;
    if (match(TokenType::Less)) {
        do {
            // Parse variance annotation (optional)
            Variance variance = Variance::Invariant;
            if (match(TokenType::Out)) {
                variance = Variance::Covariant;
            } else if (match(TokenType::In)) {
                variance = Variance::Contravariant;
            }
            
            Token typeParamToken = consume(TokenType::Identifier, "Expected type parameter name");
            
            // Optional constraint (extends clause)
            shared_ptr<Type> constraint = nullptr;
            if (match(TokenType::Extends)) {
                // Set type context for better disambiguation
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                constraint = parseUnionType();
                setContext(oldContext);
            }
            
            auto typeParam = make_unique<TypeParameter>(
                typeParamToken.getStringValue(),
                constraint,
                variance,
                getCurrentLocation()
            );
            typeParameters.push_back(std::move(typeParam));
        } while (match(TokenType::Comma));
        
        consume(TokenType::Greater, "Expected '>' after type parameters");
    }
    
    // Optional extends clause
    std::vector<shared_ptr<Type>> extends;
    if (match(TokenType::Extends)) {
        do {
            // Parse interface type (which may include generic type arguments)
            ParsingContext oldContext = currentContext_;
            setContext(ParsingContext::Type);
            auto extendsType = parseUnionType();
            setContext(oldContext);
            extends.push_back(extendsType);
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::LeftBrace, "Expected '{' after interface header");
    
    // Parse interface body (properties and method signatures)
    std::vector<unique_ptr<PropertyDeclaration>> properties;
    std::vector<unique_ptr<MethodDeclaration>> methods;
    
    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        Token memberToken = consume(TokenType::Identifier, "Expected interface member");
        String memberName = memberToken.getStringValue();
        
        if (check(TokenType::LeftParen)) {
            // Method signature
            consume(TokenType::LeftParen, "Expected '(' after method name");
            auto parameters = parseMethodParameterList();
            consume(TokenType::RightParen, "Expected ')' after method parameters");
            
            // Return type is required for interface methods
            shared_ptr<Type> returnType = typeSystem_.getVoidType();
            if (match(TokenType::Colon)) {
                // Parse type directly without colon (colon already consumed)
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                returnType = parseUnionType();
                setContext(oldContext);
            }
            
            consume(TokenType::Semicolon, "Expected ';' after interface method signature");
            
            // Interface methods have no body (nullptr)
            methods.push_back(make_unique<MethodDeclaration>(
                memberName, std::move(parameters), returnType, nullptr,
                memberToken.getLocation(), false, false, false, false
            ));
        } else {
            // Property signature
            bool isOptional = false;
            
            // Check for optional property syntax: name?: type
            if (match(TokenType::Question)) {
                isOptional = true;
            }
            
            shared_ptr<Type> propertyType = nullptr;
            if (match(TokenType::Colon)) {
                // Parse type directly without colon (colon already consumed)
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                propertyType = parseUnionType();
                setContext(oldContext);
            }
            
            consume(TokenType::Semicolon, "Expected ';' after interface property");
            
            properties.push_back(make_unique<PropertyDeclaration>(
                memberName, propertyType, nullptr, memberToken.getLocation(),
                false, false, false, isOptional
            ));
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after interface body");
    
    return make_unique<InterfaceDeclaration>(
        name, std::move(typeParameters), std::move(extends), std::move(properties), std::move(methods), location
    );
}

unique_ptr<Statement> Parser::parseEnumDeclaration() {
    SourceLocation location = getCurrentLocation();
    
    // Check for const enum
    bool isConst = false;
    if (match(TokenType::Const)) {
        isConst = true;
    }
    
    consume(TokenType::Enum, "Expected 'enum'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected enum name");
    String name = nameToken.getStringValue();
    
    consume(TokenType::LeftBrace, "Expected '{' after enum name");
    
    // Parse enum members
    std::vector<unique_ptr<EnumMember>> members;
    
    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        Token memberToken = consume(TokenType::Identifier, "Expected enum member name");
        String memberName = memberToken.getStringValue();
        
        unique_ptr<Expression> value = nullptr;
        if (match(TokenType::Equal)) {
            value = parseExpression();
        }
        
        members.push_back(make_unique<EnumMember>(
            memberName, std::move(value), memberToken.getLocation()
        ));
        
        // Handle trailing comma
        if (!match(TokenType::Comma)) {
            break;
        }
    }
    
    consume(TokenType::RightBrace, "Expected '}' after enum body");
    
    return make_unique<EnumDeclaration>(
        name, std::move(members), location, isConst
    );
}

unique_ptr<Statement> Parser::parseTypeAliasDeclaration() {
    SourceLocation location = getCurrentLocation();
    consume(TokenType::Type, "Expected 'type'");
    
    Token nameToken = consume(TokenType::Identifier, "Expected type alias name");
    String name = nameToken.getStringValue();
    
    consume(TokenType::Equal, "Expected '=' after type alias name");
    
    // Parse the type expression
    // For now, we'll parse basic type references
    // In a full implementation, we'd handle union types, object types, etc.
    shared_ptr<Type> aliasedType = parseTypeAnnotation();
    
    consume(TokenType::Semicolon, "Expected ';' after type alias declaration");
    
    return make_unique<TypeAliasDeclaration>(name, aliasedType, location);
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
    
    // Check for for-of loop: for (const/let/var variable of iterable)
    if (check(TokenType::Const) || check(TokenType::Let) || check(TokenType::Var)) {
        // Save the variable declaration type
        TokenType varType = advance().getType();
        
        // Parse variable name
        Token varToken = consume(TokenType::Identifier, "Expected variable name in for-of loop");
        String varName = varToken.getStringValue();
        
        // Check for 'of' keyword
        if (check(TokenType::Of)) {
            advance(); // consume 'of'
            
            // Parse iterable expression
            auto iterable = parseExpression();
            
            consume(TokenType::RightParen, "Expected ')' after for-of iterable");
            
            // Parse body
            auto body = parseStatement();
            
            // Create ForOfStatement with variable name
            return make_unique<ForOfStatement>(varName, std::move(iterable), std::move(body), location);
        } else {
            // This is a regular for loop with variable declaration
            // Reset and parse as regular for loop
            // We need to backtrack here, but since we don't have backtracking,
            // we'll parse the variable declaration manually
            
            // Optional type annotation
            shared_ptr<Type> typeAnnotation = nullptr;
            if (match(TokenType::Colon)) {
                typeAnnotation = parseTypeAnnotation();
            }
            
            // Optional initializer
            unique_ptr<Expression> initializer = nullptr;
            if (match(TokenType::Equal)) {
                initializer = parseAssignmentExpression();
            }
            
            consume(TokenType::Semicolon, "Expected ';' after for init");
            
            // Convert TokenType to VariableDeclaration::Kind
            VariableDeclaration::Kind kind;
            switch (varType) {
                case TokenType::Var: kind = VariableDeclaration::Kind::Var; break;
                case TokenType::Let: kind = VariableDeclaration::Kind::Let; break;
                case TokenType::Const: kind = VariableDeclaration::Kind::Const; break;
                default: kind = VariableDeclaration::Kind::Let; break; // fallback
            }
            
            auto init = make_unique<VariableDeclaration>(kind, varName, std::move(initializer), typeAnnotation, varToken.getLocation());
            
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
    }
    
    // Regular for loop without variable declaration
    // Parse init (can be expression or empty)
    unique_ptr<Statement> init = nullptr;
    if (!check(TokenType::Semicolon)) {
        // Parse expression and wrap in expression statement
        auto expr = parseExpression();
        consume(TokenType::Semicolon, "Expected ';' after for init");
        init = make_unique<ExpressionStatement>(std::move(expr), getCurrentLocation());
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
    return parseAssignmentExpression();
}

unique_ptr<Expression> Parser::parseAssignmentExpression() {
    // Check for arrow functions using a simple heuristic
    if (looksLikeArrowFunction()) {
        auto arrowFunc = parseArrowFunction();
        if (arrowFunc) {
            return arrowFunc;
        }
        // If arrow function parsing failed, fall through to normal expression parsing
    }
    
    auto expr = parseConditionalExpression();
    
    // Handle assignment operators
    if (check(TokenType::Equal) || check(TokenType::PlusEqual) || check(TokenType::MinusEqual)) {
        Token opToken = advance();
        auto right = parseAssignmentExpression(); // Right-associative
        
        AssignmentExpression::Operator assignOp = AssignmentExpression::Operator::Assign;
        if (opToken.getType() == TokenType::PlusEqual) {
            assignOp = AssignmentExpression::Operator::AddAssign;
        } else if (opToken.getType() == TokenType::MinusEqual) {
            assignOp = AssignmentExpression::Operator::SubtractAssign;
        }
        
        return make_unique<AssignmentExpression>(
            std::move(expr),
            assignOp,
            std::move(right),
            opToken.getLocation()
        );
    }
    
    return expr;
}

unique_ptr<Expression> Parser::parseConditionalExpression() {
    auto condition = parseBinaryExpression();
    
    // Check for ternary operator
    if (check(TokenType::Question)) {
        advance(); // consume '?'
        auto trueExpr = parseAssignmentExpression();
        
        if (!check(TokenType::Colon)) {
            reportError("Expected ':' in ternary expression");
            return condition;
        }
        advance(); // consume ':'
        
        auto falseExpr = parseConditionalExpression(); // Right-associative
        
        return make_unique<ConditionalExpression>(
            std::move(condition),
            std::move(trueExpr),
            std::move(falseExpr),
            condition->getLocation()
        );
    }
    
    return condition;
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
        auto right = parseBinaryExpression(precedence + 1);
        left = make_unique<BinaryExpression>(
            std::move(left),
            tokenToBinaryOperator(opType),
            std::move(right),
            opToken.getLocation()
        );
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
        else if (check(TokenType::LeftParen) || (check(TokenType::Less) && isTypeArgumentList())) {
            // Parse function call with optional type arguments: expr<T>(args...) or expr(args...)
            std::vector<shared_ptr<Type>> typeArguments;
            
            // Parse type arguments if present
            if (match(TokenType::Less)) {
                if (!check(TokenType::Greater)) {
                    do {
                        typeArguments.push_back(parseTypeAnnotation());
                    } while (match(TokenType::Comma));
                }
                consume(TokenType::Greater, "Expected '>' after type arguments");
            }
            
            // Now parse the function call
            consume(TokenType::LeftParen, "Expected '(' after function name or type arguments");
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
            if (typeArguments.empty()) {
                expr = make_unique<CallExpression>(
                    std::move(expr),
                    std::move(arguments),
                    location
                );
            } else {
                expr = make_unique<CallExpression>(
                    std::move(expr),
                    std::move(typeArguments),
                    std::move(arguments),
                    location
                );
            }
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
    
    // Template literals support
    std::cout << "DEBUG: Current token type: " << static_cast<int>(peek().getType()) << std::endl;
    if (check(TokenType::NoSubstitutionTemplate) || check(TokenType::TemplateHead)) {
        return parseTemplateLiteral();
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
    
    if (check(TokenType::Super)) {
        advance();
        return make_unique<SuperExpression>(getCurrentLocation());
    }
    
    if (check(TokenType::New)) {
        advance();
        return parseNewExpression();
    }
    
    if (check(TokenType::Function)) {
        return parseFunctionExpression();
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
    
    // Parse optional type arguments (e.g., "<number>", "<string, boolean>")
    std::vector<shared_ptr<Type>> typeArguments;
    if (check(TokenType::Less)) {
        std::cout << "DEBUG: Parser found type arguments in NewExpression" << std::endl;
        // Use the existing type argument parsing infrastructure
        typeArguments = parseTypeArgumentList();
        std::cout << "DEBUG: Parsed " << typeArguments.size() << " type arguments" << std::endl;
    } else {
        std::cout << "DEBUG: Parser did not find type arguments in NewExpression" << std::endl;
    }
    
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
    
    return make_unique<NewExpression>(std::move(constructor), std::move(typeArguments), 
                                     std::move(arguments), location);
}

// Template literals parser
unique_ptr<Expression> Parser::parseTemplateLiteral() {
    SourceLocation location = getCurrentLocation();
    std::vector<TemplateElement> elements;
    
    if (check(TokenType::NoSubstitutionTemplate)) {
        // Simple template literal without expressions: `hello world`
        Token token = advance();
        elements.emplace_back(TemplateElement(token.getStringValue()));
        return make_unique<TemplateLiteral>(std::move(elements), location);
    }
    
    // Parse template literal with expressions: `Hello ${name}!`
    while (true) {
        if (check(TokenType::TemplateHead)) {
            // TemplateHead: "Hello "
            Token token = advance();
            elements.emplace_back(TemplateElement(token.getStringValue()));
            
            // Parse the expression inside ${}
            // Handle empty expressions gracefully
            if (check(TokenType::TemplateTail)) {
                // Empty expression: ${} - create an empty string literal
                auto emptyExpression = make_unique<StringLiteral>("", location);
                elements.emplace_back(TemplateElement(std::move(emptyExpression)));
            } else {
                auto expression = parseExpression();
                elements.emplace_back(TemplateElement(std::move(expression)));
            }
            
        } else if (check(TokenType::TemplateMiddle)) {
            // TemplateMiddle: " world "
            Token token = advance();
            elements.emplace_back(TemplateElement(token.getStringValue()));
            
            // Parse the expression inside ${}
            // Handle empty expressions gracefully
            if (check(TokenType::TemplateTail)) {
                // Empty expression: ${} - create an empty string literal
                auto emptyExpression = make_unique<StringLiteral>("", location);
                elements.emplace_back(TemplateElement(std::move(emptyExpression)));
            } else {
                auto expression = parseExpression();
                elements.emplace_back(TemplateElement(std::move(expression)));
            }
            
        } else if (check(TokenType::TemplateTail)) {
            // TemplateTail: "!"
            Token token = advance();
            elements.emplace_back(TemplateElement(token.getStringValue()));
            break; // End of template literal
            
        } else {
            // Unexpected token
            break;
        }
    }
    
    return make_unique<TemplateLiteral>(std::move(elements), location);
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
        case TokenType::Percent: return BinaryExpression::Operator::Modulo;
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

void Parser::reportError(const String& message, const SourceLocation& location, 
                        const String& context, const String& suggestion) {
    SourceLocation loc = location.isValid() ? location : getCurrentLocation();
    
    // Use enhanced diagnostic engine if available
    if (enhancedDiagnostics_) {
        enhancedDiagnostics_->error(message, loc, context, suggestion);
    } else {
        // Fallback to basic diagnostic engine
        if (!context.empty()) {
            diagnostics_.error(message + " (Context: " + context + ")", loc);
        } else {
            diagnostics_.error(message, loc);
        }
        
        if (!suggestion.empty()) {
            diagnostics_.warning("Suggestion: " + suggestion, loc);
        }
    }
}

void Parser::reportWarning(const String& message, const SourceLocation& location, 
                          const String& context, const String& suggestion) {
    SourceLocation loc = location.isValid() ? location : getCurrentLocation();
    
    // Use enhanced diagnostic engine if available
    if (enhancedDiagnostics_) {
        enhancedDiagnostics_->warning(message, loc, context, suggestion);
    } else {
        // Fallback to basic diagnostic engine
        if (!context.empty()) {
            diagnostics_.warning(message + " (Context: " + context + ")", loc);
        } else {
            diagnostics_.warning(message, loc);
        }
        
        if (!suggestion.empty()) {
            diagnostics_.warning("Suggestion: " + suggestion, loc);
        }
    }
}

void Parser::reportInfo(const String& message, const SourceLocation& location, 
                       const String& context) {
    SourceLocation loc = location.isValid() ? location : getCurrentLocation();
    
    // Use enhanced diagnostic engine if available
    if (enhancedDiagnostics_) {
        enhancedDiagnostics_->info(message, loc, context);
    } else {
        // Fallback to basic diagnostic engine
        if (!context.empty()) {
            diagnostics_.warning("Info: " + message + " (Context: " + context + ")", loc);
        } else {
            diagnostics_.warning("Info: " + message, loc);
        }
    }
}

void Parser::synchronize() {
    skipToStatementBoundary();
}

void Parser::skipToStatementBoundary() {
    while (!isAtEnd()) {
        if (peek().getType() == TokenType::Semicolon) {
            advance();
            break;
        }
        if (isStatementStart(peek().getType())) {
            break;
        }
        advance();
    }
}

void Parser::skipToDeclarationBoundary() {
    while (!isAtEnd()) {
        if (peek().getType() == TokenType::Semicolon) {
            advance();
            break;
        }
        if (isDeclarationStart(peek().getType())) {
            break;
        }
        advance();
    }
}

void Parser::setContext(ParsingContext context) {
    currentContext_ = context;
}

ParsingContext Parser::getCurrentContext() const {
    return currentContext_;
}

Token Parser::peekAhead(size_t offset) const {
    while (lookaheadCache_.tokens_.size() <= offset) {
        if (tokens_->isAtEnd()) break;
        lookaheadCache_.tokens_.push_back(
            tokens_->peekAhead(lookaheadCache_.tokens_.size())
        );
    }
    if (offset < lookaheadCache_.tokens_.size()) {
        return lookaheadCache_.tokens_[offset];
    }
    return Token(TokenType::EndOfInput, SourceLocation());
}

bool Parser::hasAhead(size_t offset) const {
    return !tokens_->isAtEnd() || offset < lookaheadCache_.tokens_.size();
}

SourceLocation Parser::getCurrentLocation() const {
    return peek().getLocation();
}

// Parse type annotations like ": number", ": string", etc.
shared_ptr<Type> Parser::parseTypeAnnotation() {
    if (!check(TokenType::Colon)) return nullptr;
    advance();
    
    // Set type context for better disambiguation
    ParsingContext oldContext = currentContext_;
    setContext(ParsingContext::Type);
    
    auto type = parseUnionType();
    
    // Restore previous context
    setContext(oldContext);
    
    return type;
}

shared_ptr<Type> Parser::parseUnionType() {
    auto firstType = parsePrimaryType();
    if (!firstType) {
        return nullptr;
    }
    
    // Check if this is a union type (has '|' operator)
    std::vector<shared_ptr<Type>> unionTypes;
    unionTypes.push_back(firstType);
    
    while (match(TokenType::Pipe)) {
        auto nextType = parsePrimaryType();
        if (!nextType) {
            reportError("Expected type after '|' in union type", getCurrentLocation());
            return typeSystem_.getErrorType();
        }
        unionTypes.push_back(nextType);
    }
    
    // If we only have one type, return it directly
    if (unionTypes.size() == 1) {
        return unionTypes[0];
    }
    
    // Create union type
    return typeSystem_.createUnionType(std::move(unionTypes));
}

shared_ptr<Type> Parser::parseArrayType(shared_ptr<Type> baseType) {
    // Handle recursive array syntax: T[], T[][], T[][][], etc.
    while (check(TokenType::LeftBracket)) {
        advance(); // consume '['
        consume(TokenType::RightBracket, "Expected ']' after array type");
        baseType = typeSystem_.createArrayType(baseType);
    }
    return baseType;
}

shared_ptr<Type> Parser::parsePrimaryType() {
    // Handle TypeScript type keywords
    shared_ptr<Type> baseType = nullptr;
    if (check(TokenType::Number)) {
        advance();
        baseType = typeSystem_.getNumberType();
    } else if (check(TokenType::String)) {
        advance();
        baseType = typeSystem_.getStringType();
    } else if (check(TokenType::Boolean)) {
        advance();
        baseType = typeSystem_.getBooleanType();
    } else if (check(TokenType::Null)) {
        advance();
        baseType = typeSystem_.getNullType();
    } else if (check(TokenType::Undefined)) {
        advance();
        baseType = typeSystem_.getUndefinedType();
    } else if (check(TokenType::Void)) {
        advance();
        baseType = typeSystem_.getVoidType();
    } else if (check(TokenType::Any)) {
        advance();
        baseType = typeSystem_.getAnyType();
    } else if (check(TokenType::Unknown)) {
        advance();
        baseType = typeSystem_.getUnknownType();
    } else if (check(TokenType::Never)) {
        advance();
        baseType = typeSystem_.getNeverType();
    }
    
    // If we parsed a primitive type, check for array syntax
    if (baseType) {
        return parseArrayType(baseType);
    }
    
    // Handle literal types (string literals, numeric literals, boolean literals)
    if (check(TokenType::StringLiteral)) {
        Token literalToken = advance();
        String literalValue = literalToken.getStringValue();
        return typeSystem_.createLiteralType(TypeKind::StringLiteral, literalValue);
    } else if (check(TokenType::NumericLiteral)) {
        Token literalToken = advance();
        String literalValue = literalToken.getStringValue();
        return typeSystem_.createLiteralType(TypeKind::NumericLiteral, literalValue);
    } else if (check(TokenType::True)) {
        advance(); // consume 'true'
        return typeSystem_.createLiteralType(TypeKind::BooleanLiteral, "true");
    } else if (check(TokenType::False)) {
        advance(); // consume 'false'
        return typeSystem_.createLiteralType(TypeKind::BooleanLiteral, "false");
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
                auto typeArg = parseUnionType();
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
            } else if (typeName == "unique_ptr") {
                if (typeArguments.size() != 1) {
                    reportError("unique_ptr type requires exactly one type argument", typeToken.getLocation());
                    return typeSystem_.getErrorType();
                }
                return typeSystem_.createUniquePtrType(typeArguments[0]);
            } else if (typeName == "shared_ptr") {
                if (typeArguments.size() != 1) {
                    reportError("shared_ptr type requires exactly one type argument", typeToken.getLocation());
                    return typeSystem_.getErrorType();
                }
                return typeSystem_.createSharedPtrType(typeArguments[0]);
            } else if (typeName == "weak_ptr") {
                if (typeArguments.size() != 1) {
                    reportError("weak_ptr type requires exactly one type argument", typeToken.getLocation());
                    return typeSystem_.getErrorType();
                }
                return typeSystem_.createWeakPtrType(typeArguments[0]);
            }
            
            // For other generic types, create a generic type
            // Create unresolved base type that will be resolved in semantic analysis
            auto baseType = typeSystem_.createUnresolvedType(typeName);
            return typeSystem_.createGenericType(baseType, std::move(typeArguments));
        }
        
        // Handle built-in non-generic identifiers
        if (typeName == "Array") {
            // Array without type arguments defaults to Array<any>
            return typeSystem_.createArrayType(typeSystem_.getAnyType());
        }
        
        // Create unresolved type for unknown identifiers (will be resolved in semantic analysis)
        auto baseType = typeSystem_.createUnresolvedType(typeName);
        
        // Check for array syntax: TypeName[] (can be recursive for multi-dimensional arrays)
        return parseArrayType(baseType);
    }
    
    // Handle array types like "number[]"
    if (check(TokenType::LeftBracket)) {
        // This should not happen here - array syntax should be handled after parsing the base type
        reportError("Unexpected '[' in type annotation", getCurrentLocation());
        return typeSystem_.getErrorType();
    }
    
    reportError("Expected type name", getCurrentLocation());
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
                // Parse type directly without colon (colon already consumed)
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                param.type = parseUnionType();
                setContext(oldContext);
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
                // Parse type directly without colon (colon already consumed)
                ParsingContext oldContext = currentContext_;
                setContext(ParsingContext::Type);
                param.type = parseUnionType();
                setContext(oldContext);
            }
            
            // Parse optional default value
            if (match(TokenType::Equal)) {
                param.defaultValue = parseExpression();
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

std::vector<unique_ptr<TypeParameter>> Parser::parseTypeParameterList() {
    std::vector<unique_ptr<TypeParameter>> typeParameters;
    
    consume(TokenType::Less, "Expected '<' before type parameters");
    
    if (!check(TokenType::Greater)) {
        do {
            typeParameters.push_back(parseTypeParameter());
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::Greater, "Expected '>' after type parameters");
    
    return typeParameters;
}

unique_ptr<TypeParameter> Parser::parseTypeParameter() {
    Token nameToken = consume(TokenType::Identifier, "Expected type parameter name");
    String name = nameToken.getStringValue();
    
    shared_ptr<Type> constraint = nullptr;
    if (match(TokenType::Extends)) {
        // Set type context for better disambiguation
        ParsingContext oldContext = currentContext_;
        setContext(ParsingContext::Type);
        
        constraint = parseUnionType();
        if (!constraint) {
            reportError("Expected constraint type after 'extends'", getCurrentLocation());
            constraint = typeSystem_.getErrorType();
        }
        
        // Restore previous context
        setContext(oldContext);
    }
    
    return make_unique<TypeParameter>(name, constraint, Variance::Invariant, nameToken.getLocation());
}

std::vector<shared_ptr<Type>> Parser::parseTypeArgumentList() {
    std::vector<shared_ptr<Type>> typeArguments;
    
    consume(TokenType::Less, "Expected '<' before type arguments");
    
    if (!check(TokenType::Greater)) {
        do {
            // Parse type directly without colon (type arguments don't have colons)
            ParsingContext oldContext = currentContext_;
            setContext(ParsingContext::Type);
            typeArguments.push_back(parseUnionType());
            setContext(oldContext);
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::Greater, "Expected '>' after type arguments");
    
    return typeArguments;
}

// Arrow function parsing
unique_ptr<Expression> Parser::parseArrowFunction() {
    SourceLocation location = getCurrentLocation();
    std::vector<ArrowFunction::Parameter> parameters;
    
    if (check(TokenType::Identifier)) {
        // Single parameter without parentheses: identifier => body
        Token param = advance();
        
        // Must be followed by arrow
        if (!check(TokenType::Arrow)) {
            reportError("Expected '=>' after parameter", getCurrentLocation());
            return nullptr;
        }
        
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
        
        if (!match(TokenType::RightParen)) {
            reportError("Expected ')' after parameters", getCurrentLocation());
            return nullptr;
        }
        
        // Must be followed by arrow
        if (!check(TokenType::Arrow)) {
            // This is not an arrow function, it's a parenthesized expression
            // We need to return nullptr to let the caller handle this
            reportError("Expected '=>' after parameters", getCurrentLocation());
            return nullptr;
        }
    } else {
        reportError("Expected parameter or '(' in arrow function", getCurrentLocation());
        return nullptr;
    }
    
    if (!match(TokenType::Arrow)) {
        reportError("Expected '=>' in arrow function", getCurrentLocation());
        return nullptr;
    }
    
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
    // Simple heuristic for arrow function detection
    // Pattern 1: identifier => (single parameter)
    if (check(TokenType::Identifier)) {
        // We need to peek ahead to see if there's an arrow
        // Since we can't backtrack easily, let's try a different approach
        // For now, let's be conservative and only detect simple cases
        return false; // Will implement this step by step
    }
    
    // Pattern 2: (params) => 
    if (check(TokenType::LeftParen)) {
        // TODO: Implement proper lookahead for arrow function detection
        // For now, be conservative and let parenthesized expressions be handled normally
        // This prevents parsing errors with complex arithmetic expressions like (a + b)
        return false;
    }
    
    return false;
}

unique_ptr<Expression> Parser::parseFunctionExpression() {
    SourceLocation location = getCurrentLocation();
    
    // Consume 'function' keyword
    consume(TokenType::Function, "Expected 'function' keyword");
    
    // Optional function name (for named function expressions)
    String functionName = "";
    if (check(TokenType::Identifier)) {
        Token nameToken = advance();
        functionName = nameToken.getStringValue();
    }
    
    // Parse parameter list
    std::vector<FunctionExpression::Parameter> parameters;
    consume(TokenType::LeftParen, "Expected '(' after function name or 'function'");
    
    if (!check(TokenType::RightParen)) {
        do {
            if (check(TokenType::Identifier)) {
                Token param = advance();
                FunctionExpression::Parameter parameter;
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
    
    // Optional return type annotation
    shared_ptr<Type> returnType = nullptr;
    if (match(TokenType::Colon)) {
        returnType = parseTypeAnnotation();
    }
    
    // Parse function body (must be a block statement)
    unique_ptr<Statement> body = nullptr;
    if (match(TokenType::LeftBrace)) {
        body = parseBlockStatement();
    } else {
        reportError("Expected '{' for function expression body", getCurrentLocation());
        return nullptr;
    }
    
    if (!body) {
        reportError("Expected function expression body", getCurrentLocation());
        return nullptr;
    }
    
    return make_unique<FunctionExpression>(functionName, std::move(parameters), std::move(body), returnType, location);
}

bool Parser::isTypeArgumentList() const {
    if (!check(TokenType::Less)) return false;
    
    // In type context, < is more likely to be type arguments
    if (currentContext_ == ParsingContext::Type) {
        return true;
    }
    
    // In expression context, use sophisticated lookahead
    return analyzeTypeArgumentPattern();
}

bool Parser::analyzeTypeArgumentPattern() const {
    // Look ahead to see if this looks like a type argument list
    size_t offset = 1;
    
    // Skip whitespace
    while (hasAhead(offset) && peekAhead(offset).getType() == TokenType::WhiteSpace) {
        offset++;
    }
    
    // Check for identifier (type parameter)
    if (!hasAhead(offset) || peekAhead(offset).getType() != TokenType::Identifier) {
        return false;
    }
    
    offset++;
    
    // Skip whitespace
    while (hasAhead(offset) && peekAhead(offset).getType() == TokenType::WhiteSpace) {
        offset++;
    }
    
    // Check for comma (multiple type parameters) or closing >
    if (hasAhead(offset)) {
        TokenType nextType = peekAhead(offset).getType();
        return nextType == TokenType::Comma || nextType == TokenType::Greater;
    }
    
    return false;
}

bool Parser::isStatementStart(TokenType type) const {
    switch (type) {
        case TokenType::If:
        case TokenType::While:
        case TokenType::For:
        case TokenType::Return:
        case TokenType::Break:
        case TokenType::Continue:
        case TokenType::Try:
        case TokenType::Throw:
        case TokenType::Let:
        case TokenType::Const:
        case TokenType::Var:
        case TokenType::Function:
        case TokenType::Class:
        case TokenType::Interface:
        case TokenType::Enum:
            return true;
        default:
            return false;
    }
}

bool Parser::isDeclarationStart(TokenType type) const {
    switch (type) {
        case TokenType::Let:
        case TokenType::Const:
        case TokenType::Var:
        case TokenType::Function:
        case TokenType::Class:
        case TokenType::Interface:
        case TokenType::Enum:
        case TokenType::Type:
        case TokenType::Namespace:
            return true;
        default:
            return false;
    }
}

// Factory function
unique_ptr<Parser> createParser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem) {
    return make_unique<Parser>(diagnostics, typeSystem);
}

unique_ptr<Parser> createEnhancedParser(utils::EnhancedDiagnosticEngine& diagnostics, const TypeSystem& typeSystem) {
    return make_unique<Parser>(diagnostics, typeSystem);
}

} // namespace tsc
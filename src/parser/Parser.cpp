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
        {TokenType::QuestionQuestion, 4}, // Nullish coalescing has same precedence as logical OR
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
        {TokenType::Percent, 12}, // Modulo operator has same precedence as multiplication/division
        {TokenType::StarStar, 13}, // Exponentiation has higher precedence than multiplication/division
    };

    Parser::Parser(DiagnosticEngine &diagnostics, const TypeSystem &typeSystem)
        : diagnostics_(diagnostics), enhancedDiagnostics_(nullptr), typeSystem_(typeSystem) {
        errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);
    }

    Parser::Parser(utils::EnhancedDiagnosticEngine &enhancedDiagnostics, const TypeSystem &typeSystem)
        : diagnostics_(enhancedDiagnostics), enhancedDiagnostics_(&enhancedDiagnostics), typeSystem_(typeSystem) {
        errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);
    }

    Parser::~Parser() = default;

    unique_ptr<Module> Parser::parse(const std::vector<Token> &tokens, const String &filename) {
        std::cout << "DEBUG: Parser::parse() called with " << tokens.size() << " tokens for file: " << filename <<
                std::endl;
        // Create a token stream from the vector
        auto tokenStream = make_unique<VectorTokenStream>(tokens);
        return parse(std::move(tokenStream), filename);
    }

    unique_ptr<Module> Parser::parse(unique_ptr<TokenStream> tokenStream, const String &filename) {
        tokens_ = std::move(tokenStream);
        filename_ = filename;

        try {
            return parseModule();
        } catch (const CompilerError &e) {
            reportError(e.what(), e.getLocation());
            return nullptr;
        }
    }

    unique_ptr<Module> Parser::parseModule() {
        std::cout << "DEBUG: Parser::parseModule() called" << std::endl;
        std::vector<unique_ptr<Statement> > statements;

        std::cout << "DEBUG: Starting parseModule loop, isAtEnd(): " << isAtEnd() << std::endl;
        while (!isAtEnd()) {
            std::cout << "DEBUG: parseModule loop iteration, calling parseStatement()" << std::endl;
            try {
                if (auto stmt = parseStatement()) {
                    std::cout << "DEBUG: parseStatement returned a statement" << std::endl;
                    statements.push_back(std::move(stmt));
                } else {
                    std::cout << "DEBUG: parseStatement returned null" << std::endl;
                }
            } catch (const CompilerError &) {
                std::cout << "DEBUG: parseStatement threw CompilerError" << std::endl;
                synchronize();
            }
        }

        return make_unique<Module>(filename_, std::move(statements));
    }

    unique_ptr<Statement> Parser::parseStatement() {
        std::cout << "DEBUG: parseStatement() called" << std::endl;

        // Handle import declarations
        if (match(TokenType::Import)) {
            return parseImportDeclaration();
        }

        // Handle export declarations
        if (match(TokenType::Export)) {
            return parseExportDeclaration();
        }

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
        std::cout << "DEBUG: parseVariableStatement called, current token: " << peek().toString() << std::endl;
        // Check if this is a destructuring assignment: let [a, b] = array;
        if (check(TokenType::LeftBracket) || check(TokenType::LeftBrace)) {
            return parseDestructuringVariableStatement();
        }

        // Parse regular variable declaration: let name = value;
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
            initializer = parseAssignmentExpression();
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
        std::vector<unique_ptr<TypeParameter> > typeParameters;
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
        std::vector<unique_ptr<TypeParameter> > typeParameters;
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
                        errorReporter_->reportExpectedToken(getCurrentLocation(), "constraint type", "'extends'",
                                                            "Add a type constraint after 'extends'");
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
                errorReporter_->reportExpectedToken(getCurrentLocation(), "base class type", "'extends'",
                                                    "Add a base class type after 'extends'");
                baseClass = typeSystem_.getErrorType();
            }

            // Restore previous context
            setContext(oldContext);
        }

        // Optional interfaces (implements clause)
        std::vector<shared_ptr<Type> > interfaces;
        if (match(TokenType::Implements)) {
            do {
                Token interfaceToken = consume(TokenType::Identifier, "Expected interface name");
                // Create placeholder type - will be resolved in semantic analysis
                interfaces.push_back(typeSystem_.createClassType(interfaceToken.getStringValue()));
            } while (match(TokenType::Comma));
        }

        consume(TokenType::LeftBrace, "Expected '{' after class header");

        // Parse class body
        std::vector<unique_ptr<PropertyDeclaration> > properties;
        std::vector<unique_ptr<MethodDeclaration> > methods;
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
                else if (match(TokenType::Public)) {
                } // Default, do nothing
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
                    reportError(
                        "Destructor class name '" + destructorClassName + "' does not match class name '" + name + "'",
                        classNameToken.getLocation());
                }

                consume(TokenType::LeftParen, "Expected '(' after destructor class name");
                consume(TokenType::RightParen,
                        "Expected ')' after destructor class name (destructors take no parameters)");

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
                    std::cout << "DEBUG: Parser parsed constructor with " << parameters.size() << " parameters" <<
                            std::endl;

                    auto body = parseFunctionBody();
                    std::cout << "DEBUG: Parser parsed constructor body" << std::endl;

                    constructor = make_unique<MethodDeclaration>(
                        "constructor", std::vector<unique_ptr<TypeParameter>>{}, std::move(parameters), typeSystem_.getVoidType(),
                        std::move(body), getCurrentLocation(), isStatic, isPrivate, isProtected, isAbstract
                    );
                } else if (check(TokenType::LeftParen) || check(TokenType::Less)) {
                    // Method declaration with parameters (possibly with type parameters)
                    std::cout << "DEBUG: Parser found method declaration: " << memberName << std::endl;
                    
                    // Parse optional type parameters
                    std::vector<unique_ptr<TypeParameter>> typeParameters;
                    if (check(TokenType::Less)) {
                        // Set type context for type parameter parsing to handle union types correctly
                        ParsingContext oldContext = currentContext_;
                        setContext(ParsingContext::Type);
                        typeParameters = parseTypeParameterList();
                        setContext(oldContext);
                    }
                    
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
                        memberName, std::move(typeParameters), std::move(parameters), returnType, std::move(body),
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
                            memberName, std::vector<unique_ptr<TypeParameter>>{}, std::move(parameters), returnType, std::move(body),
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
                errorReporter_->reportInvalidDeclaration(getCurrentLocation(),
                                                         "Expected class member (method, property, constructor, or destructor)");
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
        std::vector<unique_ptr<TypeParameter> > typeParameters;
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
        std::vector<shared_ptr<Type> > extends;
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
        std::vector<unique_ptr<PropertyDeclaration> > properties;
        std::vector<unique_ptr<MethodDeclaration> > methods;

        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            Token memberToken = consume(TokenType::Identifier, "Expected interface member");
            String memberName = memberToken.getStringValue();

            if (check(TokenType::LeftParen) || check(TokenType::Less)) {
                // Method signature (possibly with type parameters)
                
                // Parse optional type parameters
                std::vector<unique_ptr<TypeParameter>> typeParameters;
                if (check(TokenType::Less)) {
                    // Set type context for type parameter parsing to handle union types correctly
                    ParsingContext oldContext = currentContext_;
                    setContext(ParsingContext::Type);
                    typeParameters = parseTypeParameterList();
                    setContext(oldContext);
                }
                
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
                    memberName, std::move(typeParameters), std::move(parameters), returnType, nullptr,
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
        std::vector<unique_ptr<EnumMember> > members;

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

        // Parse the type expression with full support for union types, object types, etc.
        shared_ptr<Type> aliasedType = parseTypeAnnotation();

        consume(TokenType::Semicolon, "Expected ';' after type alias declaration");

        return make_unique<TypeAliasDeclaration>(name, aliasedType, location);
    }

    unique_ptr<Statement> Parser::parseImportDeclaration() {
        SourceLocation location = getCurrentLocation();

        // Parse import clause
        ImportClause clause = parseImportClause();

        // Parse 'from' keyword
        consume(TokenType::From, "Expected 'from' after import clause");

        // Parse module specifier
        String moduleSpecifier = parseModuleSpecifier();

        // Expect semicolon
        consume(TokenType::Semicolon, "Expected ';' after import declaration");

        return make_unique<ImportDeclaration>(std::move(clause), moduleSpecifier, location);
    }

    unique_ptr<Statement> Parser::parseExportDeclaration() {
        SourceLocation location = getCurrentLocation();

        // Parse export clause
        ExportClause clause = parseExportClause();

        // Parse optional 'from' clause for re-exports
        String moduleSpecifier = "";
        if (match(TokenType::From)) {
            moduleSpecifier = parseModuleSpecifier();
        }

        // Only expect semicolon for export statements (not export declarations)
        // Export declarations like "export function add() {}" don't need semicolons
        if (clause.getType() == ExportClause::Named || clause.getType() == ExportClause::ReExport || clause.getType() ==
            ExportClause::All) {
            consume(TokenType::Semicolon, "Expected ';' after export statement");
        }

        return make_unique<ExportDeclaration>(std::move(clause), moduleSpecifier, location);
    }

    ImportClause Parser::parseImportClause() {
        // Check for default import
        if (check(TokenType::Identifier)) {
            Token defaultToken = consume(TokenType::Identifier, "Expected identifier");
            String defaultBinding = defaultToken.getStringValue();

            // Check if this is followed by a comma (mixed import)
            if (match(TokenType::Comma)) {
                // Mixed import: import Button, { ButtonProps } from "./Button"
                std::vector<ImportSpec> namedImports = parseNamedImports();
                return ImportClause(ImportClause::Mixed, defaultBinding, namedImports);
            } else {
                // Default import: import Button from "./Button"
                return ImportClause(ImportClause::Default, defaultBinding);
            }
        }

        // Check for named imports
        if (check(TokenType::LeftBrace)) {
            // Named imports: import { add, subtract } from "./math"
            std::vector<ImportSpec> namedImports = parseNamedImports();
            return ImportClause(ImportClause::Named, "", namedImports);
        }

        // Check for namespace import
        if (match(TokenType::Star)) {
            // Namespace import: import * as utils from "./utils"
            consume(TokenType::As, "Expected 'as' after '*'");
            Token namespaceToken = consume(TokenType::Identifier, "Expected namespace identifier");
            String namespaceBinding = namespaceToken.getStringValue();
            return ImportClause(ImportClause::Namespace, "", {}, namespaceBinding);
        }

        // If we get here, it's an error
        throw CompilerError("Invalid import clause", getCurrentLocation());
    }

    std::vector<ImportSpec> Parser::parseNamedImports() {
        std::vector<ImportSpec> namedImports;

        // Consume the opening brace
        consume(TokenType::LeftBrace, "Expected '{' before named imports");

        do {
            // Parse import spec
            Token importedToken = consume(TokenType::Identifier, "Expected imported name");
            String importedName = importedToken.getStringValue();
            String localName = importedName; // Default to same name

            // Check for 'as' clause
            if (match(TokenType::As)) {
                Token localToken = consume(TokenType::Identifier, "Expected local name after 'as'");
                localName = localToken.getStringValue();
            }

            namedImports.emplace_back(importedName, localName);
        } while (match(TokenType::Comma));

        consume(TokenType::RightBrace, "Expected '}' after named imports");

        return namedImports;
    }

    ExportClause Parser::parseExportClause() {
        // Check for default export
        if (match(TokenType::Default)) {
            // Default export: export default class Button
            unique_ptr<Expression> defaultExport = parseExpression();
            return ExportClause(ExportClause::Default, {}, std::move(defaultExport));
        }

        // Check for named exports
        if (match(TokenType::LeftBrace)) {
            // Named exports: export { add, subtract }
            std::vector<ExportSpec> namedExports = parseNamedExports();
            return ExportClause(ExportClause::Named, namedExports);
        }

        // Check for re-export all
        if (match(TokenType::Star)) {
            // Re-export all: export * from "./math"
            return ExportClause(ExportClause::All, {});
        }

        // If we get here, it's an export declaration (export function add() { })
        // Parse the declaration and create a default export
        if (check(TokenType::Function)) {
            // Export function declaration
            std::cout << "DEBUG: Parsing export function declaration" << std::endl;
            auto functionDecl = parseFunctionDeclaration();
            return ExportClause(ExportClause::Default, {}, std::move(functionDecl));
        } else if (check(TokenType::Class)) {
            // Export class declaration
            auto classDecl = parseClassDeclaration();
            return ExportClause(ExportClause::Default, {}, std::move(classDecl));
        } else if (check(TokenType::Interface)) {
            // Export interface declaration
            auto interfaceDecl = parseInterfaceDeclaration();
            return ExportClause(ExportClause::Default, {}, std::move(interfaceDecl));
        } else if (check(TokenType::Const) || check(TokenType::Let) || check(TokenType::Var)) {
            // Export variable declaration
            // Consume the keyword first
            Token keyword = advance();
            auto varDecl = parseVariableStatement();
            return ExportClause(ExportClause::Default, {}, std::move(varDecl));
        } else {
            // Export expression or other declaration
            unique_ptr<Expression> expr = parseExpression();
            return ExportClause(ExportClause::Default, {}, std::move(expr));
        }
    }

    std::vector<ExportSpec> Parser::parseNamedExports() {
        std::vector<ExportSpec> namedExports;

        do {
            // Parse export spec
            Token localToken = consume(TokenType::Identifier, "Expected local name");
            String localName = localToken.getStringValue();
            String exportedName = localName; // Default to same name

            // Check for 'as' clause
            if (match(TokenType::As)) {
                Token exportedToken = consume(TokenType::Identifier, "Expected exported name after 'as'");
                exportedName = exportedToken.getStringValue();
            }

            namedExports.emplace_back(localName, exportedName);
        } while (match(TokenType::Comma));

        consume(TokenType::RightBrace, "Expected '}' after named exports");

        return namedExports;
    }

    String Parser::parseModuleSpecifier() {
        // Module specifier should be a string literal
        Token specifierToken = consume(TokenType::StringLiteral, "Expected module specifier");
        return specifierToken.getStringValue();
    }

    unique_ptr<Statement> Parser::parseBlockStatement() {
        std::vector<unique_ptr<Statement> > statements;

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
                    case TokenType::Var: kind = VariableDeclaration::Kind::Var;
                        break;
                    case TokenType::Let: kind = VariableDeclaration::Kind::Let;
                        break;
                    case TokenType::Const: kind = VariableDeclaration::Kind::Const;
                        break;
                    default: kind = VariableDeclaration::Kind::Let;
                        break; // fallback
                }

                auto init = make_unique<VariableDeclaration>(kind, varName, std::move(initializer), typeAnnotation,
                                                             varToken.getLocation());

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

        std::vector<unique_ptr<CaseClause> > cases;
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
            errorReporter_->reportExpectedToken(getCurrentLocation(), "'case' or 'default'", "switch case",
                                                "Add 'case value:' or 'default:'");
            return nullptr;
        }

        // Parse statements until next case/default or end of block
        std::vector<unique_ptr<Statement> > statements;
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
        auto tryBlock = std::unique_ptr<BlockStatement>(static_cast<BlockStatement *>(parseBlockStatement().release()));

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
            auto catchBody = std::unique_ptr<BlockStatement>(
                static_cast<BlockStatement *>(parseBlockStatement().release()));

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
            finallyBlock = std::unique_ptr<BlockStatement>(
                static_cast<BlockStatement *>(parseBlockStatement().release()));
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

    bool Parser::isArrayAssignment(Expression* expr) const {
        // Check if the expression is an IndexExpression (array access)
        return dynamic_cast<IndexExpression*>(expr) != nullptr;
    }

    unique_ptr<Expression> Parser::parseArrayAssignmentExpression() {
        // Parse the array expression (e.g., "array")
        auto array = parseConditionalExpression();
        
        // Check if this is followed by array access syntax
        if (check(TokenType::LeftBracket)) {
            advance(); // consume '['
            auto index = parseExpression();
            consume(TokenType::RightBracket, "Expected ']' after array index");
            
            // Check if this is followed by assignment
            if (check(TokenType::Equal)) {
                advance(); // consume '='
                auto value = parseAssignmentExpression();
                
                return make_unique<ArrayAssignmentExpression>(
                    std::move(array),
                    std::move(index),
                    std::move(value),
                    getCurrentLocation()
                );
            } else {
                // This is just array access, not assignment
                // We need to reconstruct the IndexExpression
                return make_unique<IndexExpression>(
                    std::move(array),
                    std::move(index),
                    getCurrentLocation()
                );
            }
        }
        
        return array;
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
            check(TokenType::Exclamation) || check(TokenType::PlusPlus) ||
            check(TokenType::MinusMinus)) {
            Token opToken = advance();
            auto operand = parseUnaryExpression();

            return make_unique<UnaryExpression>(
                tokenToUnaryOperator(opToken.getType(), true),
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

                // Check if this is followed by an assignment operator
                if (check(TokenType::Equal) || check(TokenType::PlusEqual) || check(TokenType::MinusEqual)) {
                    // This is an array assignment: array[index] = value
                    Token opToken = advance();
                    auto value = parseAssignmentExpression();
                    
                    return make_unique<ArrayAssignmentExpression>(
                        std::move(expr),
                        std::move(index),
                        std::move(value),
                        opToken.getLocation()
                    );
                } else {
                    // This is just array access
                    SourceLocation location = getCurrentLocation();
                    expr = make_unique<IndexExpression>(
                        std::move(expr),
                        std::move(index),
                        location
                    );
                }
            } else if (check(TokenType::QuestionDot)) {
                advance(); // consume '?.'
                if (peek().getType() == TokenType::LeftBracket) {
                    // Parse optional index access: expr?.[index]
                    consume(TokenType::LeftBracket, "Expected '[' after '?.'");

                    auto index = parseExpression();
                    consume(TokenType::RightBracket, "Expected ']' after optional array index");

                    SourceLocation location = getCurrentLocation();
                    expr = make_unique<OptionalIndexAccess>(
                        std::move(expr),
                        std::move(index),
                        location
                    );
                } else if (peek().getType() == TokenType::LeftParen) {
                    // Parse optional function call: expr?.method()

                    // Parse optional type arguments if present
                    std::vector<shared_ptr<Type> > typeArguments;
                    if (check(TokenType::Less) && isTypeArgumentList()) {
                        typeArguments = parseTypeArgumentList();
                    }

                    // Parse arguments
                    consume(TokenType::LeftParen, "Expected '(' after '?.'");

                    std::vector<unique_ptr<Expression> > arguments;
                    if (!check(TokenType::RightParen)) {
                        do {
                            auto arg = parseExpression();
                            if (!arg) {
                                reportError("Expected expression in optional function call argument",
                                            getCurrentLocation());
                                return nullptr;
                            }
                            arguments.push_back(std::move(arg));
                        } while (match(TokenType::Comma));
                    }

                    consume(TokenType::RightParen, "Expected ')' after optional function call arguments");

                    SourceLocation location = getCurrentLocation();
                    if (typeArguments.empty()) {
                        expr = make_unique<OptionalCallExpr>(
                            std::move(expr),
                            std::move(arguments),
                            location
                        );
                    } else {
                        expr = make_unique<OptionalCallExpr>(
                            std::move(expr),
                            std::move(typeArguments),
                            std::move(arguments),
                            location
                        );
                    }
                } else {
                    // Parse optional property access: expr?.property
                    if (!check(TokenType::Identifier)) {
                        reportError("Expected property name after '?.'", getCurrentLocation());
                        return nullptr;
                    }
                    Token propertyToken = advance();
                    SourceLocation location = getCurrentLocation();
                    expr = make_unique<OptionalPropertyAccess>(
                        std::move(expr),
                        propertyToken.getStringValue(),
                        location
                    );
                }
            } else if (check(TokenType::LeftParen) || (check(TokenType::Less) && isTypeArgumentList())) {
                // Parse function call with optional type arguments: expr<T>(args...) or expr(args...)
                std::vector<shared_ptr<Type> > typeArguments;

                // Parse type arguments if present
                if (check(TokenType::Less)) {
                    typeArguments = parseTypeArgumentList();
                }

                // Now parse the function call
                consume(TokenType::LeftParen, "Expected '(' after function name or type arguments");
                std::vector<unique_ptr<Expression> > arguments;

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
            } else if (match(TokenType::Dot)) {
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
            } else if (check(TokenType::QuestionDot)) {
                // Parse optional property access: expr?.property
                std::cout << "DEBUG: Parsing optional property access" << std::endl;
                advance(); // consume '?.'
                if (!check(TokenType::Identifier)) {
                    reportError("Expected property name after '?.'", getCurrentLocation());
                    return nullptr;
                }
                Token propertyToken = advance();
                SourceLocation location = getCurrentLocation();
                expr = make_unique<OptionalPropertyAccess>(
                    std::move(expr),
                    propertyToken.getStringValue(),
                    location
                );
            } else if (check(TokenType::PlusPlus) || check(TokenType::MinusMinus)) {
                // Parse postfix increment/decrement: expr++ or expr--
                TokenType opType = peek().getType(); // Get the token type before consuming
                advance(); // consume the operator
                SourceLocation location = getCurrentLocation();
                expr = make_unique<UnaryExpression>(
                    tokenToUnaryOperator(opType, false),
                    std::move(expr),
                    location,
                    false // postfix
                );
            } else {
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
        std::vector<unique_ptr<Expression> > elements;

        // Handle empty array
        if (match(TokenType::RightBracket)) {
            return make_unique<ArrayLiteral>(std::move(elements), location);
        }

        // Parse elements
        do {
            if (check(TokenType::RightBracket)) {
                break; // Trailing comma case: [1, 2, ]
            }

            // Check for spread element: ...expr
            if (match(TokenType::DotDotDot)) {
                auto expression = parseExpression();
                if (expression) {
                    elements.push_back(make_unique<SpreadElement>(std::move(expression), getCurrentLocation()));
                }
            } else {
                auto element = parseExpression();
                if (element) {
                    elements.push_back(std::move(element));
                }
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

            // Check for spread element: ...expr
            if (match(TokenType::DotDotDot)) {
                auto expression = parseExpression();
                if (expression) {
                    // Create a SpreadElement for object spread
                    auto spreadElement = make_unique<SpreadElement>(std::move(expression), getCurrentLocation());
                    // Store the SpreadElement as a property with special key "..."
                    properties.emplace_back("...", std::move(spreadElement), getCurrentLocation());
                }
            } else {
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
            }
        } while (match(TokenType::Comma));

        consume(TokenType::RightBrace, "Expected '}' after object properties");

        return make_unique<ObjectLiteral>(std::move(properties), location);
    }

    unique_ptr<Expression> Parser::parseNewExpression() {
        SourceLocation location = getCurrentLocation();

        // Parse the constructor expression (e.g., "Person", "MyClass")
        auto constructor = parsePrimaryExpression();

        // Parse optional type arguments (e.g., "<number>", "<string, boolean>")
        std::vector<shared_ptr<Type> > typeArguments;
        if (check(TokenType::Less)) {
            std::cout << "DEBUG: Parser found type arguments in NewExpression" << std::endl;
            // Use the existing type argument parsing infrastructure
            typeArguments = parseTypeArgumentList();
            std::cout << "DEBUG: Parsed " << typeArguments.size() << " type arguments" << std::endl;
        } else {
            std::cout << "DEBUG: Parser did not find type arguments in NewExpression" << std::endl;
        }

        // Parse arguments if present
        std::vector<unique_ptr<Expression> > arguments;
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
        for (auto type: types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    Token Parser::consume(TokenType type, const String &errorMessage) {
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
            case TokenType::StarStar: return BinaryExpression::Operator::Power;
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
            case TokenType::QuestionQuestion: return BinaryExpression::Operator::NullishCoalescing;
            default:
                return BinaryExpression::Operator::Add;
        }
    }

    UnaryExpression::Operator Parser::tokenToUnaryOperator(TokenType type, bool isPrefix) const {
        switch (type) {
            case TokenType::Plus: return UnaryExpression::Operator::Plus;
            case TokenType::Minus: return UnaryExpression::Operator::Minus;
            case TokenType::Exclamation: return UnaryExpression::Operator::LogicalNot;
            case TokenType::PlusPlus:
                return isPrefix ? UnaryExpression::Operator::PreIncrement : UnaryExpression::Operator::PostIncrement;
            case TokenType::MinusMinus:
                return isPrefix ? UnaryExpression::Operator::PreDecrement : UnaryExpression::Operator::PostDecrement;
            default:
                return UnaryExpression::Operator::Plus;
        }
    }

    void Parser::reportError(const String &message, const SourceLocation &location,
                             const String &context, const String &suggestion) {
        SourceLocation loc = location.isValid() ? location : getCurrentLocation();

        // Use enhanced error reporting system
        if (errorReporter_) {
            // Determine error code based on context
            String errorCode = ErrorCodes::Syntax::INVALID_EXPRESSION; // Default

            if (context.find("token") != String::npos) {
                errorCode = ErrorCodes::Syntax::UNEXPECTED_TOKEN;
            } else if (context.find("semicolon") != String::npos) {
                errorCode = ErrorCodes::Syntax::MISSING_SEMICOLON;
            } else if (context.find("brace") != String::npos) {
                errorCode = ErrorCodes::Syntax::MISSING_BRACE;
            } else if (context.find("parenthesis") != String::npos) {
                errorCode = ErrorCodes::Syntax::MISSING_PARENTHESIS;
            } else if (context.find("bracket") != String::npos) {
                errorCode = ErrorCodes::Syntax::MISSING_BRACKET;
            } else if (context.find("declaration") != String::npos) {
                errorCode = ErrorCodes::Syntax::INVALID_DECLARATION;
            } else if (context.find("statement") != String::npos) {
                errorCode = ErrorCodes::Syntax::INVALID_STATEMENT;
            }

            errorReporter_->reportSyntaxError(errorCode, loc, message, suggestion);
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

    void Parser::reportWarning(const String &message, const SourceLocation &location,
                               const String &context, const String &suggestion) {
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

    void Parser::reportInfo(const String &message, const SourceLocation &location,
                            const String &context) {
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
        // The parser's current position is at lookaheadCache_.currentIndex_
        // We need to cache tokens starting from this position
        while (lookaheadCache_.tokens_.size() <= offset) {
            if (tokens_->isAtEnd()) break;
            // Calculate the offset from the token stream's current position
            size_t streamOffset = lookaheadCache_.currentIndex_ + lookaheadCache_.tokens_.size();
            lookaheadCache_.tokens_.push_back(
                tokens_->peekAhead(streamOffset)
            );
        }
        if (offset < lookaheadCache_.tokens_.size()) {
            return lookaheadCache_.tokens_[offset];
        }
        return Token(TokenType::EndOfInput, SourceLocation());
    }

    bool Parser::hasAhead(size_t offset) const {
        // Check if we have enough tokens cached
        if (offset < lookaheadCache_.tokens_.size()) {
            return true;
        }
        // Check if the token stream has more tokens at the required offset
        size_t streamOffset = lookaheadCache_.currentIndex_ + offset;
        return !tokens_->isAtEnd() && streamOffset < 1000; // Conservative check
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
        auto firstType = parseIntersectionType();
        if (!firstType) {
            return nullptr;
        }

        // Check if this is a union type (has '|' operator)
        std::vector<shared_ptr<Type> > unionTypes;
        unionTypes.push_back(firstType);

        while (match(TokenType::Pipe)) {
            auto nextType = parseIntersectionType();
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

    shared_ptr<Type> Parser::parseIntersectionType() {
        auto firstType = parsePrimaryType();
        if (!firstType) {
            return nullptr;
        }

        // Check if this is an intersection type (has '&' operator)
        std::vector<shared_ptr<Type> > intersectionTypes;
        intersectionTypes.push_back(firstType);

        while (match(TokenType::Ampersand)) {
            auto nextType = parsePrimaryType();
            if (!nextType) {
                reportError("Expected type after '&' in intersection type", getCurrentLocation());
                return typeSystem_.getErrorType();
            }
            intersectionTypes.push_back(nextType);
        }

        // If we only have one type, return it directly
        if (intersectionTypes.size() == 1) {
            return intersectionTypes[0];
        }

        // Create intersection type
        return typeSystem_.createIntersectionType(std::move(intersectionTypes));
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

    shared_ptr<Type> Parser::parseTupleType() {
        SourceLocation location = getCurrentLocation();
        advance(); // consume '['

        std::vector<shared_ptr<Type> > elementTypes;

        // Handle empty tuple: []
        if (check(TokenType::RightBracket)) {
            advance(); // consume ']'
            return typeSystem_.createTupleType(std::move(elementTypes));
        }

        // Parse tuple elements
        do {
            if (check(TokenType::RightBracket)) {
                break; // Trailing comma case: [string, number, ]
            }

            auto elementType = parseUnionType();
            if (!elementType) {
                reportError("Expected type in tuple element", getCurrentLocation());
                return typeSystem_.getErrorType();
            }

            elementTypes.push_back(elementType);
        } while (match(TokenType::Comma));

        consume(TokenType::RightBracket, "Expected ']' after tuple elements");

        return typeSystem_.createTupleType(std::move(elementTypes));
    }

    shared_ptr<Type> Parser::parseObjectType() {
        SourceLocation location = getCurrentLocation();
        advance(); // consume '{'

        std::vector<ObjectType::Property> properties;

        // Handle empty object type: {}
        if (check(TokenType::RightBrace)) {
            advance(); // consume '}'
            return typeSystem_.createObjectType(std::move(properties));
        }

        // Parse object properties
        do {
            if (check(TokenType::RightBrace)) {
                break; // Trailing comma case: { name: string; age: number; }
            }

            // Parse property modifiers (readonly, optional)
            bool readonly = false;
            bool optional = false;

            if (match(TokenType::Readonly)) {
                readonly = true;
            }

            // Parse property name
            String propertyName;
            if (check(TokenType::Identifier)) {
                Token nameToken = advance();
                propertyName = nameToken.getStringValue();
            } else if (check(TokenType::StringLiteral)) {
                Token nameToken = advance();
                propertyName = nameToken.getStringValue();
            } else {
                reportError("Expected property name in object type", getCurrentLocation());
                return typeSystem_.getErrorType();
            }

            // Check for optional property
            if (match(TokenType::Question)) {
                optional = true;
            }

            // Expect colon
            consume(TokenType::Colon, "Expected ':' after property name");

            // Parse property type
            auto propertyType = parseUnionType();
            if (!propertyType) {
                reportError("Expected type after ':' in object type property", getCurrentLocation());
                return typeSystem_.getErrorType();
            }

            // Create property
            ObjectType::Property property(propertyName, propertyType, optional, readonly);
            properties.push_back(std::move(property));
        } while (match(TokenType::Semicolon) || match(TokenType::Comma));

        consume(TokenType::RightBrace, "Expected '}' after object type properties");

        return typeSystem_.createObjectType(std::move(properties));
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

                std::vector<shared_ptr<Type> > typeArguments;

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

        // Handle tuple types like "[string, number]"
        if (check(TokenType::LeftBracket)) {
            return parseTupleType();
        }

        // Handle object types like "{ name: string; age: number }"
        if (check(TokenType::LeftBrace)) {
            return parseObjectType();
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

        std::vector<unique_ptr<Statement> > statements;

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

    std::vector<unique_ptr<TypeParameter> > Parser::parseTypeParameterList() {
        std::vector<unique_ptr<TypeParameter> > typeParameters;

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

    std::vector<shared_ptr<Type> > Parser::parseTypeArgumentList() {
        std::vector<shared_ptr<Type> > typeArguments;

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
            if (!match(TokenType::Arrow)) {
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
                std::vector<unique_ptr<Statement> > statements;
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
        // Pattern 1: identifier => (single parameter)
        if (check(TokenType::Identifier)) {
            // Look ahead to see if there's an arrow after the identifier
            Token nextToken = peekAhead(1);
            return nextToken.getType() == TokenType::Arrow;
        }

        // Pattern 2: (params) => 
        if (check(TokenType::LeftParen)) {
            // Look ahead to find the matching closing paren and check for arrow
            size_t offset = 1;
            int parenCount = 1;

            while (hasAhead(offset) && parenCount > 0) {
                Token token = peekAhead(offset);
                if (token.getType() == TokenType::LeftParen) {
                    parenCount++;
                } else if (token.getType() == TokenType::RightParen) {
                    parenCount--;
                }
                offset++;
            }

            // Check if there's an arrow after the closing paren
            if (parenCount == 0 && hasAhead(offset)) {
                Token nextToken = peekAhead(offset);
                return nextToken.getType() == TokenType::Arrow;
            }
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

        return make_unique<FunctionExpression>(functionName, std::move(parameters), std::move(body), returnType,
                                               location);
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
        // Simplified approach: For now, assume any < followed by an identifier or type keyword is a type argument
        // This is a conservative approach that should work for basic cases
        
        // Bypass the broken lookahead cache and directly use the token stream
        // We're currently at the < token, so look ahead 1 position for the identifier
        Token nextToken = tokens_->peekAhead(1);
        
        // Check if the next token is an identifier or type keyword (type name)
        TokenType tokenType = nextToken.getType();
        if (tokenType != TokenType::Identifier && 
            tokenType != TokenType::String && 
            tokenType != TokenType::Number && 
            tokenType != TokenType::Boolean && 
            tokenType != TokenType::Any && 
            tokenType != TokenType::Unknown && 
            tokenType != TokenType::Object) {
            return false;
        }
        
        // For now, assume this is a type argument list
        // A more sophisticated implementation would check for > or , after the identifier
        return true;
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
    unique_ptr<Parser> createParser(DiagnosticEngine &diagnostics, const TypeSystem &typeSystem) {
        return make_unique<Parser>(diagnostics, typeSystem);
    }

    unique_ptr<Parser> createEnhancedParser(utils::EnhancedDiagnosticEngine &diagnostics,
                                            const TypeSystem &typeSystem) {
        return make_unique<Parser>(diagnostics, typeSystem);
    }

    // Destructuring assignment parsing methods
    unique_ptr<Statement> Parser::parseDestructuringVariableStatement() {
        SourceLocation location = getCurrentLocation();

        // Parse the destructuring pattern
        auto pattern = parseDestructuringPattern();
        if (!pattern) {
            reportError("Expected destructuring pattern", getCurrentLocation());
            return nullptr;
        }

        // Optional type annotation
        shared_ptr<Type> typeAnnotation = nullptr;
        if (match(TokenType::Colon)) {
            ParsingContext oldContext = currentContext_;
            setContext(ParsingContext::Type);
            typeAnnotation = parseUnionType();
            setContext(oldContext);
        }

        // Required initializer for destructuring
        if (!match(TokenType::Equal)) {
            reportError("Expected '=' after destructuring pattern", getCurrentLocation());
            return nullptr;
        }

        auto initializer = parseExpression();
        if (!initializer) {
            reportError("Expected expression after '=' in destructuring assignment", getCurrentLocation());
            return nullptr;
        }

        consume(TokenType::Semicolon, "Expected ';' after destructuring assignment");

        // Create a destructuring assignment expression
        auto destructuringAssignment = make_unique<DestructuringAssignment>(
            std::move(pattern), std::move(initializer), location
        );

        // Wrap in expression statement
        return make_unique<ExpressionStatement>(std::move(destructuringAssignment), location);
    }

    unique_ptr<DestructuringPattern> Parser::parseDestructuringPattern() {
        if (check(TokenType::LeftBracket)) {
            return parseArrayDestructuringPattern();
        } else if (check(TokenType::LeftBrace)) {
            return parseObjectDestructuringPattern();
        } else if (check(TokenType::Identifier)) {
            return parseIdentifierPattern();
        } else {
            reportError("Expected destructuring pattern", getCurrentLocation());
            return nullptr;
        }
    }

    unique_ptr<DestructuringPattern> Parser::parseArrayDestructuringPattern() {
        SourceLocation location = getCurrentLocation();
        advance(); // consume '['

        std::vector<unique_ptr<DestructuringPattern> > elements;

        // Handle empty array: []
        if (check(TokenType::RightBracket)) {
            advance(); // consume ']'
            return make_unique<ArrayDestructuringPattern>(std::move(elements), location);
        }

        // Parse array elements
        do {
            if (check(TokenType::RightBracket)) {
                break; // Trailing comma case: [a, b, ]
            }

            // Check for spread element: ...rest
            if (match(TokenType::DotDotDot)) {
                auto restPattern = parseDestructuringPattern();
                if (restPattern) {
                    // For now, we'll treat spread as a regular element
                    // In a full implementation, this would need special handling
                    elements.push_back(std::move(restPattern));
                }
            } else {
                // Parse the pattern element
                auto element = parseDestructuringPattern();
                if (element) {
                    // Check if this is an identifier pattern with a default value
                    if (auto identifierPattern = dynamic_cast<IdentifierPattern *>(element.get())) {
                        // Check for default value: [a = default]
                        if (match(TokenType::Equal)) {
                            auto defaultValue = parseExpression();
                            if (!defaultValue) {
                                reportError("Expected default value after '='", getCurrentLocation());
                                return nullptr;
                            }
                            // Create a new IdentifierPattern with the default value
                            auto newPattern = make_unique<IdentifierPattern>(
                                identifierPattern->getName(),
                                std::move(defaultValue),
                                identifierPattern->getLocation()
                            );
                            elements.push_back(std::move(newPattern));
                        } else {
                            elements.push_back(std::move(element));
                        }
                    } else {
                        elements.push_back(std::move(element));
                    }
                }
            }
        } while (match(TokenType::Comma));

        consume(TokenType::RightBracket, "Expected ']' after array destructuring pattern");

        return make_unique<ArrayDestructuringPattern>(std::move(elements), location);
    }

    unique_ptr<DestructuringPattern> Parser::parseObjectDestructuringPattern() {
        SourceLocation location = getCurrentLocation();
        advance(); // consume '{'

        std::vector<ObjectDestructuringPattern::Property> properties;

        // Handle empty object: {}
        if (check(TokenType::RightBrace)) {
            advance(); // consume '}'
            return make_unique<ObjectDestructuringPattern>(std::move(properties), location);
        }

        // Parse object properties
        do {
            if (check(TokenType::RightBrace)) {
                break; // Trailing comma case: { a, b, }
            }

            // Parse property key (identifier or string literal)
            String key;
            if (check(TokenType::Identifier)) {
                Token keyToken = advance();
                key = keyToken.getStringValue();
            } else if (check(TokenType::StringLiteral)) {
                Token keyToken = advance();
                key = keyToken.getStringValue();
            } else {
                reportError("Expected property name in object destructuring pattern", getCurrentLocation());
                return nullptr;
            }

            // Parse pattern (can be identifier or nested destructuring)
            unique_ptr<DestructuringPattern> pattern = nullptr;

            if (match(TokenType::Colon)) {
                // Shorthand: { a: b } - parse the pattern after colon
                pattern = parseDestructuringPattern();
                if (!pattern) {
                    reportError("Expected destructuring pattern after ':'", getCurrentLocation());
                    return nullptr;
                }
            } else {
                // Shorthand: { a } - same as { a: a }
                pattern = make_unique<IdentifierPattern>(key, getCurrentLocation());
            }

            // Parse default value: { a = default }
            unique_ptr<Expression> defaultValue = nullptr;
            if (match(TokenType::Equal)) {
                defaultValue = parseExpression();
                if (!defaultValue) {
                    reportError("Expected default value after '='", getCurrentLocation());
                    return nullptr;
                }
            }

            properties.emplace_back(key, std::move(pattern), std::move(defaultValue), getCurrentLocation());
        } while (match(TokenType::Comma));

        consume(TokenType::RightBrace, "Expected '}' after object destructuring pattern");

        return make_unique<ObjectDestructuringPattern>(std::move(properties), location);
    }

    unique_ptr<DestructuringPattern> Parser::parseIdentifierPattern() {
        Token token = consume(TokenType::Identifier, "Expected identifier in destructuring pattern");
        return make_unique<IdentifierPattern>(token.getStringValue(), token.getLocation());
    }


    shared_ptr<Type> Parser::parseObjectTypeLiteral() {
        SourceLocation location = getCurrentLocation();
        std::vector<ObjectType::Property> properties;
        
        if (!check(TokenType::RightBrace)) {
            do {
                // Parse property name
                String propertyName;
                bool isOptional = false;
                
                if (check(TokenType::Identifier)) {
                    Token nameToken = advance();
                    propertyName = nameToken.getStringValue();
                } else if (check(TokenType::StringLiteral)) {
                    Token nameToken = advance();
                    propertyName = nameToken.getStringValue();
                } else {
                    throw std::runtime_error("Expected property name in object type");
                }
                
                // Check for optional property
                if (match(TokenType::Question)) {
                    isOptional = true;
                }
                
                consume(TokenType::Colon, "Expected ':' after property name");
                
                // Parse property type
                shared_ptr<Type> propertyType = parseTypeAnnotation();
                
                properties.emplace_back(propertyName, propertyType, isOptional);
                
            } while (match(TokenType::Comma));
        }
        
        consume(TokenType::RightBrace, "Expected '}' after object type");
        
        // Create object type - this would need to be implemented in TypeSystem
        // For now, return a placeholder type
        return typeSystem_.createObjectType(properties);
    }


    shared_ptr<Type> tsc::Parser::parseFunctionType() {
        consume(TokenType::LeftParen, "Expected '(' after 'function'");
        
        std::vector<FunctionType::Parameter> parameters;
        
        if (!check(TokenType::RightParen)) {
            do {
                String paramName;
                if (check(TokenType::Identifier)) {
                    Token nameToken = advance();
                    paramName = nameToken.getStringValue();
                }
                
                consume(TokenType::Colon, "Expected ':' after parameter name");
                shared_ptr<Type> paramType = parseTypeAnnotation();
                
                parameters.emplace_back(paramName, paramType, false);
                
            } while (match(TokenType::Comma));
        }
        
        consume(TokenType::RightParen, "Expected ')' after function parameters");
        consume(TokenType::Arrow, "Expected '=>' after function parameters");
        
        shared_ptr<Type> returnType = parseTypeAnnotation();
        
        return typeSystem_.createFunctionType(parameters, returnType);
    }
} // namespace tsc

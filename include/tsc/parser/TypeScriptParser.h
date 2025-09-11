#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {

class Parser;
class DiagnosticEngine;

// Specialized parser for TypeScript-specific features
class TypeScriptParser {
public:
    explicit TypeScriptParser(Parser& parentParser, DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
    ~TypeScriptParser() = default;
    
    // Type parsing methods
    shared_ptr<Type> parseTypeAnnotation();
    shared_ptr<Type> parseUnionType();
    shared_ptr<Type> parseIntersectionType();
    shared_ptr<Type> parseConditionalType();
    shared_ptr<Type> parseMappedType();
    shared_ptr<Type> parseTemplateLiteralType();
    shared_ptr<Type> parsePrimaryType();
    shared_ptr<Type> parseTupleType();
    shared_ptr<Type> parseObjectType();
    shared_ptr<Type> parseFunctionType();
    shared_ptr<Type> parseConstructorType();
    
    // Type parameter parsing
    std::vector<unique_ptr<TypeParameter>> parseTypeParameterList();
    unique_ptr<TypeParameter> parseTypeParameter();
    std::vector<shared_ptr<Type>> parseTypeArgumentList();
    
    // Type-specific declarations
    unique_ptr<Statement> parseInterfaceDeclaration();
    unique_ptr<Statement> parseTypeAliasDeclaration();
    unique_ptr<Statement> parseEnumDeclaration();
    
    // Type guards and assertions
    unique_ptr<Expression> parseTypeAssertion();
    unique_ptr<Expression> parseAsExpression();
    
    // Template literal types
    shared_ptr<Type> parseTemplateLiteralType();
    
private:
    Parser& parentParser_;
    DiagnosticEngine& diagnostics_;
    const TypeSystem& typeSystem_;
    
    // Helper methods
    bool isTypeToken(TokenType type) const;
    bool isTypeStart(TokenType type) const;
    shared_ptr<Type> parseTypeReference();
    shared_ptr<Type> parseArrayType();
    shared_ptr<Type> parseIndexedAccessType();
    shared_ptr<Type> parseKeyofType();
    shared_ptr<Type> parseTypeofType();
    shared_ptr<Type> parseInferType();
};

} // namespace tsc
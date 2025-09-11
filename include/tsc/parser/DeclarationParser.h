#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {

class Parser;
class DiagnosticEngine;
class TypeSystem;

// Specialized parser for declarations
class DeclarationParser {
public:
    explicit DeclarationParser(Parser& parentParser, DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
    ~DeclarationParser() = default;
    
    // Declaration parsing methods
    unique_ptr<Statement> parseVariableStatement();
    unique_ptr<Statement> parseFunctionDeclaration();
    unique_ptr<Statement> parseClassDeclaration();
    unique_ptr<Statement> parseInterfaceDeclaration();
    unique_ptr<Statement> parseEnumDeclaration();
    unique_ptr<Statement> parseTypeAliasDeclaration();
    
    // Class-specific parsing
    std::vector<unique_ptr<MethodDeclaration>> parseClassMethods();
    unique_ptr<MethodDeclaration> parseMethodDeclaration();
    std::vector<unique_ptr<PropertyDeclaration>> parseClassProperties();
    unique_ptr<PropertyDeclaration> parsePropertyDeclaration();
    unique_ptr<ConstructorDeclaration> parseConstructorDeclaration();
    
    // Interface-specific parsing
    std::vector<unique_ptr<PropertyDeclaration>> parseInterfaceProperties();
    std::vector<unique_ptr<MethodDeclaration>> parseInterfaceMethods();
    
    // Enum-specific parsing
    std::vector<unique_ptr<EnumMember>> parseEnumMembers();
    unique_ptr<EnumMember> parseEnumMember();
    
    // Parameter parsing
    std::vector<FunctionDeclaration::Parameter> parseParameterList();
    std::vector<MethodDeclaration::Parameter> parseMethodParameterList();
    FunctionDeclaration::Parameter parseParameter();
    
    // Type parameter parsing
    std::vector<unique_ptr<TypeParameter>> parseTypeParameterList();
    unique_ptr<TypeParameter> parseTypeParameter();
    
private:
    Parser& parentParser_;
    DiagnosticEngine& diagnostics_;
    const TypeSystem& typeSystem_;
    
    // Helper methods
    bool isDeclarationStart(TokenType type) const;
    bool isClassMemberStart(TokenType type) const;
    bool isInterfaceMemberStart(TokenType type) const;
    bool isEnumMemberStart(TokenType type) const;
    
    // Modifier parsing
    bool parseAccessibilityModifier(String& accessibility);
    bool parseStaticModifier();
    bool parseReadonlyModifier();
    bool parseAbstractModifier();
    bool parseAsyncModifier();
    bool parseOverrideModifier();
};

} // namespace tsc
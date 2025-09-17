#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {
namespace parser {

// Forward declarations
class ParserCore;

// Declaration parsing engine - handles all declaration types
class DeclarationParser {
public:
    explicit DeclarationParser(ParserCore& parser);
    
    // Main declaration parsing interface
    std::unique_ptr<Statement> parseDeclaration();
    
    // Function declarations
    std::unique_ptr<Statement> parseFunctionDeclaration();
    std::unique_ptr<FunctionDeclaration> parseFunctionSignature();
    std::unique_ptr<BlockStatement> parseFunctionBody();
    
    // Class declarations
    std::unique_ptr<Statement> parseClassDeclaration();
    std::unique_ptr<ClassDeclaration> parseClassSignature();
    std::vector<std::unique_ptr<Statement>> parseClassMembers();
    std::unique_ptr<PropertyDeclaration> parsePropertyDeclaration();
    std::unique_ptr<MethodDeclaration> parseMethodDeclaration();
    std::unique_ptr<ConstructorDeclaration> parseConstructorDeclaration();
    std::unique_ptr<DestructorDeclaration> parseDestructorDeclaration();
    
    // Interface declarations
    std::unique_ptr<Statement> parseInterfaceDeclaration();
    std::unique_ptr<InterfaceDeclaration> parseInterfaceSignature();
    std::vector<std::unique_ptr<Statement>> parseInterfaceMembers();
    
    // Enum declarations
    std::unique_ptr<Statement> parseEnumDeclaration();
    std::unique_ptr<EnumDeclaration> parseEnumSignature();
    std::vector<std::unique_ptr<EnumMember>> parseEnumMembers();
    
    // Type alias declarations
    std::unique_ptr<Statement> parseTypeAliasDeclaration();
    std::unique_ptr<TypeAliasDeclaration> parseTypeAliasSignature();
    
    // Import/Export declarations
    std::unique_ptr<Statement> parseImportDeclaration();
    std::unique_ptr<Statement> parseExportDeclaration();
    
    // Variable declarations
    std::unique_ptr<VariableDeclaration> parseVariableDeclaration();
    std::vector<std::unique_ptr<VariableDeclaration>> parseVariableDeclarationList();

private:
    ParserCore& parser_;
    
    // Helper methods
    std::vector<std::unique_ptr<TypeParameter>> parseTypeParameters();
    std::vector<std::unique_ptr<TypeParameter>> parseTypeParameterList();
    std::unique_ptr<TypeParameter> parseTypeParameter();
    std::vector<std::unique_ptr<Parameter>> parseParameterList();
    std::unique_ptr<Parameter> parseParameter();
    std::vector<std::unique_ptr<Type>> parseTypeArguments();
    std::unique_ptr<Type> parseType();
    std::unique_ptr<Type> parseTypeAnnotation();
};

} // namespace parser
} // namespace tsc
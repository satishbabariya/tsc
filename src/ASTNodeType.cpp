#include "tsc/ASTNodeType.h"
#include <unordered_map>

namespace tsc {

const char* astNodeTypeToString(ASTNodeType type) {
    static const std::unordered_map<ASTNodeType, const char*> typeNames = {
        {ASTNodeType::ASTNode, "ASTNode"},
        {ASTNodeType::Expression, "Expression"},
        {ASTNodeType::Statement, "Statement"},
        {ASTNodeType::Declaration, "Declaration"},
        
        {ASTNodeType::NumericLiteral, "NumericLiteral"},
        {ASTNodeType::StringLiteral, "StringLiteral"},
        {ASTNodeType::BooleanLiteral, "BooleanLiteral"},
        {ASTNodeType::NullLiteral, "NullLiteral"},
        {ASTNodeType::TemplateLiteral, "TemplateLiteral"},
        
        {ASTNodeType::Identifier, "Identifier"},
        {ASTNodeType::ThisExpression, "ThisExpression"},
        
        {ASTNodeType::BinaryExpression, "BinaryExpression"},
        {ASTNodeType::UnaryExpression, "UnaryExpression"},
        {ASTNodeType::AssignmentExpression, "AssignmentExpression"},
        {ASTNodeType::ArrayAssignmentExpression, "ArrayAssignmentExpression"},
        
        {ASTNodeType::MemberExpression, "MemberExpression"},
        {ASTNodeType::CallExpression, "CallExpression"},
        
        {ASTNodeType::ArrayExpression, "ArrayExpression"},
        {ASTNodeType::ObjectLiteral, "ObjectLiteral"},
        {ASTNodeType::ObjectProperty, "ObjectProperty"},
        
        {ASTNodeType::IfStatement, "IfStatement"},
        {ASTNodeType::WhileStatement, "WhileStatement"},
        {ASTNodeType::DoWhileStatement, "DoWhileStatement"},
        {ASTNodeType::ForStatement, "ForStatement"},
        {ASTNodeType::ForOfStatement, "ForOfStatement"},
        {ASTNodeType::ForInStatement, "ForInStatement"},
        {ASTNodeType::SwitchStatement, "SwitchStatement"},
        {ASTNodeType::CaseClause, "CaseClause"},
        {ASTNodeType::DefaultClause, "DefaultClause"},
        
        {ASTNodeType::FunctionDeclaration, "FunctionDeclaration"},
        {ASTNodeType::ArrowFunctionExpression, "ArrowFunctionExpression"},
        {ASTNodeType::MethodDeclaration, "MethodDeclaration"},
        {ASTNodeType::ConstructorDeclaration, "ConstructorDeclaration"},
        {ASTNodeType::Parameter, "Parameter"},
        
        {ASTNodeType::VariableDeclaration, "VariableDeclaration"},
        {ASTNodeType::VariableDeclarator, "VariableDeclarator"},
        
        {ASTNodeType::BlockStatement, "BlockStatement"},
        {ASTNodeType::ExpressionStatement, "ExpressionStatement"},
        
        {ASTNodeType::ReturnStatement, "ReturnStatement"},
        {ASTNodeType::BreakStatement, "BreakStatement"},
        {ASTNodeType::ContinueStatement, "ContinueStatement"},
        
        {ASTNodeType::TryStatement, "TryStatement"},
        {ASTNodeType::CatchClause, "CatchClause"},
        {ASTNodeType::FinallyClause, "FinallyClause"},
        {ASTNodeType::ThrowStatement, "ThrowStatement"},
        {ASTNodeType::PanicStatement, "PanicStatement"},
        {ASTNodeType::AbortStatement, "AbortStatement"},
        {ASTNodeType::ErrorPropagationExpression, "ErrorPropagationExpression"},
        
        {ASTNodeType::DestructuringPattern, "DestructuringPattern"},
        {ASTNodeType::ArrayDestructuringPattern, "ArrayDestructuringPattern"},
        {ASTNodeType::ObjectDestructuringPattern, "ObjectDestructuringPattern"},
        {ASTNodeType::DestructuringProperty, "DestructuringProperty"},
        {ASTNodeType::RestElement, "RestElement"},
        {ASTNodeType::AssignmentPattern, "AssignmentPattern"},
        
        {ASTNodeType::ImportDeclaration, "ImportDeclaration"},
        {ASTNodeType::ExportDeclaration, "ExportDeclaration"},
        {ASTNodeType::ImportClause, "ImportClause"},
        {ASTNodeType::ExportClause, "ExportClause"},
        {ASTNodeType::ImportSpec, "ImportSpec"},
        {ASTNodeType::ExportSpec, "ExportSpec"},
        {ASTNodeType::Module, "Module"},
        
        {ASTNodeType::TypeAnnotation, "TypeAnnotation"},
        {ASTNodeType::TypeParameter, "TypeParameter"},
        {ASTNodeType::GenericDeclaration, "GenericDeclaration"},
        
        {ASTNodeType::Unknown, "Unknown"}
    };
    
    auto it = typeNames.find(type);
    return (it != typeNames.end()) ? it->second : "Unknown";
}

bool isExpressionType(ASTNodeType type) {
    switch (type) {
        case ASTNodeType::NumericLiteral:
        case ASTNodeType::StringLiteral:
        case ASTNodeType::BooleanLiteral:
        case ASTNodeType::NullLiteral:
        case ASTNodeType::TemplateLiteral:
        case ASTNodeType::Identifier:
        case ASTNodeType::ThisExpression:
        case ASTNodeType::BinaryExpression:
        case ASTNodeType::UnaryExpression:
        case ASTNodeType::AssignmentExpression:
        case ASTNodeType::ArrayAssignmentExpression:
        case ASTNodeType::MemberExpression:
        case ASTNodeType::CallExpression:
        case ASTNodeType::ArrayExpression:
        case ASTNodeType::ObjectLiteral:
        case ASTNodeType::ArrowFunctionExpression:
        case ASTNodeType::ErrorPropagationExpression:
            return true;
        default:
            return false;
    }
}

bool isStatementType(ASTNodeType type) {
    switch (type) {
        case ASTNodeType::IfStatement:
        case ASTNodeType::WhileStatement:
        case ASTNodeType::DoWhileStatement:
        case ASTNodeType::ForStatement:
        case ASTNodeType::ForOfStatement:
        case ASTNodeType::ForInStatement:
        case ASTNodeType::SwitchStatement:
        case ASTNodeType::CaseClause:
        case ASTNodeType::DefaultClause:
        case ASTNodeType::BlockStatement:
        case ASTNodeType::ExpressionStatement:
        case ASTNodeType::ReturnStatement:
        case ASTNodeType::BreakStatement:
        case ASTNodeType::ContinueStatement:
        case ASTNodeType::TryStatement:
        case ASTNodeType::CatchClause:
        case ASTNodeType::FinallyClause:
        case ASTNodeType::ThrowStatement:
        case ASTNodeType::PanicStatement:
        case ASTNodeType::AbortStatement:
            return true;
        default:
            return false;
    }
}

bool isDeclarationType(ASTNodeType type) {
    switch (type) {
        case ASTNodeType::FunctionDeclaration:
        case ASTNodeType::MethodDeclaration:
        case ASTNodeType::ConstructorDeclaration:
        case ASTNodeType::VariableDeclaration:
        case ASTNodeType::ImportDeclaration:
        case ASTNodeType::ExportDeclaration:
        case ASTNodeType::GenericDeclaration:
            return true;
        default:
            return false;
    }
}

} // namespace tsc
#!/usr/bin/env python3
"""
Script to add getNodeType() method to all AST node classes
"""

import re
import sys

def update_ast_file():
    """Update AST.h file to add getNodeType() methods"""
    
    # Read the file
    with open('/workspace/include/tsc/AST.h', 'r') as f:
        content = f.read()
    
    # Mapping of class names to ASTNodeType values
    class_to_type = {
        'NumericLiteral': 'ASTNodeType::NumericLiteral',
        'StringLiteral': 'ASTNodeType::StringLiteral',
        'BooleanLiteral': 'ASTNodeType::BooleanLiteral',
        'NullLiteral': 'ASTNodeType::NullLiteral',
        'TemplateLiteral': 'ASTNodeType::TemplateLiteral',
        'Identifier': 'ASTNodeType::Identifier',
        'ThisExpression': 'ASTNodeType::ThisExpression',
        'SuperExpression': 'ASTNodeType::ThisExpression',  # Map to ThisExpression for now
        'NewExpression': 'ASTNodeType::CallExpression',  # Map to CallExpression for now
        'BinaryExpression': 'ASTNodeType::BinaryExpression',
        'UnaryExpression': 'ASTNodeType::UnaryExpression',
        'AssignmentExpression': 'ASTNodeType::AssignmentExpression',
        'ConditionalExpression': 'ASTNodeType::BinaryExpression',  # Map to BinaryExpression for now
        'CallExpression': 'ASTNodeType::CallExpression',
        'OptionalCallExpr': 'ASTNodeType::CallExpression',  # Map to CallExpression for now
        'ArrayLiteral': 'ASTNodeType::ArrayExpression',
        'SpreadElement': 'ASTNodeType::ArrayExpression',  # Map to ArrayExpression for now
        'IndexExpression': 'ASTNodeType::MemberExpression',  # Map to MemberExpression for now
        'OptionalIndexAccess': 'ASTNodeType::MemberExpression',  # Map to MemberExpression for now
        'ObjectLiteral': 'ASTNodeType::ObjectLiteral',
        'PropertyAccess': 'ASTNodeType::MemberExpression',  # Map to MemberExpression for now
        'OptionalPropertyAccess': 'ASTNodeType::MemberExpression',  # Map to MemberExpression for now
        'ArrowFunction': 'ASTNodeType::ArrowFunctionExpression',
        'FunctionExpression': 'ASTNodeType::FunctionDeclaration',  # Map to FunctionDeclaration for now
        'MoveExpression': 'ASTNodeType::AssignmentExpression',  # Map to AssignmentExpression for now
        'DestructuringPattern': 'ASTNodeType::DestructuringPattern',
        'DestructuringAssignment': 'ASTNodeType::AssignmentExpression',  # Map to AssignmentExpression for now
    }
    
    # Pattern to match class definitions
    class_pattern = r'class\s+(\w+)\s*:\s*public\s+(\w+)\s*\{'
    
    def add_getNodeType(match):
        class_name = match.group(1)
        base_class = match.group(2)
        
        if class_name in class_to_type:
            node_type = class_to_type[class_name]
            return f'{match.group(0)}\n    // Add getNodeType() method\n    ASTNodeType getNodeType() const override {{ return {node_type}; }}'
        else:
            return match.group(0)
    
    # Apply the transformation
    updated_content = re.sub(class_pattern, add_getNodeType, content)
    
    # Write back to file
    with open('/workspace/include/tsc/AST.h', 'w') as f:
        f.write(updated_content)
    
    print("Updated AST.h with getNodeType() methods")

if __name__ == '__main__':
    update_ast_file()
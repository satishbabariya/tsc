#!/usr/bin/env python3

import os
import re
import sys

def fix_specific_errors(file_path):
    """Fix specific string concatenation errors"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Fix string concatenation with size() calls
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ ([^+]+)\.size\(\) \+ "([^"]*)"',
            r'TSC_LOG_DEBUG("\1" + std::to_string(\2.size()) + "\3"',
            content
        )
        
        # Fix string concatenation with ternary operators
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ \(([^?]+)\? "([^"]*)" : "([^"]*)"\) \+ "([^"]*)"',
            r'TSC_LOG_DEBUG("\1" + (\2 ? "\3" : "\4") + "\5"',
            content
        )
        
        # Fix string concatenation with exception messages
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ ([^,]+)\.what\(\)',
            r'TSC_LOG_DEBUG("\1" + std::string(\2.what())',
            content
        )
        
        # Fix string concatenation with pointers (convert to string)
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ ([^,]+)',
            r'TSC_LOG_DEBUG("\1" + std::string(typeid(\2).name())',
            content
        )
        
        # Fix string concatenation with TypeID
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ ([^,]+)\.getTypeID\(\)',
            r'TSC_LOG_DEBUG("\1" + std::to_string(static_cast<int>(\2.getTypeID()))',
            content
        )
        
        # Fix string concatenation with getTypeAllocSize
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ ([^+]+)\.getTypeAllocSize\([^)]+\) \+ "([^"]*)"',
            r'TSC_LOG_DEBUG("\1" + std::to_string(\2.getTypeAllocSize(\3)) + "\4"',
            content
        )
        
        # Fix string concatenation with boolean expressions
        content = re.sub(
            r'TSC_LOG_DEBUG\("([^"]*)" \+ \(([^?]+)\? "([^"]*)" : "([^"]*)"\)',
            r'TSC_LOG_DEBUG("\1" + (\2 ? "\3" : "\4")',
            content
        )
        
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"Fixed string errors in {file_path}")
            return True
        return False
        
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return False

def main():
    # Focus on the most problematic files first
    problematic_files = [
        "/workspace/src/codegen/LLVMCodeGen.cpp",
        "/workspace/src/semantic/SemanticAnalyzer.cpp",
        "/workspace/src/semantic/ModuleSymbolTable.cpp",
        "/workspace/src/semantic/DependencyScanner.cpp"
    ]
    
    fixed_count = 0
    for file_path in problematic_files:
        if os.path.exists(file_path):
            if fix_specific_errors(file_path):
                fixed_count += 1
    
    print(f"Fixed string errors in {fixed_count} files")

if __name__ == "__main__":
    main()
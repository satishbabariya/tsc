#!/usr/bin/env python3

import os
import re
import sys

def fix_logging_errors(file_path):
    """Fix string concatenation errors in TSC_LOG_DEBUG statements"""
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
        
        # Fix string concatenation with Scope* pointers
        content = re.sub(
            r'std::to_string\(([^)]*getCurrentScope\(\)[^)]*)\)',
            r'std::string(typeid(\1).name())',
            content
        )
        
        # Fix string concatenation with Type* pointers
        content = re.sub(
            r'std::to_string\(static_cast<int>\(([^)]*getType\(\)[^)]*)\)->getKind\(\)\)',
            r'std::to_string(static_cast<int>((\1)->getKind()))',
            content
        )
        
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"Fixed logging errors in {file_path}")
            return True
        return False
        
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return False

def main():
    src_dir = "/workspace/src"
    fixed_count = 0
    
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            if file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                if fix_logging_errors(file_path):
                    fixed_count += 1
    
    print(f"Fixed logging errors in {fixed_count} files")

if __name__ == "__main__":
    main()
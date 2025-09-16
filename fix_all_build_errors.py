#!/usr/bin/env python3
"""
Comprehensive script to fix all build errors in TSC codebase.
"""

import os
import re
import sys

def fix_build_errors(file_path):
    """Fix all build errors in a single file."""
    print(f"Processing {file_path}...")
    
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Fix missing Logger include
    if 'TSC_LOG_DEBUG' in content and '#include "tsc/utils/Logger.h"' not in content:
        # Find the last #include and add Logger include after it
        include_pattern = r'(#include[^\n]+\n)'
        includes = re.findall(include_pattern, content)
        if includes:
            last_include = includes[-1]
            content = content.replace(last_include, last_include + '#include "tsc/utils/Logger.h"\n')
    
    # Fix string concatenation issues
    # Fix static_cast<int> concatenations
    content = re.sub(r'\+ static_cast<int>\(([^)]+)\) \+', r' + std::to_string(static_cast<int>(\1))', content)
    content = re.sub(r'\+ static_cast<int>\(([^)]+)\)', r' + std::to_string(static_cast<int>(\1))', content)
    
    # Fix size() concatenations
    content = re.sub(r'\+ (\w+\.size\(\)) \+', r' + std::to_string(\1)', content)
    content = re.sub(r'\+ (\w+\.size\(\))', r' + std::to_string(\1)', content)
    
    # Fix pointer concatenations
    content = re.sub(r'\+ (\w+->getCurrentScope\(\)) \+', r' + std::to_string(\1)', content)
    content = re.sub(r'\+ (\w+->getCurrentScope\(\))', r' + std::to_string(\1)', content)
    content = re.sub(r'\+ (\w+\.get\(\)) \+', r' + std::to_string(\1)', content)
    content = re.sub(r'\+ (\w+\.get\(\))', r' + std::to_string(\1)', content)
    
    # Fix typeid concatenations
    content = re.sub(r'\+ typeid\([^)]+\)\.name\(\) \+', r' + std::string(typeid(*stmt.get()).name())', content)
    content = re.sub(r'\+ typeid\([^)]+\)\.name\(\)', r' + std::string(typeid(*stmt.get()).name())', content)
    
    # Fix enum concatenations
    content = re.sub(r'\+ (\w+\.getType\(\)) \+', r' + std::to_string(static_cast<int>(\1))', content)
    content = re.sub(r'\+ (\w+\.getType\(\))', r' + std::to_string(static_cast<int>(\1))', content)
    content = re.sub(r'\+ (\w+\.getKind\(\)) \+', r' + std::to_string(static_cast<int>(\1))', content)
    content = re.sub(r'\+ (\w+\.getKind\(\))', r' + std::to_string(static_cast<int>(\1))', content)
    
    # Fix ternary operator concatenations
    content = re.sub(r'\+ \(([^)]+) \? "([^"]+)" : "([^"]+)"\) \+', r' + (\1 ? "\2" : "\3")', content)
    content = re.sub(r'\+ \(([^)]+) \? "([^"]+)" : "([^"]+)"\)', r' + (\1 ? "\2" : "\3")', content)
    
    # Fix remaining empty string concatenations
    content = re.sub(r'\+\s*""\s*', '', content)
    content = re.sub(r'""\s*\+\s*', '', content)
    
    # Fix trailing commas and spaces
    content = re.sub(r',\s*"([^"]*)"\s*\)', r', "\1")', content)
    
    # Fix missing + operators in string concatenation
    content = re.sub(r'"([^"]+)"\s*"([^"]+)"', r'"\1" + "\2"', content)
    
    # Fix specific syntax errors
    content = re.sub(r'std::to_string\(static_cast<int>\(kind\)\) "\)', r'std::to_string(static_cast<int>(kind)) + ")"', content)
    
    # Check if any changes were made
    if content != original_content:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  Updated {file_path}")
        return True
    else:
        print(f"  No changes needed in {file_path}")
        return False

def main():
    """Main function to process all C++ files."""
    if len(sys.argv) > 1:
        # Process specific file
        file_path = sys.argv[1]
        if os.path.exists(file_path):
            fix_build_errors(file_path)
        else:
            print(f"File not found: {file_path}")
    else:
        # Process all C++ files in src directory
        src_dir = "/workspace/src"
        updated_files = 0
        
        for root, dirs, files in os.walk(src_dir):
            for file in files:
                if file.endswith('.cpp'):
                    file_path = os.path.join(root, file)
                    if fix_build_errors(file_path):
                        updated_files += 1
        
        print(f"\nProcessed {updated_files} files with build errors.")

if __name__ == "__main__":
    main()
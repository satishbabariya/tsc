#!/usr/bin/env python3
"""
Script to fix string concatenation issues in TSC codebase.
This script will fix common string concatenation problems:
1. Remove empty string concatenations (+ "")
2. Fix pointer concatenations
3. Fix number concatenations
"""

import os
import re
import sys

def fix_string_concat(file_path):
    """Fix string concatenation issues in a single file."""
    print(f"Processing {file_path}...")
    
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Fix empty string concatenations
    content = re.sub(r'\+\s*""\s*', '', content)
    content = re.sub(r'""\s*\+\s*', '', content)
    
    # Fix pointer concatenations (this + "")
    content = re.sub(r'\bthis\s*\+\s*""', 'this', content)
    content = re.sub(r'""\s*\+\s*\bthis', 'this', content)
    
    # Fix number concatenations (size() + "")
    content = re.sub(r'\.size\(\)\s*\+\s*""', '.size()', content)
    content = re.sub(r'""\s*\+\s*\.size\(\)', '.size()', content)
    
    # Fix specific patterns
    content = re.sub(r'\+ this \+', '+ this', content)
    content = re.sub(r'\+ symbolTable_\.get\(\) \+', '+ symbolTable_.get()', content)
    content = re.sub(r'\+ currentScope_ \+', '+ currentScope_', content)
    content = re.sub(r'\+ parent_ \+', '+ parent_', content)
    content = re.sub(r'\+ scope \+', '+ scope', content)
    
    # Fix number concatenations with std::to_string
    content = re.sub(r'\+ (\w+\.size\(\)) \+', r'+ std::to_string(\1) +', content)
    content = re.sub(r'\+ (\w+\.get\(\)) \+', r'+ std::to_string(\1) +', content)
    
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
            fix_string_concat(file_path)
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
                    if fix_string_concat(file_path):
                        updated_files += 1
        
        print(f"\nProcessed {updated_files} files with string concatenation issues.")

if __name__ == "__main__":
    main()
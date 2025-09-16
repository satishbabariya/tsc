#!/usr/bin/env python3
"""
Script to replace debug statements with proper logging in TSC codebase.
This script will:
1. Replace std::cout << "DEBUG: ..." with TSC_LOG_DEBUG(...)
2. Handle string concatenation properly
3. Add component names where appropriate
"""

import os
import re
import sys

def replace_debug_statements(file_path):
    """Replace debug statements in a single file."""
    print(f"Processing {file_path}...")
    
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Pattern to match std::cout << "DEBUG: ..." statements
    debug_pattern = r'std::cout\s*<<\s*"DEBUG:\s*([^"]+)"\s*<<\s*([^;]+);'
    
    def replace_debug(match):
        message = match.group(1).strip()
        rest = match.group(2).strip()
        
        # Determine component based on file path
        component = ""
        if "parser" in file_path:
            component = "Parser"
        elif "codegen" in file_path:
            component = "CodeGen"
        elif "semantic" in file_path:
            component = "Semantic"
        elif "lexer" in file_path:
            component = "Lexer"
        
        # Handle string concatenation
        if rest and rest != "":
            # Convert << operators to + for string concatenation
            rest = rest.replace('<<', '+')
            rest = rest.replace('std::endl', '""')
            # Clean up the expression
            rest = re.sub(r'\s*\+\s*', ' + ', rest)
            message = f'"{message}" + {rest}'
        else:
            message = f'"{message}"'
        
        if component:
            return f'TSC_LOG_DEBUG({message}, "{component}");'
        else:
            return f'TSC_LOG_DEBUG({message});'
    
    # Apply the replacement
    content = re.sub(debug_pattern, replace_debug, content)
    
    # Also handle simpler patterns like std::cout << "DEBUG: message" << std::endl;
    simple_pattern = r'std::cout\s*<<\s*"DEBUG:\s*([^"]+)"\s*<<\s*std::endl;'
    
    def replace_simple_debug(match):
        message = match.group(1).strip()
        
        # Determine component based on file path
        component = ""
        if "parser" in file_path:
            component = "Parser"
        elif "codegen" in file_path:
            component = "CodeGen"
        elif "semantic" in file_path:
            component = "Semantic"
        elif "lexer" in file_path:
            component = "Lexer"
        
        if component:
            return f'TSC_LOG_DEBUG("{message}", "{component}");'
        else:
            return f'TSC_LOG_DEBUG("{message}");'
    
    content = re.sub(simple_pattern, replace_simple_debug, content)
    
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
            replace_debug_statements(file_path)
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
                    if replace_debug_statements(file_path):
                        updated_files += 1
        
        print(f"\nProcessed {updated_files} files with debug statements.")

if __name__ == "__main__":
    main()
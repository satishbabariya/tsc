#!/usr/bin/env python3
"""
LLVM IR Validation Script for console_log variadic function signature
"""

import re
import sys
from typing import List, Dict, Tuple

def parse_llvm_ir(file_path: str) -> Dict[str, any]:
    """Parse LLVM IR file and extract function signatures and calls"""
    with open(file_path, 'r') as f:
        content = f.read()
    
    result = {
        'file_path': file_path,
        'function_declarations': [],
        'function_calls': [],
        'console_log_declaration': None,
        'console_log_calls': [],
        'number_to_string_declaration': None,
        'number_to_string_calls': []
    }
    
    # Extract function declarations
    decl_pattern = r'declare\s+(\w+)\s+@(\w+)\((.*?)\)'
    for match in re.finditer(decl_pattern, content):
        return_type = match.group(1)
        function_name = match.group(2)
        params = match.group(3).strip()
        
        decl = {
            'return_type': return_type,
            'name': function_name,
            'parameters': params,
            'is_variadic': '...' in params
        }
        result['function_declarations'].append(decl)
        
        if function_name == 'console_log':
            result['console_log_declaration'] = decl
        elif function_name == 'number_to_string':
            result['number_to_string_declaration'] = decl
    
    # Extract function calls
    call_pattern = r'call\s+(\w+)\s+\(([^)]*)\)\s+@(\w+)\((.*?)\)'
    for match in re.finditer(call_pattern, content):
        return_type = match.group(1)
        call_type = match.group(2)  # e.g., "ptr, ..."
        function_name = match.group(3)
        args = match.group(4).strip()
        
        call = {
            'return_type': return_type,
            'call_type': call_type,
            'name': function_name,
            'arguments': args,
            'is_variadic': '...' in call_type
        }
        result['function_calls'].append(call)
        
        if function_name == 'console_log':
            result['console_log_calls'].append(call)
        elif function_name == 'number_to_string':
            result['number_to_string_calls'].append(call)
    
    return result

def validate_console_log_signature(ir_data: Dict) -> List[str]:
    """Validate console_log function signature and calls"""
    issues = []
    
    # Check if console_log declaration exists
    if not ir_data['console_log_declaration']:
        issues.append("ERROR: console_log function declaration not found")
        return issues
    
    decl = ir_data['console_log_declaration']
    
    # Check return type
    if decl['return_type'] != 'void':
        issues.append(f"ERROR: console_log return type should be 'void', found '{decl['return_type']}'")
    
    # Check if it's variadic
    if not decl['is_variadic']:
        issues.append("ERROR: console_log should be variadic (have '...' in parameters)")
    
    # Check parameter types
    expected_params = "ptr, ..."
    if decl['parameters'] != expected_params:
        issues.append(f"ERROR: console_log parameters should be '{expected_params}', found '{decl['parameters']}'")
    
    # Validate function calls
    for i, call in enumerate(ir_data['console_log_calls']):
        call_id = f"call #{i+1}"
        
        # Check call type
        if not call['is_variadic']:
            issues.append(f"ERROR: {call_id} should be variadic (have '...' in call type)")
        
        # Check return type
        if call['return_type'] != 'void':
            issues.append(f"ERROR: {call_id} return type should be 'void', found '{call['return_type']}'")
        
        # Count arguments
        args = call['arguments']
        if args:
            arg_count = len([arg.strip() for arg in args.split(',') if arg.strip()])
            print(f"INFO: {call_id} has {arg_count} arguments: {args}")
        else:
            print(f"INFO: {call_id} has 0 arguments")
    
    return issues

def validate_number_to_string_signature(ir_data: Dict) -> List[str]:
    """Validate number_to_string function signature"""
    issues = []
    
    if not ir_data['number_to_string_declaration']:
        issues.append("ERROR: number_to_string function declaration not found")
        return issues
    
    decl = ir_data['number_to_string_declaration']
    
    # Check return type
    if decl['return_type'] != 'ptr':
        issues.append(f"ERROR: number_to_string return type should be 'ptr', found '{decl['return_type']}'")
    
    # Check parameters
    expected_params = "double"
    if decl['parameters'] != expected_params:
        issues.append(f"ERROR: number_to_string parameters should be '{expected_params}', found '{decl['parameters']}'")
    
    return issues

def print_validation_report(ir_data: Dict, issues: List[str]):
    """Print a comprehensive validation report"""
    print("=" * 60)
    print("LLVM IR VALIDATION REPORT")
    print("=" * 60)
    print(f"File: {ir_data['file_path']}")
    print()
    
    # Function declarations
    print("FUNCTION DECLARATIONS:")
    for decl in ir_data['function_declarations']:
        variadic_str = " (variadic)" if decl['is_variadic'] else ""
        print(f"  {decl['return_type']} @{decl['name']}({decl['parameters']}){variadic_str}")
    print()
    
    # Console.log calls
    print("CONSOLE.LOG CALLS:")
    for i, call in enumerate(ir_data['console_log_calls']):
        print(f"  Call #{i+1}: {call['return_type']} ({call['call_type']}) @{call['name']}({call['arguments']})")
    print()
    
    # Number to string calls
    print("NUMBER_TO_STRING CALLS:")
    for i, call in enumerate(ir_data['number_to_string_calls']):
        print(f"  Call #{i+1}: {call['return_type']} @{call['name']}({call['arguments']})")
    print()
    
    # Issues
    if issues:
        print("VALIDATION ISSUES:")
        for issue in issues:
            print(f"  {issue}")
        print()
        print("❌ VALIDATION FAILED")
    else:
        print("✅ VALIDATION PASSED")
    
    print("=" * 60)

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 validate_llvm_ir.py <llvm_ir_file>")
        sys.exit(1)
    
    ir_file = sys.argv[1]
    
    try:
        # Parse the LLVM IR
        ir_data = parse_llvm_ir(ir_file)
        
        # Validate console_log
        console_log_issues = validate_console_log_signature(ir_data)
        
        # Validate number_to_string
        number_to_string_issues = validate_number_to_string_signature(ir_data)
        
        # Combine all issues
        all_issues = console_log_issues + number_to_string_issues
        
        # Print report
        print_validation_report(ir_data, all_issues)
        
        # Exit with appropriate code
        sys.exit(0 if not all_issues else 1)
        
    except FileNotFoundError:
        print(f"ERROR: File '{ir_file}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
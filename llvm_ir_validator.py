#!/usr/bin/env python3
"""
LLVM IR Validator - Comprehensive tool for analyzing and validating generated LLVM IR
"""

import re
import sys
import json
import argparse
from typing import Dict, List, Set, Optional, Tuple
from dataclasses import dataclass
from pathlib import Path

@dataclass
class FunctionSignature:
    name: str
    return_type: str
    parameters: List[str]
    is_variadic: bool = False
    is_generic: bool = False

@dataclass
class CallSite:
    function_name: str
    arguments: List[str]
    line_number: int

@dataclass
class ValidationResult:
    file_path: str
    functions: List[FunctionSignature]
    call_sites: List[CallSite]
    errors: List[str]
    warnings: List[str]
    is_valid: bool

class LLVMIRValidator:
    def __init__(self):
        self.functions: Dict[str, FunctionSignature] = {}
        self.call_sites: List[CallSite] = []
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.content: str = ""
    
    def parse_llvm_file(self, file_path: str) -> ValidationResult:
        """Parse an LLVM IR file and extract function signatures and call sites"""
        self.functions = {}
        self.call_sites = []
        self.errors = []
        self.warnings = []
        
        try:
            with open(file_path, 'r') as f:
                self.content = f.read()
            
            self._parse_functions(self.content)
            self._parse_call_sites(self.content)
            self._validate_signatures()
            
            return ValidationResult(
                file_path=file_path,
                functions=list(self.functions.values()),
                call_sites=self.call_sites,
                errors=self.errors,
                warnings=self.warnings,
                is_valid=len(self.errors) == 0
            )
            
        except Exception as e:
            self.errors.append(f"Failed to parse file {file_path}: {str(e)}")
            return ValidationResult(
                file_path=file_path,
                functions=[],
                call_sites=[],
                errors=self.errors,
                warnings=self.warnings,
                is_valid=False
            )
    
    def _parse_functions(self, content: str):
        """Parse function definitions from LLVM IR"""
        # Pattern for function definitions
        func_pattern = r'define\s+(\w+)\s+@(\w+)\((.*?)\)\s*{'
        
        for match in re.finditer(func_pattern, content, re.MULTILINE):
            return_type = match.group(1)
            func_name = match.group(2)
            params_str = match.group(3).strip()
            
            # Parse parameters
            parameters = []
            is_variadic = False
            
            if params_str and params_str != "...":
                # Split parameters, handling complex types
                param_parts = self._split_parameters(params_str)
                for param in param_parts:
                    if param.strip() == "...":
                        is_variadic = True
                    else:
                        parameters.append(param.strip())
            elif params_str == "...":
                is_variadic = True
            
            # Check if this is a generic function (contains type parameters)
            is_generic = "_" in func_name and any(
                type_name in func_name for type_name in ["number", "string", "boolean"]
            )
            
            self.functions[func_name] = FunctionSignature(
                name=func_name,
                return_type=return_type,
                parameters=parameters,
                is_variadic=is_variadic,
                is_generic=is_generic
            )
    
    def _split_parameters(self, params_str: str) -> List[str]:
        """Split parameter string, handling complex types with commas"""
        params = []
        current_param = ""
        paren_count = 0
        
        for char in params_str:
            if char == '(':
                paren_count += 1
            elif char == ')':
                paren_count -= 1
            elif char == ',' and paren_count == 0:
                if current_param.strip():
                    params.append(current_param.strip())
                current_param = ""
                continue
            
            current_param += char
        
        if current_param.strip():
            params.append(current_param.strip())
        
        return params
    
    def _parse_call_sites(self, content: str):
        """Parse function call sites from LLVM IR"""
        # Pattern for function calls
        call_pattern = r'%(\w+)\s*=\s*call\s+(\w+)\s+@(\w+)\((.*?)\)'
        
        lines = content.split('\n')
        for i, line in enumerate(lines):
            for match in re.finditer(call_pattern, line):
                result_var = match.group(1)
                return_type = match.group(2)
                func_name = match.group(3)
                args_str = match.group(4).strip()
                
                # Parse arguments
                arguments = []
                if args_str:
                    arg_parts = self._split_parameters(args_str)
                    arguments = [arg.strip() for arg in arg_parts if arg.strip()]
                
                self.call_sites.append(CallSite(
                    function_name=func_name,
                    arguments=arguments,
                    line_number=i + 1
                ))
    
    def _validate_signatures(self):
        """Validate that call sites match function signatures"""
        for call_site in self.call_sites:
            func_name = call_site.function_name
            
            if func_name not in self.functions:
                self.warnings.append(
                    f"Line {call_site.line_number}: Call to undefined function '{func_name}'"
                )
                continue
            
            func_sig = self.functions[func_name]
            
            # Check argument count
            if not func_sig.is_variadic:
                if len(call_site.arguments) != len(func_sig.parameters):
                    self.errors.append(
                        f"Line {call_site.line_number}: Function '{func_name}' expects "
                        f"{len(func_sig.parameters)} arguments, got {len(call_site.arguments)}"
                    )
            else:
                if len(call_site.arguments) < len(func_sig.parameters):
                    self.errors.append(
                        f"Line {call_site.line_number}: Variadic function '{func_name}' expects "
                        f"at least {len(func_sig.parameters)} arguments, got {len(call_site.arguments)}"
                    )
    
    def validate_generic_methods(self) -> List[str]:
        """Validate generic method signatures and calls"""
        generic_errors = []
        
        for func_name, func_sig in self.functions.items():
            if func_sig.is_generic:
                # Check if generic method has proper signature
                if not func_sig.parameters:
                    generic_errors.append(
                        f"Generic method '{func_name}' should have at least one parameter (this pointer)"
                    )
                elif not func_sig.parameters[0].startswith('ptr'):
                    generic_errors.append(
                        f"Generic method '{func_name}' first parameter should be 'ptr' (this pointer), "
                        f"got '{func_sig.parameters[0]}'"
                    )
        
        return generic_errors
    
    def validate_print_function(self) -> List[str]:
        """Validate _print function signature"""
        print_errors = []
        
        # Check for _print in both functions and declarations
        print_found = False
        print_is_variadic = False
        print_return_type = None
        
        if '_print' in self.functions:
            print_func = self.functions['_print']
            print_found = True
            print_is_variadic = print_func.is_variadic
            print_return_type = print_func.return_type
        else:
            # Check if _print is declared but not defined
            if 'declare void @_print(ptr, ...)' in self.content:
                print_found = True
                print_is_variadic = True
                print_return_type = 'void'
        
        if not print_found:
            print_errors.append("_print function not found")
        else:
            if not print_is_variadic:
                print_errors.append("_print function should be variadic")
            if print_return_type != 'void':
                print_errors.append("_print function should return void")
        
        return print_errors

def main():
    parser = argparse.ArgumentParser(description='Validate LLVM IR files')
    parser.add_argument('file', help='LLVM IR file to validate')
    parser.add_argument('--json', action='store_true', help='Output results in JSON format')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    
    args = parser.parse_args()
    
    validator = LLVMIRValidator()
    result = validator.parse_llvm_file(args.file)
    
    # Additional validations
    generic_errors = validator.validate_generic_methods()
    print_errors = validator.validate_print_function()
    
    result.errors.extend(generic_errors)
    result.errors.extend(print_errors)
    result.is_valid = len(result.errors) == 0
    
    if args.json:
        output = {
            'file_path': result.file_path,
            'is_valid': result.is_valid,
            'functions': [
                {
                    'name': f.name,
                    'return_type': f.return_type,
                    'parameters': f.parameters,
                    'is_variadic': f.is_variadic,
                    'is_generic': f.is_generic
                } for f in result.functions
            ],
            'call_sites': [
                {
                    'function_name': c.function_name,
                    'arguments': c.arguments,
                    'line_number': c.line_number
                } for c in result.call_sites
            ],
            'errors': result.errors,
            'warnings': result.warnings
        }
        print(json.dumps(output, indent=2))
    else:
        print(f"LLVM IR Validation Results for: {result.file_path}")
        print("=" * 60)
        
        print(f"Valid: {'✓' if result.is_valid else '✗'}")
        print(f"Functions found: {len(result.functions)}")
        print(f"Call sites found: {len(result.call_sites)}")
        print(f"Errors: {len(result.errors)}")
        print(f"Warnings: {len(result.warnings)}")
        
        if result.errors:
            print("\nErrors:")
            for error in result.errors:
                print(f"  ✗ {error}")
        
        if result.warnings:
            print("\nWarnings:")
            for warning in result.warnings:
                print(f"  ⚠ {warning}")
        
        if args.verbose:
            print("\nFunctions:")
            for func in result.functions:
                print(f"  {func.name}: {func.return_type} ({', '.join(func.parameters)})")
                if func.is_variadic:
                    print("    [variadic]")
                if func.is_generic:
                    print("    [generic]")
            
            print("\nCall Sites:")
            for call in result.call_sites:
                print(f"  Line {call.line_number}: {call.function_name}({', '.join(call.arguments)})")

if __name__ == "__main__":
    main()
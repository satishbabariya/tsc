#!/usr/bin/env python3
"""
LLVM IR Validation Suite - Comprehensive validation system for TypeScript compiler
A practical and maintainable validation framework
"""

import re
import sys
import os
import json
import subprocess
from pathlib import Path
from typing import Dict, List, Set, Optional, Tuple, Any
from dataclasses import dataclass, asdict
from enum import Enum
import argparse

class ValidationLevel(Enum):
    BASIC = "basic"
    COMPREHENSIVE = "comprehensive"
    STRICT = "strict"

@dataclass
class ValidationRule:
    name: str
    pattern: str
    error_message: str
    severity: str  # "error", "warning", "info"
    category: str  # "syntax", "semantics", "performance", "safety"

@dataclass
class ValidationResult:
    file_path: str
    is_valid: bool
    errors: List[str]
    warnings: List[str]
    info: List[str]
    function_count: int
    call_site_count: int
    validation_time: float

class LLVMValidationSuite:
    def __init__(self, validation_level: ValidationLevel = ValidationLevel.BASIC):
        self.validation_level = validation_level
        self.rules = self._load_validation_rules()
        self.results: List[ValidationResult] = []
    
    def _load_validation_rules(self) -> List[ValidationRule]:
        """Load validation rules based on validation level"""
        rules = []
        
        # Basic rules - always applied
        rules.extend([
            ValidationRule(
                name="function_declaration_syntax",
                pattern=r"define\s+\w+\s+@\w+\([^)]*\)\s*\{",
                error_message="Invalid function declaration syntax",
                severity="error",
                category="syntax"
            ),
            ValidationRule(
                name="call_site_syntax",
                pattern=r"(?:%\w+\s*=\s*)?call\s+(?:\w+\s+\([^)]+\)\s+)?@\w+\([^)]*\)",
                error_message="Invalid call site syntax",
                severity="error",
                category="syntax"
            ),
            ValidationRule(
                name="print_function_variadic",
                pattern=r"declare\s+void\s+@_print\(ptr,\s*\.\.\.\)",
                error_message="_print function should be variadic",
                severity="error",
                category="semantics"
            )
        ])
        
        if self.validation_level in [ValidationLevel.COMPREHENSIVE, ValidationLevel.STRICT]:
            # Comprehensive rules
            rules.extend([
                ValidationRule(
                    name="generic_method_signature",
                    pattern=r"define\s+\w+\s+@\w+_\w+_\w+\(ptr\s+%\w+\)",
                    error_message="Generic method should have ptr as first parameter",
                    severity="error",
                    category="semantics"
                ),
                ValidationRule(
                    name="method_call_this_pointer",
                    pattern=r"call\s+\w+\s+@\w+\(ptr\s+%\w+",
                    error_message="Method calls should pass ptr as first argument",
                    severity="warning",
                    category="semantics"
                )
            ])
        
        if self.validation_level == ValidationLevel.STRICT:
            # Strict rules
            rules.extend([
                ValidationRule(
                    name="no_undefined_functions",
                    pattern=r"call\s+\w+\s+@undefined",
                    error_message="Call to undefined function",
                    severity="error",
                    category="safety"
                ),
                ValidationRule(
                    name="proper_type_annotations",
                    pattern=r"define\s+void\s+@\w+\(\)",
                    error_message="Functions should have proper return types",
                    severity="warning",
                    category="semantics"
                )
            ])
        
        return rules
    
    def validate_llvm_file(self, file_path: str) -> ValidationResult:
        """Validate a single LLVM IR file"""
        start_time = os.times().elapsed
        
        if not os.path.exists(file_path):
            return ValidationResult(
                file_path=file_path,
                is_valid=False,
                errors=[f"File not found: {file_path}"],
                warnings=[],
                info=[],
                function_count=0,
                call_site_count=0,
                validation_time=0.0
            )
        
        try:
            with open(file_path, 'r') as f:
                content = f.read()
            
            errors = []
            warnings = []
            info = []
            
            # Apply validation rules
            for rule in self.rules:
                if not re.search(rule.pattern, content, re.MULTILINE):
                    if rule.severity == "error":
                        errors.append(f"{rule.name}: {rule.error_message}")
                    elif rule.severity == "warning":
                        warnings.append(f"{rule.name}: {rule.error_message}")
                    else:
                        info.append(f"{rule.name}: {rule.error_message}")
            
            # Count functions and call sites
            function_count = len(re.findall(r"define\s+\w+\s+@\w+", content))
            call_site_count = len(re.findall(r"call\s+\w+\s+@\w+", content))
            
            # Additional validations
            self._validate_function_signatures(content, errors, warnings)
            self._validate_call_sites(content, errors, warnings)
            
            validation_time = os.times().elapsed - start_time
            
            result = ValidationResult(
                file_path=file_path,
                is_valid=len(errors) == 0,
                errors=errors,
                warnings=warnings,
                info=info,
                function_count=function_count,
                call_site_count=call_site_count,
                validation_time=validation_time
            )
            
            self.results.append(result)
            return result
            
        except Exception as e:
            return ValidationResult(
                file_path=file_path,
                is_valid=False,
                errors=[f"Validation error: {str(e)}"],
                warnings=[],
                info=[],
                function_count=0,
                call_site_count=0,
                validation_time=0.0
            )
    
    def _validate_function_signatures(self, content: str, errors: List[str], warnings: List[str]):
        """Validate function signatures for common issues"""
        # Check for generic method signatures
        generic_methods = re.findall(r"define\s+(\w+)\s+@(\w+_\w+_\w+)\(([^)]+)\)", content)
        for return_type, func_name, params in generic_methods:
            if not params.strip().startswith('ptr'):
                errors.append(f"Generic method {func_name} should have ptr as first parameter")
    
    def _validate_call_sites(self, content: str, errors: List[str], warnings: List[str]):
        """Validate call sites for common issues"""
        # Check for method calls with proper this pointer
        method_calls = re.findall(r"call\s+(\w+)\s+@(\w+_\w+_\w+)\(([^)]+)\)", content)
        for return_type, func_name, args in method_calls:
            if not args.strip().startswith('ptr'):
                warnings.append(f"Method call {func_name} should pass ptr as first argument")
    
    def validate_directory(self, directory: str) -> List[ValidationResult]:
        """Validate all LLVM IR files in a directory"""
        results = []
        for file_path in Path(directory).rglob("*.ll"):
            result = self.validate_llvm_file(str(file_path))
            results.append(result)
        return results
    
    def generate_report(self, output_file: Optional[str] = None) -> str:
        """Generate a comprehensive validation report"""
        if not self.results:
            return "No validation results to report"
        
        total_files = len(self.results)
        valid_files = sum(1 for r in self.results if r.is_valid)
        total_errors = sum(len(r.errors) for r in self.results)
        total_warnings = sum(len(r.warnings) for r in self.results)
        total_functions = sum(r.function_count for r in self.results)
        total_call_sites = sum(r.call_site_count for r in self.results)
        
        report = []
        report.append("LLVM IR VALIDATION REPORT")
        report.append("=" * 50)
        report.append(f"Validation Level: {self.validation_level.value}")
        report.append(f"Total Files: {total_files}")
        report.append(f"Valid Files: {valid_files} ({valid_files/total_files*100:.1f}%)")
        report.append(f"Total Errors: {total_errors}")
        report.append(f"Total Warnings: {total_warnings}")
        report.append(f"Total Functions: {total_functions}")
        report.append(f"Total Call Sites: {total_call_sites}")
        
        # Error summary by category
        error_categories = {}
        for result in self.results:
            for error in result.errors:
                category = error.split(':')[0] if ':' in error else 'unknown'
                error_categories[category] = error_categories.get(category, 0) + 1
        
        if error_categories:
            report.append("\nError Categories:")
            report.append("-" * 30)
            for category, count in sorted(error_categories.items()):
                report.append(f"{category}: {count}")
        
        # File-by-file results
        report.append("\nFile Results:")
        report.append("-" * 30)
        for result in self.results:
            status = "✓" if result.is_valid else "✗"
            report.append(f"{status} {result.file_path}")
            if result.errors:
                for error in result.errors[:3]:  # Show first 3 errors
                    report.append(f"  ERROR: {error}")
                if len(result.errors) > 3:
                    report.append(f"  ... and {len(result.errors) - 3} more errors")
        
        report_text = "\n".join(report)
        
        if output_file:
            with open(output_file, 'w') as f:
                f.write(report_text)
            report.append(f"\nReport saved to: {output_file}")
        
        return report_text
    
    def export_results(self, output_file: str):
        """Export validation results to JSON"""
        results_data = [asdict(result) for result in self.results]
        with open(output_file, 'w') as f:
            json.dump(results_data, f, indent=2)

def main():
    parser = argparse.ArgumentParser(description='LLVM IR Validation Suite')
    parser.add_argument('input', help='LLVM IR file or directory to validate')
    parser.add_argument('--level', choices=['basic', 'comprehensive', 'strict'], 
                       default='basic', help='Validation level')
    parser.add_argument('--output', help='Output file for report')
    parser.add_argument('--export', help='Export results to JSON file')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    
    args = parser.parse_args()
    
    validation_level = ValidationLevel(args.level)
    suite = LLVMValidationSuite(validation_level)
    
    if os.path.isfile(args.input):
        result = suite.validate_llvm_file(args.input)
        if args.verbose:
            print(f"Validating: {args.input}")
            print(f"Valid: {'✓' if result.is_valid else '✗'}")
            print(f"Functions: {result.function_count}")
            print(f"Call Sites: {result.call_site_count}")
            if result.errors:
                print("Errors:")
                for error in result.errors:
                    print(f"  - {error}")
    else:
        suite.validate_directory(args.input)
    
    # Generate report
    report = suite.generate_report(args.output)
    print(report)
    
    # Export results if requested
    if args.export:
        suite.export_results(args.export)
        print(f"Results exported to: {args.export}")

if __name__ == "__main__":
    main()
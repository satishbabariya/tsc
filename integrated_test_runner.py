#!/usr/bin/env python3
"""
Integrated Test Runner - Compiles tests and validates LLVM IR
Combines compilation, execution, and validation in one tool
"""

import subprocess
import sys
import os
import time
from pathlib import Path
from typing import List, Dict, Any
from llvm_validation_suite import LLVMValidationSuite, ValidationLevel

class IntegratedTestRunner:
    def __init__(self, compiler_path: str = "./build/tsc", validation_level: ValidationLevel = ValidationLevel.BASIC):
        self.compiler_path = compiler_path
        self.validation_suite = LLVMValidationSuite(validation_level)
        self.results = []
    
    def run_test(self, test_file: str, emit_llvm: bool = True) -> Dict[str, Any]:
        """Run a complete test: compile, validate, and optionally execute"""
        print(f"Testing: {test_file}")
        
        result = {
            'file': test_file,
            'compilation_success': False,
            'execution_success': False,
            'llvm_validation_success': False,
            'compilation_time': 0.0,
            'validation_time': 0.0,
            'errors': [],
            'warnings': []
        }
        
        # Step 1: Compile the test
        start_time = time.time()
        try:
            cmd = [self.compiler_path, test_file]
            if emit_llvm:
                cmd.append('--emit-llvm')
            
            compile_result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            result['compilation_time'] = time.time() - start_time
            result['compilation_success'] = compile_result.returncode == 0
            
            if not result['compilation_success']:
                result['errors'].append(f"Compilation failed: {compile_result.stderr}")
                return result
                
        except subprocess.TimeoutExpired:
            result['errors'].append("Compilation timeout (60s)")
            return result
        except Exception as e:
            result['errors'].append(f"Compilation error: {str(e)}")
            return result
        
        # Step 2: Validate LLVM IR if compilation succeeded and LLVM was emitted
        if emit_llvm and os.path.exists("generated_ir.ll"):
            validation_result = self.validation_suite.validate_llvm_file("generated_ir.ll")
            result['llvm_validation_success'] = validation_result.is_valid
            result['validation_time'] = validation_result.validation_time
            result['errors'].extend(validation_result.errors)
            result['warnings'].extend(validation_result.warnings)
        
        # Step 3: Execute the test if compilation succeeded
        if result['compilation_success']:
            executable = os.path.abspath(os.path.splitext(test_file)[0])
            if os.path.exists(executable) and os.access(executable, os.X_OK):
                try:
                    exec_result = subprocess.run(
                        [executable],
                        capture_output=True,
                        text=True,
                        timeout=10
                    )
                    result['execution_success'] = exec_result.returncode == 0
                    if not result['execution_success']:
                        result['warnings'].append(f"Execution failed: {exec_result.stderr}")
                except subprocess.TimeoutExpired:
                    result['warnings'].append("Execution timeout (10s)")
                except Exception as e:
                    result['warnings'].append(f"Execution error: {str(e)}")
            else:
                result['warnings'].append("No executable found for execution test")
        
        self.results.append(result)
        return result
    
    def run_test_suite(self, test_directory: str = "tests/unit") -> None:
        """Run all tests in a directory"""
        test_files = []
        for file_path in Path(test_directory).rglob("*.ts"):
            test_files.append(str(file_path))
        
        print(f"Found {len(test_files)} test files")
        print("=" * 60)
        
        for test_file in test_files:
            result = self.run_test(test_file)
            status = "✓" if result['compilation_success'] and result['llvm_validation_success'] else "✗"
            print(f"{status} {test_file}")
            
            if result['errors']:
                for error in result['errors'][:2]:  # Show first 2 errors
                    print(f"  ERROR: {error}")
                if len(result['errors']) > 2:
                    print(f"  ... and {len(result['errors']) - 2} more errors")
    
    def generate_summary(self) -> str:
        """Generate a summary of test results"""
        if not self.results:
            return "No test results to summarize"
        
        total_tests = len(self.results)
        compilation_success = sum(1 for r in self.results if r['compilation_success'])
        validation_success = sum(1 for r in self.results if r['llvm_validation_success'])
        execution_success = sum(1 for r in self.results if r['execution_success'])
        
        total_errors = sum(len(r['errors']) for r in self.results)
        total_warnings = sum(len(r['warnings']) for r in self.results)
        
        summary = []
        summary.append("\n" + "=" * 60)
        summary.append("INTEGRATED TEST RUNNER SUMMARY")
        summary.append("=" * 60)
        summary.append(f"Total Tests: {total_tests}")
        summary.append(f"Compilation Success: {compilation_success} ({compilation_success/total_tests*100:.1f}%)")
        summary.append(f"LLVM Validation Success: {validation_success} ({validation_success/total_tests*100:.1f}%)")
        summary.append(f"Execution Success: {execution_success} ({execution_success/total_tests*100:.1f}%)")
        summary.append(f"Total Errors: {total_errors}")
        summary.append(f"Total Warnings: {total_warnings}")
        
        # Show failing tests
        failing_tests = [r for r in self.results if not (r['compilation_success'] and r['llvm_validation_success'])]
        if failing_tests:
            summary.append(f"\nFailing Tests ({len(failing_tests)}):")
            summary.append("-" * 40)
            for result in failing_tests[:10]:  # Show first 10 failures
                summary.append(f"✗ {result['file']}")
                for error in result['errors'][:1]:  # Show first error
                    summary.append(f"  - {error}")
        
        return "\n".join(summary)

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Integrated Test Runner')
    parser.add_argument('--compiler', default='./build/tsc', help='Path to TypeScript compiler')
    parser.add_argument('--test-dir', default='tests/unit', help='Test directory to run')
    parser.add_argument('--validation-level', choices=['basic', 'comprehensive', 'strict'], 
                       default='basic', help='LLVM validation level')
    parser.add_argument('--single', help='Run a single test file')
    
    args = parser.parse_args()
    
    validation_level = ValidationLevel(args.validation_level)
    runner = IntegratedTestRunner(args.compiler, validation_level)
    
    if args.single:
        result = runner.run_test(args.single)
        print(f"Test: {result['file']}")
        print(f"Compilation: {'✓' if result['compilation_success'] else '✗'}")
        print(f"Validation: {'✓' if result['llvm_validation_success'] else '✗'}")
        print(f"Execution: {'✓' if result['execution_success'] else '✗'}")
        if result['errors']:
            print("Errors:")
            for error in result['errors']:
                print(f"  - {error}")
    else:
        runner.run_test_suite(args.test_dir)
        print(runner.generate_summary())

if __name__ == "__main__":
    main()
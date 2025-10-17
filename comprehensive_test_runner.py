#!/usr/bin/env python3
"""
Comprehensive Test Runner for TypeScript Compiler
Tests all available test files and validates LLVM IR generation
"""

import subprocess
import sys
import os
import json
import time
from pathlib import Path
from typing import List, Dict, Any, Tuple
from dataclasses import dataclass
import concurrent.futures
import threading

@dataclass
class TestResult:
    file_path: str
    compilation_success: bool
    execution_success: bool
    llvm_ir_valid: bool
    errors: List[str]
    warnings: List[str]
    execution_time: float
    output: str

class ComprehensiveTestRunner:
    def __init__(self, compiler_path: str = "./build/tsc", max_workers: int = 4):
        self.compiler_path = compiler_path
        self.validator_path = "./llvm_ir_validator.py"
        self.max_workers = max_workers
        self.results: List[TestResult] = []
        self.lock = threading.Lock()
    
    def find_all_tests(self) -> List[str]:
        """Find all TypeScript test files in the project"""
        test_dirs = [
            "tests/unit",
            "tests/integration", 
            "tests/semantic",
            "tests/types",
            "tests/es5"
        ]
        
        all_tests = []
        for test_dir in test_dirs:
            if os.path.exists(test_dir):
                for file_path in Path(test_dir).rglob("*.ts"):
                    all_tests.append(str(file_path))
        
        return sorted(all_tests)
    
    def compile_test(self, test_file: str) -> Tuple[bool, str, str, float]:
        """Compile a single test file"""
        start_time = time.time()
        
        try:
            result = subprocess.run(
                [self.compiler_path, test_file],
                capture_output=True,
                text=True,
                timeout=60
            )
            
            execution_time = time.time() - start_time
            
            return (
                result.returncode == 0,
                result.stdout,
                result.stderr,
                execution_time
            )
        except subprocess.TimeoutExpired:
            return (False, "", "Compilation timeout (60s)", time.time() - start_time)
        except Exception as e:
            return (False, "", str(e), time.time() - start_time)
    
    def validate_llvm_ir(self, test_file: str) -> Tuple[bool, List[str], List[str]]:
        """Validate generated LLVM IR"""
        ir_file = "generated_ir.ll"
        
        if not os.path.exists(ir_file):
            return False, ["No LLVM IR file generated"], []
        
        try:
            result = subprocess.run(
                [sys.executable, self.validator_path, ir_file, '--json'],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            if result.returncode == 0:
                validation_data = json.loads(result.stdout)
                return (
                    validation_data.get('is_valid', False),
                    validation_data.get('errors', []),
                    validation_data.get('warnings', [])
                )
            else:
                return False, [f"Validation failed: {result.stderr}"], []
        except Exception as e:
            return False, [f"Validation error: {str(e)}"], []
    
    def execute_test(self, test_file: str) -> Tuple[bool, str]:
        """Execute compiled test if executable exists"""
        # Look for executable with same name as test file
        base_name = os.path.splitext(test_file)[0]
        executable = base_name
        
        if not os.path.exists(executable) or not os.access(executable, os.X_OK):
            return False, "No executable found"
        
        try:
            result = subprocess.run(
                [executable],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            return (
                result.returncode == 0,
                result.stdout + result.stderr
            )
        except subprocess.TimeoutExpired:
            return False, "Execution timeout (10s)"
        except Exception as e:
            return False, f"Execution error: {str(e)}"
    
    def run_single_test(self, test_file: str) -> TestResult:
        """Run a complete test on a single file"""
        print(f"Testing: {test_file}")
        
        # Compile the test
        comp_success, stdout, stderr, exec_time = self.compile_test(test_file)
        
        # Validate LLVM IR if compilation succeeded
        ir_valid, ir_errors, ir_warnings = False, [], []
        if comp_success:
            ir_valid, ir_errors, ir_warnings = self.validate_llvm_ir(test_file)
        
        # Execute the test if compilation succeeded
        exec_success, exec_output = False, ""
        if comp_success:
            exec_success, exec_output = self.execute_test(test_file)
        
        # Combine all errors and warnings
        all_errors = []
        all_warnings = []
        
        if not comp_success:
            all_errors.append(f"Compilation failed: {stderr}")
        
        all_errors.extend(ir_errors)
        all_warnings.extend(ir_warnings)
        
        if not exec_success and comp_success:
            all_errors.append(f"Execution failed: {exec_output}")
        
        result = TestResult(
            file_path=test_file,
            compilation_success=comp_success,
            execution_success=exec_success,
            llvm_ir_valid=ir_valid,
            errors=all_errors,
            warnings=all_warnings,
            execution_time=exec_time,
            output=stdout + exec_output
        )
        
        with self.lock:
            self.results.append(result)
        
        return result
    
    def run_all_tests(self, test_files: List[str] = None) -> None:
        """Run all tests with parallel execution"""
        if test_files is None:
            test_files = self.find_all_tests()
        
        print(f"Found {len(test_files)} test files")
        print(f"Running tests with {self.max_workers} parallel workers...")
        
        with concurrent.futures.ThreadPoolExecutor(max_workers=self.max_workers) as executor:
            futures = [executor.submit(self.run_single_test, test_file) for test_file in test_files]
            
            for future in concurrent.futures.as_completed(futures):
                try:
                    result = future.result()
                    status = "✓" if result.compilation_success and result.llvm_ir_valid else "✗"
                    print(f"{status} {result.file_path}")
                except Exception as e:
                    print(f"✗ Error running test: {e}")
    
    def generate_report(self) -> None:
        """Generate comprehensive test report"""
        if not self.results:
            print("No test results to report")
            return
        
        print("\n" + "="*80)
        print("COMPREHENSIVE TEST EXECUTION REPORT")
        print("="*80)
        
        total_tests = len(self.results)
        successful_compilations = sum(1 for r in self.results if r.compilation_success)
        successful_executions = sum(1 for r in self.results if r.execution_success)
        valid_ir = sum(1 for r in self.results if r.llvm_ir_valid)
        overall_success = sum(1 for r in self.results if r.compilation_success and r.llvm_ir_valid)
        
        print(f"Total Tests: {total_tests}")
        print(f"Successful Compilations: {successful_compilations} ({successful_compilations/total_tests*100:.1f}%)")
        print(f"Successful Executions: {successful_executions} ({successful_executions/total_tests*100:.1f}%)")
        print(f"Valid LLVM IR: {valid_ir} ({valid_ir/total_tests*100:.1f}%)")
        print(f"Overall Success: {overall_success} ({overall_success/total_tests*100:.1f}%)")
        
        # Show failing tests
        failing_tests = [r for r in self.results if not (r.compilation_success and r.llvm_ir_valid)]
        if failing_tests:
            print(f"\nFailing Tests ({len(failing_tests)}):")
            print("-" * 80)
            for result in failing_tests[:20]:  # Show first 20 failures
                print(f"✗ {result.file_path}")
                for error in result.errors[:3]:  # Show first 3 errors
                    print(f"  - {error}")
                if len(result.errors) > 3:
                    print(f"  ... and {len(result.errors) - 3} more errors")
        
        # Show test categories
        categories = {}
        for result in self.results:
            category = result.file_path.split('/')[1] if '/' in result.file_path else 'root'
            if category not in categories:
                categories[category] = {'total': 0, 'success': 0}
            categories[category]['total'] += 1
            if result.compilation_success and result.llvm_ir_valid:
                categories[category]['success'] += 1
        
        print(f"\nTest Categories:")
        print("-" * 80)
        for category, stats in categories.items():
            success_rate = stats['success'] / stats['total'] * 100
            print(f"{category}: {stats['success']}/{stats['total']} ({success_rate:.1f}%)")
        
        # Save detailed results
        with open('comprehensive_test_results.json', 'w') as f:
            json.dump([{
                'file_path': r.file_path,
                'compilation_success': r.compilation_success,
                'execution_success': r.execution_success,
                'llvm_ir_valid': r.llvm_ir_valid,
                'errors': r.errors,
                'warnings': r.warnings,
                'execution_time': r.execution_time
            } for r in self.results], f, indent=2)
        
        print(f"\nDetailed results saved to: comprehensive_test_results.json")

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Run comprehensive compiler tests')
    parser.add_argument('--compiler', default='./build/tsc',
                       help='Path to the TypeScript compiler')
    parser.add_argument('--workers', type=int, default=4,
                       help='Number of parallel workers')
    parser.add_argument('--limit', type=int, default=None,
                       help='Limit number of tests to run')
    parser.add_argument('--category', choices=['unit', 'integration', 'semantic', 'types', 'es5'],
                       help='Run tests from specific category only')
    
    args = parser.parse_args()
    
    runner = ComprehensiveTestRunner(args.compiler, args.workers)
    
    # Find test files
    all_tests = runner.find_all_tests()
    
    # Filter by category if specified
    if args.category:
        all_tests = [t for t in all_tests if f'tests/{args.category}' in t]
    
    # Limit number of tests if specified
    if args.limit:
        all_tests = all_tests[:args.limit]
    
    print(f"Running {len(all_tests)} tests...")
    
    # Run tests
    runner.run_all_tests(all_tests)
    
    # Generate report
    runner.generate_report()

if __name__ == "__main__":
    main()
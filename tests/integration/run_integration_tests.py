#!/usr/bin/env python3
"""
Comprehensive Integration Test Runner
Validates EnhancedIRGen and parser improvements with realistic TypeScript code
"""

import os
import sys
import json
import subprocess
import time
import psutil
from pathlib import Path
from typing import Dict, List, Any, Tuple
import concurrent.futures
import threading

class IntegrationTestRunner:
    def __init__(self, test_dir: str = "tests/integration"):
        self.test_dir = Path(test_dir)
        self.config = self._load_config()
        self.results = {
            "test_cases": {},
            "performance": {},
            "memory": {},
            "summary": {}
        }
        self.lock = threading.Lock()
        
    def _load_config(self) -> Dict[str, Any]:
        """Load test configuration"""
        config_file = self.test_dir / "test_config.json"
        if config_file.exists():
            with open(config_file, 'r') as f:
                return json.load(f)
        return {}
    
    def run_all_tests(self) -> bool:
        """Run all integration tests"""
        print("🚀 Starting Comprehensive Integration Tests")
        print("=" * 60)
        
        start_time = time.time()
        
        # Run test suites
        test_suites = self.config.get("test_suites", {})
        all_passed = True
        
        for suite_name, suite_config in test_suites.items():
            print(f"\n📁 Running {suite_name} tests...")
            suite_passed = self._run_test_suite(suite_name, suite_config)
            all_passed = all_passed and suite_passed
            
        # Run validation tests
        print(f"\n🔍 Running validation tests...")
        validation_passed = self._run_validation_tests()
        all_passed = all_passed and validation_passed
        
        # Run performance benchmarks
        print(f"\n⚡ Running performance benchmarks...")
        performance_passed = self._run_performance_tests()
        all_passed = all_passed and performance_passed
        
        # Run memory validation
        print(f"\n🧠 Running memory validation...")
        memory_passed = self._run_memory_tests()
        all_passed = all_passed and memory_passed
        
        # Generate summary
        end_time = time.time()
        self._generate_summary(end_time - start_time, all_passed)
        
        return all_passed
    
    def _run_test_suite(self, suite_name: str, suite_config: Dict[str, Any]) -> bool:
        """Run a specific test suite"""
        files = suite_config.get("files", [])
        suite_passed = True
        
        for file_path in files:
            full_path = self.test_dir / "test_cases" / file_path
            if full_path.exists():
                print(f"  📄 Testing {file_path}...")
                test_passed = self._run_single_test(full_path)
                suite_passed = suite_passed and test_passed
            else:
                print(f"  ❌ Test file not found: {file_path}")
                suite_passed = False
                
        return suite_passed
    
    def _run_single_test(self, test_file: Path) -> bool:
        """Run a single test file"""
        test_name = test_file.stem
        
        try:
            # Compile the TypeScript file
            compile_result = self._compile_typescript(test_file)
            
            # Generate LLVM IR
            ir_result = self._generate_llvm_ir(test_file)
            
            # Validate LLVM IR
            validation_result = self._validate_llvm_ir(test_file)
            
            # Check if all steps passed
            test_passed = (
                compile_result["success"] and 
                ir_result["success"] and 
                validation_result["success"]
            )
            
            # Store results
            with self.lock:
                self.results["test_cases"][test_name] = {
                    "compile": compile_result,
                    "ir_generation": ir_result,
                    "validation": validation_result,
                    "overall_success": test_passed
                }
            
            status = "✅ PASS" if test_passed else "❌ FAIL"
            print(f"    {status} {test_name}")
            
            if not test_passed:
                print(f"      Compile: {'✅' if compile_result['success'] else '❌'}")
                print(f"      IR Gen: {'✅' if ir_result['success'] else '❌'}")
                print(f"      Validation: {'✅' if validation_result['success'] else '❌'}")
            
            return test_passed
            
        except Exception as e:
            print(f"    ❌ ERROR {test_name}: {e}")
            with self.lock:
                self.results["test_cases"][test_name] = {
                    "error": str(e),
                    "overall_success": False
                }
            return False
    
    def _compile_typescript(self, test_file: Path) -> Dict[str, Any]:
        """Compile TypeScript file using our compiler"""
        try:
            start_time = time.time()
            
            # Run the compiler
            result = subprocess.run([
                "./tsc", str(test_file)
            ], capture_output=True, text=True, timeout=30)
            
            end_time = time.time()
            
            return {
                "success": result.returncode == 0,
                "stdout": result.stdout,
                "stderr": result.stderr,
                "compile_time": end_time - start_time,
                "return_code": result.returncode
            }
            
        except subprocess.TimeoutExpired:
            return {
                "success": False,
                "error": "Compilation timeout",
                "compile_time": 30
            }
        except Exception as e:
            return {
                "success": False,
                "error": str(e),
                "compile_time": 0
            }
    
    def _generate_llvm_ir(self, test_file: Path) -> Dict[str, Any]:
        """Generate LLVM IR from compiled code"""
        try:
            test_name = test_file.stem
            ir_file = self.test_dir / "generated_output" / f"{test_name}.ll"
            
            # Ensure output directory exists
            ir_file.parent.mkdir(parents=True, exist_ok=True)
            
            # Generate IR (this would be implemented in the compiler)
            result = subprocess.run([
                "./tsc", "--emit-llvm", str(test_file), "-o", str(ir_file)
            ], capture_output=True, text=True, timeout=30)
            
            return {
                "success": result.returncode == 0 and ir_file.exists(),
                "ir_file": str(ir_file),
                "stdout": result.stdout,
                "stderr": result.stderr,
                "return_code": result.returncode
            }
            
        except Exception as e:
            return {
                "success": False,
                "error": str(e)
            }
    
    def _validate_llvm_ir(self, test_file: Path) -> Dict[str, Any]:
        """Validate generated LLVM IR"""
        try:
            test_name = test_file.stem
            ir_file = self.test_dir / "generated_output" / f"{test_name}.ll"
            
            if not ir_file.exists():
                return {
                    "success": False,
                    "error": "IR file not generated"
                }
            
            # Validate IR using LLVM tools
            result = subprocess.run([
                "llvm-as", str(ir_file), "-o", "/dev/null"
            ], capture_output=True, text=True, timeout=10)
            
            # Check IR structure
            with open(ir_file, 'r') as f:
                ir_content = f.read()
            
            # Basic IR validation
            required_elements = ['define', 'ret']
            missing_elements = [elem for elem in required_elements if elem not in ir_content]
            
            return {
                "success": result.returncode == 0 and len(missing_elements) == 0,
                "llvm_validation": result.returncode == 0,
                "structure_validation": len(missing_elements) == 0,
                "missing_elements": missing_elements,
                "stderr": result.stderr
            }
            
        except Exception as e:
            return {
                "success": False,
                "error": str(e)
            }
    
    def _run_validation_tests(self) -> bool:
        """Run validation tests"""
        try:
            # Run LLVM IR validation
            validation_script = self.test_dir / "validate_llvm_ir.py"
            if validation_script.exists():
                result = subprocess.run([
                    "python3", str(validation_script)
                ], capture_output=True, text=True, timeout=60)
                
                self.results["validation"] = {
                    "llvm_ir_validation": {
                        "success": result.returncode == 0,
                        "stdout": result.stdout,
                        "stderr": result.stderr
                    }
                }
                
                return result.returncode == 0
            
            return True
            
        except Exception as e:
            print(f"  ❌ Validation test error: {e}")
            return False
    
    def _run_performance_tests(self) -> bool:
        """Run performance benchmark tests"""
        try:
            performance_results = {}
            
            # Test compilation performance
            test_files = list(self.test_dir.glob("test_cases/**/*.ts"))
            
            for test_file in test_files[:5]:  # Test first 5 files
                start_time = time.time()
                result = subprocess.run([
                    "./tsc", str(test_file)
                ], capture_output=True, text=True, timeout=30)
                end_time = time.time()
                
                performance_results[test_file.stem] = {
                    "compile_time": end_time - start_time,
                    "success": result.returncode == 0,
                    "memory_usage": self._get_memory_usage()
                }
            
            self.results["performance"] = performance_results
            
            # Check if performance is within acceptable thresholds
            thresholds = self.config.get("thresholds", {})
            max_compile_time = thresholds.get("max_compile_time_seconds", 30)
            
            all_within_threshold = all(
                result["compile_time"] <= max_compile_time
                for result in performance_results.values()
            )
            
            return all_within_threshold
            
        except Exception as e:
            print(f"  ❌ Performance test error: {e}")
            return False
    
    def _run_memory_tests(self) -> bool:
        """Run memory validation tests"""
        try:
            # Run memory validation script
            memory_script = self.test_dir / "validate_memory.py"
            if memory_script.exists():
                result = subprocess.run([
                    "python3", str(memory_script)
                ], capture_output=True, text=True, timeout=120)
                
                self.results["memory"] = {
                    "validation": {
                        "success": result.returncode == 0,
                        "stdout": result.stdout,
                        "stderr": result.stderr
                    }
                }
                
                return result.returncode == 0
            
            return True
            
        except Exception as e:
            print(f"  ❌ Memory test error: {e}")
            return False
    
    def _get_memory_usage(self) -> float:
        """Get current memory usage in MB"""
        try:
            process = psutil.Process()
            return process.memory_info().rss / 1024 / 1024
        except:
            return 0.0
    
    def _generate_summary(self, total_time: float, all_passed: bool):
        """Generate test summary"""
        test_cases = self.results["test_cases"]
        total_tests = len(test_cases)
        passed_tests = sum(1 for result in test_cases.values() if result.get("overall_success", False))
        
        self.results["summary"] = {
            "total_tests": total_tests,
            "passed_tests": passed_tests,
            "failed_tests": total_tests - passed_tests,
            "success_rate": (passed_tests / total_tests * 100) if total_tests > 0 else 0,
            "total_time": total_time,
            "all_passed": all_passed
        }
        
        print(f"\n📊 Test Summary")
        print("=" * 60)
        print(f"Total Tests: {total_tests}")
        print(f"Passed: {passed_tests}")
        print(f"Failed: {total_tests - passed_tests}")
        print(f"Success Rate: {self.results['summary']['success_rate']:.1f}%")
        print(f"Total Time: {total_time:.2f}s")
        print(f"Overall Result: {'✅ ALL TESTS PASSED' if all_passed else '❌ SOME TESTS FAILED'}")
        
        # Save results to file
        results_file = self.test_dir / "test_results.json"
        with open(results_file, 'w') as f:
            json.dump(self.results, f, indent=2)
        
        print(f"\n📄 Detailed results saved to: {results_file}")

def main():
    if len(sys.argv) > 1:
        test_dir = sys.argv[1]
    else:
        test_dir = "tests/integration"
    
    runner = IntegrationTestRunner(test_dir)
    success = runner.run_all_tests()
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
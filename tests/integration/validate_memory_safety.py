#!/usr/bin/env python3
"""
Memory Safety Validation Script
Validates memory safety of Enhanced IRGen and parser using Valgrind
"""

import os
import sys
import subprocess
import json
import time
from pathlib import Path
from typing import Dict, List, Any

class MemorySafetyValidator:
    def __init__(self, test_dir: str = "tests/integration"):
        self.test_dir = Path(test_dir)
        self.results = {
            "memory_leaks": {},
            "invalid_access": {},
            "memory_usage": {},
            "summary": {}
        }
        
    def validate_all(self) -> bool:
        """Validate memory safety for all test cases"""
        print("🧠 Starting Memory Safety Validation")
        print("=" * 50)
        
        # Test files for memory validation
        test_files = [
            "test_cases/basic/arithmetic.ts",
            "test_cases/basic/control_flow.ts",
            "test_cases/basic/functions.ts",
            "test_cases/generics/function_generics.ts",
            "test_cases/generics/class_generics.ts",
            "test_cases/classes/basic_classes.ts",
            "test_cases/real_world/utility_functions.ts"
        ]
        
        all_passed = True
        
        for test_file in test_files:
            full_path = self.test_dir / test_file
            if full_path.exists():
                print(f"🔍 Validating memory safety for {test_file}...")
                test_passed = self._validate_file(full_path)
                all_passed = all_passed and test_passed
            else:
                print(f"⚠️ Test file not found: {test_file}")
        
        # Generate memory safety report
        self._generate_memory_report()
        
        return all_passed
    
    def _validate_file(self, test_file: Path) -> bool:
        """Validate memory safety for a single test file"""
        test_name = test_file.stem
        
        try:
            # Run with Valgrind
            valgrind_result = self._run_valgrind(test_file)
            
            # Analyze Valgrind output
            leak_analysis = self._analyze_memory_leaks(valgrind_result["stderr"])
            access_analysis = self._analyze_invalid_access(valgrind_result["stderr"])
            usage_analysis = self._analyze_memory_usage(valgrind_result["stderr"])
            
            # Store results
            self.results["memory_leaks"][test_name] = leak_analysis
            self.results["invalid_access"][test_name] = access_analysis
            self.results["memory_usage"][test_name] = usage_analysis
            
            # Determine if test passed
            test_passed = (
                leak_analysis["definitely_lost"] == 0 and
                leak_analysis["indirectly_lost"] == 0 and
                leak_analysis["possibly_lost"] == 0 and
                access_analysis["invalid_reads"] == 0 and
                access_analysis["invalid_writes"] == 0
            )
            
            status = "✅ PASS" if test_passed else "❌ FAIL"
            print(f"  {status} {test_name}")
            
            if not test_passed:
                if leak_analysis["definitely_lost"] > 0:
                    print(f"    Memory leaks: {leak_analysis['definitely_lost']} bytes")
                if access_analysis["invalid_reads"] > 0:
                    print(f"    Invalid reads: {access_analysis['invalid_reads']}")
                if access_analysis["invalid_writes"] > 0:
                    print(f"    Invalid writes: {access_analysis['invalid_writes']}")
            
            return test_passed
            
        except Exception as e:
            print(f"  ❌ ERROR {test_name}: {e}")
            self.results["memory_leaks"][test_name] = {"error": str(e)}
            return False
    
    def _run_valgrind(self, test_file: Path) -> Dict[str, Any]:
        """Run Valgrind on the test file"""
        try:
            # Compile the test file first
            compile_result = subprocess.run([
                "./tsc", str(test_file)
            ], capture_output=True, text=True, timeout=30)
            
            if compile_result.returncode != 0:
                return {
                    "success": False,
                    "stdout": compile_result.stdout,
                    "stderr": compile_result.stderr,
                    "error": "Compilation failed"
                }
            
            # Run with Valgrind
            valgrind_cmd = [
                "valgrind",
                "--tool=memcheck",
                "--leak-check=full",
                "--show-leak-kinds=all",
                "--track-origins=yes",
                "--error-exitcode=1",
                "./tsc", str(test_file)
            ]
            
            start_time = time.time()
            result = subprocess.run(
                valgrind_cmd,
                capture_output=True,
                text=True,
                timeout=60
            )
            end_time = time.time()
            
            return {
                "success": result.returncode == 0,
                "stdout": result.stdout,
                "stderr": result.stderr,
                "execution_time": end_time - start_time
            }
            
        except subprocess.TimeoutExpired:
            return {
                "success": False,
                "error": "Valgrind timeout",
                "execution_time": 60
            }
        except Exception as e:
            return {
                "success": False,
                "error": str(e),
                "execution_time": 0
            }
    
    def _analyze_memory_leaks(self, valgrind_output: str) -> Dict[str, Any]:
        """Analyze memory leak information from Valgrind output"""
        analysis = {
            "definitely_lost": 0,
            "indirectly_lost": 0,
            "possibly_lost": 0,
            "still_reachable": 0,
            "suppressed": 0,
            "total_leaked": 0
        }
        
        lines = valgrind_output.split('\n')
        for line in lines:
            if "definitely lost:" in line:
                analysis["definitely_lost"] = self._extract_bytes(line)
            elif "indirectly lost:" in line:
                analysis["indirectly_lost"] = self._extract_bytes(line)
            elif "possibly lost:" in line:
                analysis["possibly_lost"] = self._extract_bytes(line)
            elif "still reachable:" in line:
                analysis["still_reachable"] = self._extract_bytes(line)
            elif "suppressed:" in line:
                analysis["suppressed"] = self._extract_bytes(line)
        
        analysis["total_leaked"] = (
            analysis["definitely_lost"] + 
            analysis["indirectly_lost"] + 
            analysis["possibly_lost"]
        )
        
        return analysis
    
    def _analyze_invalid_access(self, valgrind_output: str) -> Dict[str, Any]:
        """Analyze invalid memory access information from Valgrind output"""
        analysis = {
            "invalid_reads": 0,
            "invalid_writes": 0,
            "invalid_frees": 0,
            "mismatched_frees": 0,
            "total_errors": 0
        }
        
        lines = valgrind_output.split('\n')
        for line in lines:
            if "Invalid read" in line:
                analysis["invalid_reads"] += 1
            elif "Invalid write" in line:
                analysis["invalid_writes"] += 1
            elif "Invalid free" in line:
                analysis["invalid_frees"] += 1
            elif "Mismatched free" in line:
                analysis["mismatched_frees"] += 1
        
        analysis["total_errors"] = (
            analysis["invalid_reads"] +
            analysis["invalid_writes"] +
            analysis["invalid_frees"] +
            analysis["mismatched_frees"]
        )
        
        return analysis
    
    def _analyze_memory_usage(self, valgrind_output: str) -> Dict[str, Any]:
        """Analyze memory usage information from Valgrind output"""
        analysis = {
            "total_heap_usage": 0,
            "peak_heap_usage": 0,
            "stack_usage": 0,
            "total_allocations": 0,
            "total_frees": 0
        }
        
        lines = valgrind_output.split('\n')
        for line in lines:
            if "total heap usage:" in line:
                # Extract total heap usage
                parts = line.split()
                for i, part in enumerate(parts):
                    if part == "allocs,":
                        analysis["total_allocations"] = int(parts[i-1])
                    elif part == "frees,":
                        analysis["total_frees"] = int(parts[i-1])
            elif "peak heap usage:" in line:
                analysis["peak_heap_usage"] = self._extract_bytes(line)
            elif "total stack usage:" in line:
                analysis["stack_usage"] = self._extract_bytes(line)
        
        return analysis
    
    def _extract_bytes(self, line: str) -> int:
        """Extract byte count from Valgrind output line"""
        try:
            # Look for patterns like "1,234 bytes" or "1,234B"
            import re
            match = re.search(r'(\d+(?:,\d+)*)\s*(?:bytes?|B)', line)
            if match:
                return int(match.group(1).replace(',', ''))
            return 0
        except:
            return 0
    
    def _generate_memory_report(self):
        """Generate comprehensive memory safety report"""
        print("\n📊 Memory Safety Report")
        print("=" * 50)
        
        # Calculate summary statistics
        total_tests = len(self.results["memory_leaks"])
        passed_tests = 0
        total_leaks = 0
        total_errors = 0
        
        for test_name in self.results["memory_leaks"]:
            leak_info = self.results["memory_leaks"][test_name]
            access_info = self.results["invalid_access"][test_name]
            
            if "error" not in leak_info:
                if (leak_info["definitely_lost"] == 0 and 
                    leak_info["indirectly_lost"] == 0 and
                    leak_info["possibly_lost"] == 0 and
                    access_info["total_errors"] == 0):
                    passed_tests += 1
                
                total_leaks += leak_info["total_leaked"]
                total_errors += access_info["total_errors"]
        
        self.results["summary"] = {
            "total_tests": total_tests,
            "passed_tests": passed_tests,
            "failed_tests": total_tests - passed_tests,
            "success_rate": (passed_tests / total_tests * 100) if total_tests > 0 else 0,
            "total_memory_leaks": total_leaks,
            "total_memory_errors": total_errors
        }
        
        print(f"Total Tests: {total_tests}")
        print(f"Passed: {passed_tests}")
        print(f"Failed: {total_tests - passed_tests}")
        print(f"Success Rate: {self.results['summary']['success_rate']:.1f}%")
        print(f"Total Memory Leaks: {total_leaks} bytes")
        print(f"Total Memory Errors: {total_errors}")
        
        # Save detailed results
        report_file = self.test_dir / "memory_safety_report.json"
        with open(report_file, 'w') as f:
            json.dump(self.results, f, indent=2)
        
        print(f"\n📄 Detailed memory safety report saved to: {report_file}")

def main():
    if len(sys.argv) > 1:
        test_dir = sys.argv[1]
    else:
        test_dir = "tests/integration"
    
    validator = MemorySafetyValidator(test_dir)
    success = validator.validate_all()
    
    if success:
        print("\n✅ All memory safety tests passed!")
    else:
        print("\n❌ Some memory safety tests failed!")
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
#!/usr/bin/env python3
"""
Simple Test Validator - Quick validation of core compiler functionality
"""

import subprocess
import sys
import os
from pathlib import Path

class SimpleTestValidator:
    def __init__(self, compiler_path: str = "./build/tsc"):
        self.compiler_path = compiler_path
        self.test_results = []
    
    def test_compilation(self, test_file: str) -> bool:
        """Test if a file compiles successfully"""
        try:
            result = subprocess.run(
                [self.compiler_path, test_file],
                capture_output=True,
                text=True,
                timeout=30
            )
            return result.returncode == 0
        except:
            return False
    
    def test_execution(self, test_file: str) -> bool:
        """Test if compiled executable runs successfully"""
        base_name = os.path.splitext(test_file)[0]
        executable = base_name
        
        if not os.path.exists(executable) or not os.access(executable, os.X_OK):
            return False
        
        try:
            result = subprocess.run(
                [executable],
                capture_output=True,
                text=True,
                timeout=10
            )
            return result.returncode == 0
        except:
            return False
    
    def validate_core_tests(self) -> None:
        """Validate core functionality tests"""
        core_tests = [
            "tests/unit/ultra_simple_test.ts",
            "tests/unit/simple_class_test.ts", 
            "tests/unit/minimal_generic_test.ts",
            "tests/unit/generic_method_simple_test.ts",
            "tests/unit/critical_functionality_test.ts"
        ]
        
        print("Core Functionality Validation")
        print("=" * 50)
        
        total_tests = 0
        passed_tests = 0
        
        for test_file in core_tests:
            if os.path.exists(test_file):
                total_tests += 1
                print(f"\nTesting: {test_file}")
                
                # Test compilation
                compiles = self.test_compilation(test_file)
                print(f"  Compilation: {'✓' if compiles else '✗'}")
                
                if compiles:
                    # Test execution
                    executes = self.test_execution(test_file)
                    print(f"  Execution: {'✓' if executes else '✗'}")
                    
                    if executes:
                        passed_tests += 1
                else:
                    print(f"  Execution: ✗ (compilation failed)")
            else:
                print(f"Test file not found: {test_file}")
        
        print(f"\nResults: {passed_tests}/{total_tests} tests passed ({passed_tests/total_tests*100:.1f}%)")
        
        if passed_tests == total_tests:
            print("🎉 All core tests passed!")
        else:
            print("⚠️  Some core tests failed - compiler needs attention")
    
    def quick_validation(self) -> None:
        """Quick validation of the most important functionality"""
        print("Quick Compiler Validation")
        print("=" * 40)
        
        # Test 1: Basic compilation
        print("\n1. Testing basic compilation...")
        basic_test = "tests/unit/ultra_simple_test.ts"
        if os.path.exists(basic_test):
            compiles = self.test_compilation(basic_test)
            print(f"   Basic compilation: {'✓' if compiles else '✗'}")
        else:
            print("   Basic test file not found")
        
        # Test 2: Generic functionality
        print("\n2. Testing generic functionality...")
        generic_test = "tests/unit/minimal_generic_test.ts"
        if os.path.exists(generic_test):
            compiles = self.test_compilation(generic_test)
            print(f"   Generic compilation: {'✓' if compiles else '✗'}")
        else:
            print("   Generic test file not found")
        
        # Test 3: Class functionality
        print("\n3. Testing class functionality...")
        class_test = "tests/unit/simple_class_test.ts"
        if os.path.exists(class_test):
            compiles = self.test_compilation(class_test)
            print(f"   Class compilation: {'✓' if compiles else '✗'}")
        else:
            print("   Class test file not found")
        
        print("\nQuick validation complete!")

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Quick compiler validation')
    parser.add_argument('--compiler', default='./build/tsc',
                       help='Path to the TypeScript compiler')
    parser.add_argument('--full', action='store_true',
                       help='Run full core test validation')
    
    args = parser.parse_args()
    
    validator = SimpleTestValidator(args.compiler)
    
    if args.full:
        validator.validate_core_tests()
    else:
        validator.quick_validation()

if __name__ == "__main__":
    main()
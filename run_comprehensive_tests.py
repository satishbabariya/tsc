#!/usr/bin/env python3
"""
Comprehensive Test Runner - Runs all test suites with proper configuration
"""

import json
import sys
import os
import time
from pathlib import Path
from integrated_test_runner import IntegratedTestRunner
from llvm_validation_suite import LLVMValidationSuite, ValidationLevel

class ComprehensiveTestRunner:
    def __init__(self, config_file: str = "test_config.json"):
        self.config = self._load_config(config_file)
        self.results = {}
    
    def _load_config(self, config_file: str) -> dict:
        """Load test configuration from JSON file"""
        try:
            with open(config_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Warning: Config file {config_file} not found, using defaults")
            return {
                "test_suites": {
                    "core_functionality": {
                        "files": ["tests/unit/ultra_simple_test.ts"],
                        "validation_level": "basic"
                    }
                }
            }
    
    def run_test_suite(self, suite_name: str) -> dict:
        """Run a specific test suite"""
        if suite_name not in self.config["test_suites"]:
            raise ValueError(f"Test suite '{suite_name}' not found in configuration")
        
        suite_config = self.config["test_suites"][suite_name]
        validation_level = ValidationLevel(suite_config["validation_level"])
        
        print(f"\n{'='*60}")
        print(f"RUNNING TEST SUITE: {suite_name.upper()}")
        print(f"Description: {suite_config.get('description', 'No description')}")
        print(f"Validation Level: {validation_level.value}")
        print(f"Files: {len(suite_config['files'])}")
        print('='*60)
        
        runner = IntegratedTestRunner(validation_level=validation_level)
        suite_results = {
            'suite_name': suite_name,
            'description': suite_config.get('description', ''),
            'validation_level': validation_level.value,
            'files': suite_config['files'],
            'results': [],
            'summary': {}
        }
        
        for test_file in suite_config['files']:
            if os.path.exists(test_file):
                result = runner.run_test(test_file)
                suite_results['results'].append(result)
            else:
                print(f"Warning: Test file {test_file} not found")
                suite_results['results'].append({
                    'file': test_file,
                    'compilation_success': False,
                    'execution_success': False,
                    'llvm_validation_success': False,
                    'errors': [f"File not found: {test_file}"],
                    'warnings': []
                })
        
        # Calculate summary
        total_tests = len(suite_results['results'])
        compilation_success = sum(1 for r in suite_results['results'] if r['compilation_success'])
        validation_success = sum(1 for r in suite_results['results'] if r['llvm_validation_success'])
        execution_success = sum(1 for r in suite_results['results'] if r['execution_success'])
        
        suite_results['summary'] = {
            'total_tests': total_tests,
            'compilation_success': compilation_success,
            'compilation_rate': compilation_success / total_tests * 100 if total_tests > 0 else 0,
            'validation_success': validation_success,
            'validation_rate': validation_success / total_tests * 100 if total_tests > 0 else 0,
            'execution_success': execution_success,
            'execution_rate': execution_success / total_tests * 100 if total_tests > 0 else 0
        }
        
        self.results[suite_name] = suite_results
        return suite_results
    
    def run_all_suites(self) -> dict:
        """Run all configured test suites"""
        print("COMPREHENSIVE TEST SUITE EXECUTION")
        print("=" * 60)
        print(f"Configuration loaded from: test_config.json")
        print(f"Test suites: {len(self.config['test_suites'])}")
        
        all_results = {}
        overall_summary = {
            'total_suites': 0,
            'total_tests': 0,
            'total_compilation_success': 0,
            'total_validation_success': 0,
            'total_execution_success': 0
        }
        
        for suite_name in self.config['test_suites']:
            suite_result = self.run_test_suite(suite_name)
            all_results[suite_name] = suite_result
            
            # Update overall summary
            overall_summary['total_suites'] += 1
            overall_summary['total_tests'] += suite_result['summary']['total_tests']
            overall_summary['total_compilation_success'] += suite_result['summary']['compilation_success']
            overall_summary['total_validation_success'] += suite_result['summary']['validation_success']
            overall_summary['total_execution_success'] += suite_result['summary']['execution_success']
        
        # Calculate overall rates
        if overall_summary['total_tests'] > 0:
            overall_summary['compilation_rate'] = overall_summary['total_compilation_success'] / overall_summary['total_tests'] * 100
            overall_summary['validation_rate'] = overall_summary['total_validation_success'] / overall_summary['total_tests'] * 100
            overall_summary['execution_rate'] = overall_summary['total_execution_success'] / overall_summary['total_tests'] * 100
        
        return {
            'suites': all_results,
            'overall_summary': overall_summary
        }
    
    def generate_comprehensive_report(self) -> str:
        """Generate a comprehensive report of all test results"""
        if not self.results:
            return "No test results to report"
        
        report = []
        report.append("\n" + "="*80)
        report.append("COMPREHENSIVE TEST SUITE REPORT")
        report.append("="*80)
        
        # Overall summary
        total_suites = len(self.results)
        total_tests = sum(s['summary']['total_tests'] for s in self.results.values())
        total_compilation_success = sum(s['summary']['compilation_success'] for s in self.results.values())
        total_validation_success = sum(s['summary']['validation_success'] for s in self.results.values())
        total_execution_success = sum(s['summary']['execution_success'] for s in self.results.values())
        
        report.append(f"Total Test Suites: {total_suites}")
        report.append(f"Total Tests: {total_tests}")
        report.append(f"Compilation Success: {total_compilation_success}/{total_tests} ({total_compilation_success/total_tests*100:.1f}%)")
        report.append(f"Validation Success: {total_validation_success}/{total_tests} ({total_validation_success/total_tests*100:.1f}%)")
        report.append(f"Execution Success: {total_execution_success}/{total_tests} ({total_execution_success/total_tests*100:.1f}%)")
        
        # Suite-by-suite results
        report.append("\nTest Suite Results:")
        report.append("-" * 60)
        
        for suite_name, suite_result in self.results.items():
            summary = suite_result['summary']
            report.append(f"\n{suite_name.upper()}:")
            report.append(f"  Description: {suite_result['description']}")
            report.append(f"  Validation Level: {suite_result['validation_level']}")
            report.append(f"  Tests: {summary['total_tests']}")
            report.append(f"  Compilation: {summary['compilation_success']}/{summary['total_tests']} ({summary['compilation_rate']:.1f}%)")
            report.append(f"  Validation: {summary['validation_success']}/{summary['total_tests']} ({summary['validation_rate']:.1f}%)")
            report.append(f"  Execution: {summary['execution_success']}/{summary['total_tests']} ({summary['execution_rate']:.1f}%)")
            
            # Show failing tests
            failing_tests = [r for r in suite_result['results'] if not (r['compilation_success'] and r['llvm_validation_success'])]
            if failing_tests:
                report.append(f"  Failing Tests ({len(failing_tests)}):")
                for test in failing_tests[:3]:  # Show first 3 failures
                    report.append(f"    ✗ {test['file']}")
                    if test['errors']:
                        report.append(f"      ERROR: {test['errors'][0]}")
                if len(failing_tests) > 3:
                    report.append(f"    ... and {len(failing_tests) - 3} more failures")
        
        return "\n".join(report)
    
    def export_results(self, output_file: str):
        """Export all results to JSON"""
        with open(output_file, 'w') as f:
            json.dump(self.results, f, indent=2)
        print(f"Results exported to: {output_file}")

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Comprehensive Test Runner')
    parser.add_argument('--suite', help='Run specific test suite')
    parser.add_argument('--config', default='test_config.json', help='Configuration file')
    parser.add_argument('--export', help='Export results to JSON file')
    parser.add_argument('--report', help='Save report to file')
    
    args = parser.parse_args()
    
    runner = ComprehensiveTestRunner(args.config)
    
    if args.suite:
        result = runner.run_test_suite(args.suite)
        print(f"\nSuite '{args.suite}' completed")
        summary = result['summary']
        print(f"Compilation: {summary['compilation_success']}/{summary['total_tests']} ({summary['compilation_rate']:.1f}%)")
        print(f"Validation: {summary['validation_success']}/{summary['total_tests']} ({summary['validation_rate']:.1f}%)")
        print(f"Execution: {summary['execution_success']}/{summary['total_tests']} ({summary['execution_rate']:.1f}%)")
    else:
        runner.run_all_suites()
        report = runner.generate_comprehensive_report()
        print(report)
        
        if args.report:
            with open(args.report, 'w') as f:
                f.write(report)
            print(f"Report saved to: {args.report}")
    
    if args.export:
        runner.export_results(args.export)

if __name__ == "__main__":
    main()
#!/usr/bin/env python3
"""
Error Pattern Analyzer - Identifies and categorizes common compilation errors
"""

import re
import json
from collections import Counter, defaultdict
from typing import Dict, List, Tuple

class ErrorPatternAnalyzer:
    def __init__(self):
        self.error_patterns = Counter()
        self.error_categories = defaultdict(list)
        self.file_errors = defaultdict(list)
    
    def analyze_error_output(self, error_text: str, file_path: str = ""):
        """Analyze error output and categorize patterns"""
        lines = error_text.split('\n')
        
        for line in lines:
            # Look for error patterns
            if 'error:' in line:
                # Extract error message
                error_match = re.search(r'error:\s*(.+)', line)
                if error_match:
                    error_msg = error_match.group(1).strip()
                    self.error_patterns[error_msg] += 1
                    self.error_categories[self._categorize_error(error_msg)].append(error_msg)
                    if file_path:
                        self.file_errors[file_path].append(error_msg)
    
    def _categorize_error(self, error_msg: str) -> str:
        """Categorize error messages into common types"""
        if 'Expected' in error_msg and 'after' in error_msg:
            return 'syntax_expected'
        elif 'Unexpected token' in error_msg:
            return 'syntax_unexpected'
        elif 'not found' in error_msg:
            return 'symbol_not_found'
        elif 'not assignable' in error_msg:
            return 'type_assignment'
        elif 'Invalid declaration syntax' in error_msg:
            return 'declaration_syntax'
        elif 'Expected type name' in error_msg:
            return 'type_syntax'
        elif 'Expected parameter' in error_msg:
            return 'parameter_syntax'
        elif 'Expected property' in error_msg:
            return 'property_syntax'
        else:
            return 'other'
    
    def get_most_common_errors(self, n: int = 10) -> List[Tuple[str, int]]:
        """Get the most common error patterns"""
        return self.error_patterns.most_common(n)
    
    def get_error_categories(self) -> Dict[str, List[str]]:
        """Get errors grouped by category"""
        return dict(self.error_categories)
    
    def generate_report(self) -> str:
        """Generate a comprehensive error analysis report"""
        report = []
        report.append("COMPILATION ERROR ANALYSIS REPORT")
        report.append("=" * 50)
        
        # Most common errors
        report.append("\nMost Common Errors:")
        report.append("-" * 30)
        for error, count in self.get_most_common_errors(15):
            report.append(f"{count:3d}x {error}")
        
        # Error categories
        report.append("\nError Categories:")
        report.append("-" * 30)
        categories = self.get_error_categories()
        for category, errors in categories.items():
            report.append(f"\n{category.upper()}:")
            for error in errors[:5]:  # Show first 5 in each category
                report.append(f"  - {error}")
            if len(errors) > 5:
                report.append(f"  ... and {len(errors) - 5} more")
        
        return "\n".join(report)

def main():
    # Sample error analysis from our test results
    sample_errors = """
tests/unit/angular_service_test.ts:8:18: error: Expected type name [TS1007]
tests/unit/angular_service_test.ts:8:18: error: Expected ';' after interface property [TS1007]
tests/unit/angular_service_test.ts:9:12: error: Unexpected token in expression [TS1007]
tests/unit/angular_service_test.ts:17:18: error: Expected parameter name [TS1007]
tests/unit/angular_service_test.ts:22:54: error: Expected property name after '.' [TS1007]
tests/unit/arc_design_patterns.ts:48:64: error: Expected '>' after generic type arguments [TS1007]
tests/unit/arc_design_patterns.ts:50:16: error: Unexpected token in expression [TS1007]
tests/unit/arc_design_patterns.ts:56:33: error: Expected property name after '.' [TS1007]
tests/unit/arc_design_patterns.ts:85:16: error: Expected type name [TS1007]
tests/unit/arc_design_patterns.ts:87:28: error: Expected type name [TS1007]
tests/unit/arc_design_patterns.ts:98:57: error: Expected '>' after generic type arguments [TS1007]
tests/unit/arc_design_patterns.ts:100:16: error: Unexpected token in expression [TS1007]
tests/unit/arc_design_patterns.ts:105:42: error: Expected property name after '.' [TS1007]
tests/unit/arc_regression_tests.ts:22:40: error: Expected ';' after variable declaration [TS1007]
tests/unit/arc_regression_tests.ts:29:2: error: Unexpected token in expression [TS1007]
tests/unit/arc_regression_tests.ts:107:14: error: Expected property name after '.' [TS1007]
tests/unit/arc_regression_tests.ts:136:44: error: Expected '>' after generic type arguments [TS1007]
tests/unit/arc_regression_tests.ts:156:32: error: Expected type name [TS1007]
tests/unit/arc_regression_tests.ts:161:18: error: Expected property name after '.' [TS1007]
tests/unit/arc_regression_tests.ts:182:47: error: Expected ';' after variable declaration [TS1007]
tests/unit/arc_regression_tests.ts:212:25: error: Unexpected token in expression [TS1007]
tests/unit/arc_regression_tests.ts:239:30: error: Expected '>' after type arguments [TS1007]
tests/unit/arc_regression_tests.ts:327:14: error: Expected property name after '.' [TS1007]
    """
    
    analyzer = ErrorPatternAnalyzer()
    analyzer.analyze_error_output(sample_errors)
    
    print(analyzer.generate_report())
    
    # Save detailed analysis
    analysis_data = {
        'most_common_errors': analyzer.get_most_common_errors(20),
        'error_categories': analyzer.get_error_categories(),
        'file_errors': dict(analyzer.file_errors)
    }
    
    with open('error_analysis.json', 'w') as f:
        json.dump(analysis_data, f, indent=2)
    
    print(f"\nDetailed analysis saved to: error_analysis.json")

if __name__ == "__main__":
    main()
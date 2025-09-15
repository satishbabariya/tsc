#include "tsc/testing/CodeTransformationFramework.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>

namespace tsc {
namespace testing {

class ErrorReportingValidator {
public:
    struct ValidationResult {
        String testName;
        bool passed;
        std::vector<String> detectedErrors;
        std::vector<String> detectedWarnings;
        std::vector<String> expectedErrors;
        std::vector<String> missingErrors;
        std::vector<String> unexpectedErrors;
        double executionTime;
        String compilerOutput;
    };
    
    struct CoverageReport {
        String component;
        int totalErrorPaths;
        int testedErrorPaths;
        double coveragePercentage;
        std::vector<String> untestedPaths;
    };
    
    ErrorReportingValidator() {
        validator_ = createTestValidator();
    }
    
    void validateAllErrorReporting() {
        std::cout << "=== Comprehensive Error Reporting Validation ===" << std::endl;
        
        // Validate parser error reporting
        validateParserErrorReporting();
        
        // Validate semantic error reporting
        validateSemanticErrorReporting();
        
        // Validate type checker error reporting
        validateTypeCheckerErrorReporting();
        
        // Validate code generation error reporting
        validateCodeGenErrorReporting();
        
        // Validate cross-phase error reporting
        validateCrossPhaseErrorReporting();
        
        // Generate coverage report
        generateCoverageReport();
        
        std::cout << "=== Validation Complete ===" << std::endl;
    }
    
private:
    unique_ptr<TestValidator> validator_;
    std::vector<ValidationResult> results_;
    std::vector<CoverageReport> coverageReports_;
    
    void validateParserErrorReporting() {
        std::cout << "\n--- Validating Parser Error Reporting ---" << std::endl;
        
        std::vector<std::pair<String, std::vector<String>>> parserTests = {
            {"parser_missing_semicolon_1.ts", {"Expected ';' after variable declaration"}},
            {"parser_missing_semicolon_2.ts", {"Expected ';' after expression"}},
            {"parser_missing_semicolon_3.ts", {"Expected ';' after return statement"}},
            {"parser_missing_brace_1.ts", {"Expected '}' after block"}},
            {"parser_missing_brace_2.ts", {"Expected '}' after block"}},
            {"parser_missing_parenthesis_1.ts", {"Expected ')' after function call"}},
            {"parser_missing_parenthesis_2.ts", {"Expected ')' after function call"}},
            {"parser_invalid_expression_1.ts", {"Invalid binary operation"}},
            {"parser_invalid_expression_2.ts", {"Invalid unary operation"}},
            {"parser_malformed_statement_1.ts", {"Expected '}' after block"}}
        };
        
        for (const auto& test : parserTests) {
            ValidationResult result = runValidationTest(test.first, test.second);
            results_.push_back(result);
            printValidationResult(result);
        }
    }
    
    void validateSemanticErrorReporting() {
        std::cout << "\n--- Validating Semantic Error Reporting ---" << std::endl;
        
        std::vector<std::pair<String, std::vector<String>>> semanticTests = {
            {"semantic_undefined_1.ts", {"Undefined symbol"}},
            {"semantic_undefined_2.ts", {"Undefined symbol"}},
            {"semantic_duplicate_1.ts", {"Redefinition of symbol"}},
            {"semantic_duplicate_2.ts", {"Redefinition of symbol"}},
            {"semantic_scope_1.ts", {"Private property access"}},
            {"semantic_scope_2.ts", {"'this' used outside of method context"}},
            {"semantic_function_1.ts", {"Wrong number of arguments"}},
            {"semantic_function_2.ts", {"Wrong argument types"}}
        };
        
        for (const auto& test : semanticTests) {
            ValidationResult result = runValidationTest(test.first, test.second);
            results_.push_back(result);
            printValidationResult(result);
        }
    }
    
    void validateTypeCheckerErrorReporting() {
        std::cout << "\n--- Validating Type Checker Error Reporting ---" << std::endl;
        
        std::vector<std::pair<String, std::vector<String>>> typeTests = {
            {"type_mismatch_1.ts", {"Type mismatch"}},
            {"type_mismatch_2.ts", {"Invalid binary operation"}},
            {"type_mismatch_3.ts", {"Type mismatch"}},
            {"type_incompatible_1.ts", {"Invalid binary operation"}},
            {"type_incompatible_2.ts", {"Invalid unary operation"}},
            {"type_missing_1.ts", {"Missing type annotation"}},
            {"type_missing_2.ts", {"Missing function return type"}},
            {"type_invalid_1.ts", {"Undefined type"}},
            {"type_invalid_2.ts", {"Circular type reference"}}
        };
        
        for (const auto& test : typeTests) {
            ValidationResult result = runValidationTest(test.first, test.second);
            results_.push_back(result);
            printValidationResult(result);
        }
    }
    
    void validateCodeGenErrorReporting() {
        std::cout << "\n--- Validating Code Generation Error Reporting ---" << std::endl;
        
        std::vector<std::pair<String, std::vector<String>>> codeGenTests = {
            {"codegen_llvm_1.ts", {"LLVM generation error"}},
            {"codegen_llvm_2.ts", {"LLVM generation error"}},
            {"codegen_memory_1.ts", {"Memory management error"}},
            {"codegen_memory_2.ts", {"Memory management error"}},
            {"codegen_optimization_1.ts", {"Optimization error"}},
            {"codegen_optimization_2.ts", {"Optimization error"}}
        };
        
        for (const auto& test : codeGenTests) {
            ValidationResult result = runValidationTest(test.first, test.second);
            results_.push_back(result);
            printValidationResult(result);
        }
    }
    
    void validateCrossPhaseErrorReporting() {
        std::cout << "\n--- Validating Cross-Phase Error Reporting ---" << std::endl;
        
        std::vector<std::pair<String, std::vector<String>>> crossPhaseTests = {
            {"integration_cross_phase_1.ts", {"Expected ';'", "Type mismatch"}},
            {"integration_cross_phase_2.ts", {"Type mismatch", "Code generation error"}},
            {"integration_cross_phase_3.ts", {"Semantic error", "Type mismatch"}},
            {"integration_cross_phase_4.ts", {"Type error", "Semantic error", "Code generation error"}},
            {"integration_complex_1.ts", {"Type error", "Semantic error"}},
            {"integration_complex_2.ts", {"Type error", "Semantic error"}},
            {"integration_performance_1.ts", {"Type error", "Semantic error"}},
            {"integration_performance_2.ts", {"Memory stress", "Semantic error"}}
        };
        
        for (const auto& test : crossPhaseTests) {
            ValidationResult result = runValidationTest(test.first, test.second);
            results_.push_back(result);
            printValidationResult(result);
        }
    }
    
    ValidationResult runValidationTest(const String& filename, const std::vector<String>& expectedErrors) {
        ValidationResult result;
        result.testName = filename;
        result.expectedErrors = expectedErrors;
        result.passed = false;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run the compiler
        String command = "./build/tsc " + filename + " 2>&1";
        String output = executeCommand(command);
        
        auto end = std::chrono::high_resolution_clock::now();
        result.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        result.compilerOutput = output;
        
        // Parse errors and warnings
        parseCompilerOutput(output, result.detectedErrors, result.detectedWarnings);
        
        // Check for expected errors
        for (const String& expectedError : expectedErrors) {
            bool found = false;
            for (const String& detectedError : result.detectedErrors) {
                if (detectedError.find(expectedError) != String::npos) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.missingErrors.push_back(expectedError);
            }
        }
        
        // Check for unexpected errors
        for (const String& detectedError : result.detectedErrors) {
            bool expected = false;
            for (const String& expectedError : expectedErrors) {
                if (detectedError.find(expectedError) != String::npos) {
                    expected = true;
                    break;
                }
            }
            if (!expected) {
                result.unexpectedErrors.push_back(detectedError);
            }
        }
        
        // Determine if test passed
        result.passed = result.missingErrors.empty() && result.unexpectedErrors.empty();
        
        return result;
    }
    
    String executeCommand(const String& command) {
        String output;
        FILE* pipe = popen(command.c_str(), "r");
        if (pipe) {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                output += buffer;
            }
            pclose(pipe);
        }
        return output;
    }
    
    void parseCompilerOutput(const String& output, std::vector<String>& errors, std::vector<String>& warnings) {
        std::istringstream stream(output);
        String line;
        
        while (std::getline(stream, line)) {
            if (line.find("error:") != String::npos) {
                errors.push_back(line);
            } else if (line.find("warning:") != String::npos) {
                warnings.push_back(line);
            }
        }
    }
    
    void printValidationResult(const ValidationResult& result) {
        std::cout << "\nTest: " << result.testName << std::endl;
        std::cout << "Status: " << (result.passed ? "PASSED" : "FAILED") << std::endl;
        std::cout << "Execution time: " << result.executionTime << " ms" << std::endl;
        std::cout << "Errors detected: " << result.detectedErrors.size() << std::endl;
        std::cout << "Warnings detected: " << result.detectedWarnings.size() << std::endl;
        
        if (!result.missingErrors.empty()) {
            std::cout << "Missing expected errors:" << std::endl;
            for (const String& error : result.missingErrors) {
                std::cout << "  - " << error << std::endl;
            }
        }
        
        if (!result.unexpectedErrors.empty()) {
            std::cout << "Unexpected errors:" << std::endl;
            for (const String& error : result.unexpectedErrors) {
                std::cout << "  - " << error << std::endl;
            }
        }
    }
    
    void generateCoverageReport() {
        std::cout << "\n=== Error Reporting Coverage Report ===" << std::endl;
        
        // Analyze parser coverage
        analyzeParserCoverage();
        
        // Analyze semantic coverage
        analyzeSemanticCoverage();
        
        // Analyze type checker coverage
        analyzeTypeCheckerCoverage();
        
        // Analyze code generation coverage
        analyzeCodeGenCoverage();
        
        // Print overall coverage summary
        printOverallCoverage();
    }
    
    void analyzeParserCoverage() {
        CoverageReport report;
        report.component = "Parser";
        report.totalErrorPaths = 25; // Estimated total parser error paths
        report.testedErrorPaths = 0;
        
        // Count tested parser error paths
        for (const ValidationResult& result : results_) {
            if (result.testName.find("parser_") != String::npos) {
                report.testedErrorPaths += result.detectedErrors.size();
            }
        }
        
        report.coveragePercentage = (double)report.testedErrorPaths / report.totalErrorPaths * 100.0;
        coverageReports_.push_back(report);
        
        std::cout << "\nParser Coverage:" << std::endl;
        std::cout << "  Tested paths: " << report.testedErrorPaths << "/" << report.totalErrorPaths << std::endl;
        std::cout << "  Coverage: " << report.coveragePercentage << "%" << std::endl;
    }
    
    void analyzeSemanticCoverage() {
        CoverageReport report;
        report.component = "Semantic Analyzer";
        report.totalErrorPaths = 30; // Estimated total semantic error paths
        report.testedErrorPaths = 0;
        
        // Count tested semantic error paths
        for (const ValidationResult& result : results_) {
            if (result.testName.find("semantic_") != String::npos) {
                report.testedErrorPaths += result.detectedErrors.size();
            }
        }
        
        report.coveragePercentage = (double)report.testedErrorPaths / report.totalErrorPaths * 100.0;
        coverageReports_.push_back(report);
        
        std::cout << "\nSemantic Analyzer Coverage:" << std::endl;
        std::cout << "  Tested paths: " << report.testedErrorPaths << "/" << report.totalErrorPaths << std::endl;
        std::cout << "  Coverage: " << report.coveragePercentage << "%" << std::endl;
    }
    
    void analyzeTypeCheckerCoverage() {
        CoverageReport report;
        report.component = "Type Checker";
        report.totalErrorPaths = 35; // Estimated total type checker error paths
        report.testedErrorPaths = 0;
        
        // Count tested type checker error paths
        for (const ValidationResult& result : results_) {
            if (result.testName.find("type_") != String::npos) {
                report.testedErrorPaths += result.detectedErrors.size();
            }
        }
        
        report.coveragePercentage = (double)report.testedErrorPaths / report.totalErrorPaths * 100.0;
        coverageReports_.push_back(report);
        
        std::cout << "\nType Checker Coverage:" << std::endl;
        std::cout << "  Tested paths: " << report.testedErrorPaths << "/" << report.totalErrorPaths << std::endl;
        std::cout << "  Coverage: " << report.coveragePercentage << "%" << std::endl;
    }
    
    void analyzeCodeGenCoverage() {
        CoverageReport report;
        report.component = "Code Generation";
        report.totalErrorPaths = 20; // Estimated total code generation error paths
        report.testedErrorPaths = 0;
        
        // Count tested code generation error paths
        for (const ValidationResult& result : results_) {
            if (result.testName.find("codegen_") != String::npos) {
                report.testedErrorPaths += result.detectedErrors.size();
            }
        }
        
        report.coveragePercentage = (double)report.testedErrorPaths / report.totalErrorPaths * 100.0;
        coverageReports_.push_back(report);
        
        std::cout << "\nCode Generation Coverage:" << std::endl;
        std::cout << "  Tested paths: " << report.testedErrorPaths << "/" << report.totalErrorPaths << std::endl;
        std::cout << "  Coverage: " << report.coveragePercentage << "%" << std::endl;
    }
    
    void printOverallCoverage() {
        std::cout << "\n=== Overall Coverage Summary ===" << std::endl;
        
        int totalPaths = 0;
        int totalTestedPaths = 0;
        
        for (const CoverageReport& report : coverageReports_) {
            totalPaths += report.totalErrorPaths;
            totalTestedPaths += report.testedErrorPaths;
        }
        
        double overallCoverage = (double)totalTestedPaths / totalPaths * 100.0;
        
        std::cout << "Total error paths: " << totalPaths << std::endl;
        std::cout << "Total tested paths: " << totalTestedPaths << std::endl;
        std::cout << "Overall coverage: " << overallCoverage << "%" << std::endl;
        
        // Print detailed results
        std::cout << "\n=== Detailed Results ===" << std::endl;
        
        int totalTests = results_.size();
        int passedTests = 0;
        int totalErrors = 0;
        int totalWarnings = 0;
        double totalTime = 0.0;
        
        for (const ValidationResult& result : results_) {
            if (result.passed) {
                passedTests++;
            }
            totalErrors += result.detectedErrors.size();
            totalWarnings += result.detectedWarnings.size();
            totalTime += result.executionTime;
        }
        
        std::cout << "Total tests: " << totalTests << std::endl;
        std::cout << "Passed tests: " << passedTests << std::endl;
        std::cout << "Failed tests: " << (totalTests - passedTests) << std::endl;
        std::cout << "Success rate: " << (100.0 * passedTests / totalTests) << "%" << std::endl;
        std::cout << "Total errors detected: " << totalErrors << std::endl;
        std::cout << "Total warnings detected: " << totalWarnings << std::endl;
        std::cout << "Total execution time: " << totalTime << " ms" << std::endl;
        std::cout << "Average execution time: " << (totalTime / totalTests) << " ms" << std::endl;
    }
};

// Main function for validation
int validateErrorReporting() {
    try {
        ErrorReportingValidator validator;
        validator.validateAllErrorReporting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error during validation: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace testing
} // namespace tsc
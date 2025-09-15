#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <chrono>

class ErrorReportingValidator {
private:
    struct TestResult {
        std::string testName;
        bool passed;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        std::string compilerOutput;
        double executionTime;
    };
    
    std::vector<TestResult> results_;
    
public:
    void runAllTests() {
        std::cout << "=== Error Reporting Validation Suite ===" << std::endl;
        
        // Test cases with expected error patterns
        std::vector<std::pair<std::string, std::vector<std::string>>> testCases = {
            {"syntax_error_test.ts", {
                "Expected ';' after variable declaration",
                "Expected ';' after expression",
                "Expected ']' after array elements"
            }},
            {"type_error_test.ts", {
                "Invalid binary operation between number and boolean",
                "Type mismatch",
                "Incompatible types"
            }},
            {"semantic_error_test.ts", {
                "Undefined symbol:",
                "Redefinition of symbol:",
                "Failed to declare variable:"
            }},
            {"control_flow_error_test.ts", {
                "Expected '}' after block",
                "Expected ')' after condition",
                "Expected ':' after case"
            }},
            {"nesting_stress_test.ts", {
                // Should compile successfully or show specific errors
            }},
            {"long_expression_test.ts", {
                // Should compile successfully or show specific errors
            }},
            {"large_number_test.ts", {
                // Should compile successfully or show specific errors
            }},
            {"unusual_text_test.ts", {
                // Should compile successfully or show specific errors
            }}
        };
        
        for (const auto& testCase : testCases) {
            runTest(testCase.first, testCase.second);
        }
        
        generateReport();
    }
    
private:
    void runTest(const std::string& filename, const std::vector<std::string>& expectedErrors) {
        std::cout << "\n--- Testing " << filename << " ---" << std::endl;
        
        TestResult result;
        result.testName = filename;
        result.passed = false;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run the compiler
        std::string command = "./build/tsc " + filename + " 2>&1";
        std::string output = executeCommand(command);
        
        auto end = std::chrono::high_resolution_clock::now();
        result.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        result.compilerOutput = output;
        
        // Parse errors and warnings
        parseCompilerOutput(output, result.errors, result.warnings);
        
        // Validate expected errors
        bool allExpectedErrorsFound = true;
        for (const std::string& expectedError : expectedErrors) {
            bool found = false;
            for (const std::string& actualError : result.errors) {
                if (actualError.find(expectedError) != std::string::npos) {
                    found = true;
                    break;
                }
            }
            if (!found && !expectedError.empty()) {
                allExpectedErrorsFound = false;
                std::cout << "❌ Expected error not found: " << expectedError << std::endl;
            }
        }
        
        // Validate error locations
        bool validLocations = validateErrorLocations(result.errors);
        
        // Validate error messages
        bool validMessages = validateErrorMessageFormat(result.errors);
        
        // Determine if test passed
        result.passed = allExpectedErrorsFound && validLocations && validMessages;
        
        if (result.passed) {
            std::cout << "✅ Test PASSED" << std::endl;
        } else {
            std::cout << "❌ Test FAILED" << std::endl;
        }
        
        std::cout << "   Errors: " << result.errors.size() << std::endl;
        std::cout << "   Warnings: " << result.warnings.size() << std::endl;
        std::cout << "   Execution time: " << result.executionTime << " ms" << std::endl;
        
        results_.push_back(result);
    }
    
    std::string executeCommand(const std::string& command) {
        std::string output;
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
    
    void parseCompilerOutput(const std::string& output, std::vector<std::string>& errors, std::vector<std::string>& warnings) {
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            if (line.find("error:") != std::string::npos) {
                errors.push_back(line);
            } else if (line.find("warning:") != std::string::npos) {
                warnings.push_back(line);
            }
        }
    }
    
    bool validateErrorLocations(const std::vector<std::string>& errors) {
        std::regex locationPattern(R"(([^:]+):(\d+):(\d+):)");
        
        for (const std::string& error : errors) {
            std::smatch match;
            if (!std::regex_search(error, match, locationPattern)) {
                std::cout << "❌ Invalid error location format: " << error << std::endl;
                return false;
            }
            
            // Validate line and column numbers
            int line = std::stoi(match[2].str());
            int column = std::stoi(match[3].str());
            
            if (line < 1 || column < 1) {
                std::cout << "❌ Invalid line/column numbers: " << error << std::endl;
                return false;
            }
        }
        
        return true;
    }
    
    bool validateErrorMessageFormat(const std::vector<std::string>& errors) {
        for (const std::string& error : errors) {
            // Check for proper error message format
            if (error.find("error:") == std::string::npos) {
                std::cout << "❌ Missing 'error:' prefix: " << error << std::endl;
                return false;
            }
            
            // Check for meaningful error messages
            size_t errorPos = error.find("error:");
            if (errorPos != std::string::npos) {
                std::string message = error.substr(errorPos + 6);
                if (message.empty() || message.find_first_not_of(" \t") == std::string::npos) {
                    std::cout << "❌ Empty or meaningless error message: " << error << std::endl;
                    return false;
                }
            }
        }
        
        return true;
    }
    
    void generateReport() {
        std::cout << "\n=== Validation Report ===" << std::endl;
        
        int totalTests = results_.size();
        int passedTests = 0;
        int totalErrors = 0;
        int totalWarnings = 0;
        double totalTime = 0.0;
        
        for (const TestResult& result : results_) {
            if (result.passed) {
                passedTests++;
            }
            totalErrors += result.errors.size();
            totalWarnings += result.warnings.size();
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
        
        // Detailed results
        std::cout << "\n=== Detailed Results ===" << std::endl;
        for (const TestResult& result : results_) {
            std::cout << "\n" << result.testName << ":" << std::endl;
            std::cout << "  Status: " << (result.passed ? "PASSED" : "FAILED") << std::endl;
            std::cout << "  Errors: " << result.errors.size() << std::endl;
            std::cout << "  Warnings: " << result.warnings.size() << std::endl;
            std::cout << "  Time: " << result.executionTime << " ms" << std::endl;
            
            if (!result.errors.empty()) {
                std::cout << "  Error details:" << std::endl;
                for (const std::string& error : result.errors) {
                    std::cout << "    " << error << std::endl;
                }
            }
        }
        
        // Performance analysis
        analyzePerformance();
        
        // Error pattern analysis
        analyzeErrorPatterns();
    }
    
    void analyzePerformance() {
        std::cout << "\n=== Performance Analysis ===" << std::endl;
        
        double minTime = std::numeric_limits<double>::max();
        double maxTime = 0.0;
        double avgTime = 0.0;
        
        for (const TestResult& result : results_) {
            minTime = std::min(minTime, result.executionTime);
            maxTime = std::max(maxTime, result.executionTime);
            avgTime += result.executionTime;
        }
        
        avgTime /= results_.size();
        
        std::cout << "Min execution time: " << minTime << " ms" << std::endl;
        std::cout << "Max execution time: " << maxTime << " ms" << std::endl;
        std::cout << "Average execution time: " << avgTime << " ms" << std::endl;
        
        // Identify slow tests
        std::cout << "\nSlow tests (>1000ms):" << std::endl;
        for (const TestResult& result : results_) {
            if (result.executionTime > 1000.0) {
                std::cout << "  " << result.testName << ": " << result.executionTime << " ms" << std::endl;
            }
        }
    }
    
    void analyzeErrorPatterns() {
        std::cout << "\n=== Error Pattern Analysis ===" << std::endl;
        
        std::map<std::string, int> errorPatterns;
        
        for (const TestResult& result : results_) {
            for (const std::string& error : result.errors) {
                // Extract error type
                size_t errorPos = error.find("error:");
                if (errorPos != std::string::npos) {
                    std::string errorType = error.substr(errorPos + 6);
                    // Remove leading whitespace
                    errorType.erase(0, errorType.find_first_not_of(" \t"));
                    // Take first word
                    size_t spacePos = errorType.find(' ');
                    if (spacePos != std::string::npos) {
                        errorType = errorType.substr(0, spacePos);
                    }
                    errorPatterns[errorType]++;
                }
            }
        }
        
        std::cout << "Most common error types:" << std::endl;
        for (const auto& pattern : errorPatterns) {
            std::cout << "  " << pattern.first << ": " << pattern.second << " occurrences" << std::endl;
        }
    }
};

int main() {
    try {
        ErrorReportingValidator validator;
        validator.runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
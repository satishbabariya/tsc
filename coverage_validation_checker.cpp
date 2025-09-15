#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <filesystem>

// Coverage Validation and Accuracy Checker
class CoverageValidationChecker {
public:
    struct CoverageValidation {
        std::string filename;
        double expectedCoverage;
        double actualCoverage;
        bool passed;
        std::vector<std::string> discrepancies;
        std::vector<std::string> recommendations;
    };
    
    struct FileCoverageTarget {
        std::string filename;
        double targetCoverage;
        std::string component;
        std::vector<std::string> criticalLines;
    };
    
    void validateCoverageAccuracy() {
        std::cout << "=== Coverage Validation and Accuracy Check ===" << std::endl;
        
        // Step 1: Define expected coverage targets
        defineCoverageTargets();
        
        // Step 2: Analyze actual coverage data
        analyzeActualCoverage();
        
        // Step 3: Compare expected vs actual
        compareCoverageData();
        
        // Step 4: Identify discrepancies
        identifyDiscrepancies();
        
        // Step 5: Generate recommendations
        generateRecommendations();
        
        // Step 6: Generate validation report
        generateValidationReport();
        
        std::cout << "=== Coverage Validation Complete ===" << std::endl;
    }
    
private:
    std::vector<FileCoverageTarget> coverageTargets_;
    std::vector<CoverageValidation> validations_;
    
    void defineCoverageTargets() {
        std::cout << "\n--- Defining Coverage Targets ---" << std::endl;
        
        // Define coverage targets for individual files
        coverageTargets_ = {
            // Parser files
            {"src/parser/Parser.cpp", 85.0, "Parser", {"parseExpression", "parseStatement", "parseDeclaration"}},
            {"src/parser/Parser.h", 90.0, "Parser", {"class Parser", "parseFunction", "parseClass"}},
            
            // Lexer files
            {"src/lexer/Lexer.cpp", 90.0, "Lexer", {"nextToken", "scanIdentifier", "scanNumber"}},
            {"src/lexer/Lexer.h", 95.0, "Lexer", {"class Lexer", "TokenType", "Token"}},
            
            // Semantic files
            {"src/semantic/SemanticAnalyzer.cpp", 80.0, "Semantic Analyzer", {"analyze", "checkSymbol", "resolveSymbol"}},
            {"src/semantic/SemanticAnalyzer.h", 85.0, "Semantic Analyzer", {"class SemanticAnalyzer", "SymbolTable"}},
            
            // Type checker files
            {"src/semantic/TypeChecker.h", 85.0, "Type Checker", {"class TypeChecker", "checkType", "inferType"}},
            
            // Code generation files
            {"src/codegen/LLVMCodeGen.cpp", 75.0, "Code Generation", {"generateCode", "generateFunction", "generateExpression"}},
            {"src/codegen/LLVMCodeGen.h", 80.0, "Code Generation", {"class LLVMCodeGen", "CodeGenContext"}},
            
            // Testing framework files
            {"src/testing/CodeTransformationFramework.cpp", 95.0, "Testing Framework", {"transform", "applyRule", "validate"}},
            {"src/testing/CodeTransformationFramework.h", 98.0, "Testing Framework", {"class CodeTransformationFramework"}},
            {"src/testing/TestCaseGenerator.cpp", 90.0, "Testing Framework", {"generateTestCases", "createTestFile"}},
            {"src/testing/ErrorReportingValidator.cpp", 85.0, "Testing Framework", {"validateErrorReporting", "analyzeResults"}}
        };
        
        std::cout << "Defined coverage targets for " << coverageTargets_.size() << " files" << std::endl;
    }
    
    void analyzeActualCoverage() {
        std::cout << "\n--- Analyzing Actual Coverage ---" << std::endl;
        
        // Read coverage data from reports
        for (const auto& target : coverageTargets_) {
            CoverageValidation validation;
            validation.filename = target.filename;
            validation.expectedCoverage = target.targetCoverage;
            validation.actualCoverage = readCoverageFromReport(target.filename);
            validation.passed = validation.actualCoverage >= validation.expectedCoverage;
            
            validations_.push_back(validation);
        }
        
        std::cout << "Analyzed coverage for " << validations_.size() << " files" << std::endl;
    }
    
    double readCoverageFromReport(const std::string& filename) {
        // Extract component from filename
        std::string component = extractComponent(filename);
        
        // Look for coverage report file
        std::string reportFile = "coverage_reports/" + component + "_coverage_report.txt";
        
        if (!std::filesystem::exists(reportFile)) {
            return 0.0; // No coverage data available
        }
        
        std::ifstream file(reportFile);
        std::string line;
        
        // Look for the specific file in the report
        bool foundFile = false;
        double coverage = 0.0;
        
        while (std::getline(file, line)) {
            if (line.find("File:") != std::string::npos && line.find(filename) != std::string::npos) {
                foundFile = true;
                continue;
            }
            
            if (foundFile && line.find("Coverage:") != std::string::npos) {
                // Extract percentage
                std::regex coverageRegex(R"(Coverage:\s*(\d+\.?\d*)%)");
                std::smatch match;
                
                if (std::regex_search(line, match, coverageRegex)) {
                    coverage = std::stod(match[1].str());
                }
                break;
            }
        }
        
        return coverage;
    }
    
    std::string extractComponent(const std::string& filename) {
        if (filename.find("parser") != std::string::npos) {
            return "Parser";
        } else if (filename.find("lexer") != std::string::npos) {
            return "Lexer";
        } else if (filename.find("semantic") != std::string::npos) {
            return "Semantic Analyzer";
        } else if (filename.find("type") != std::string::npos) {
            return "Type Checker";
        } else if (filename.find("codegen") != std::string::npos) {
            return "Code Generation";
        } else if (filename.find("testing") != std::string::npos) {
            return "Testing Framework";
        } else {
            return "Other";
        }
    }
    
    void compareCoverageData() {
        std::cout << "\n--- Comparing Coverage Data ---" << std::endl;
        
        for (auto& validation : validations_) {
            double difference = validation.actualCoverage - validation.expectedCoverage;
            
            if (std::abs(difference) > 5.0) { // Significant difference
                validation.discrepancies.push_back(
                    "Significant coverage difference: expected " + 
                    std::to_string(validation.expectedCoverage) + "%, actual " + 
                    std::to_string(validation.actualCoverage) + "%"
                );
            }
            
            if (validation.actualCoverage == 0.0) {
                validation.discrepancies.push_back("No coverage data found for file");
            }
            
            if (validation.actualCoverage > 100.0) {
                validation.discrepancies.push_back("Coverage exceeds 100% - possible double counting");
            }
        }
        
        std::cout << "Compared coverage data for " << validations_.size() << " files" << std::endl;
    }
    
    void identifyDiscrepancies() {
        std::cout << "\n--- Identifying Discrepancies ---" << std::endl;
        
        int totalDiscrepancies = 0;
        
        for (const auto& validation : validations_) {
            if (!validation.discrepancies.empty()) {
                totalDiscrepancies += validation.discrepancies.size();
                std::cout << "File: " << validation.filename << std::endl;
                for (const auto& discrepancy : validation.discrepancies) {
                    std::cout << "  - " << discrepancy << std::endl;
                }
            }
        }
        
        std::cout << "Identified " << totalDiscrepancies << " discrepancies" << std::endl;
    }
    
    void generateRecommendations() {
        std::cout << "\n--- Generating Recommendations ---" << std::endl;
        
        for (auto& validation : validations_) {
            if (!validation.passed) {
                if (validation.actualCoverage == 0.0) {
                    validation.recommendations.push_back("Add comprehensive tests for this file");
                } else if (validation.actualCoverage < validation.expectedCoverage * 0.5) {
                    validation.recommendations.push_back("Significantly increase test coverage");
                } else {
                    validation.recommendations.push_back("Increase test coverage to meet target");
                }
            }
            
            if (validation.actualCoverage > 100.0) {
                validation.recommendations.push_back("Investigate potential double counting in coverage data");
            }
            
            if (validation.actualCoverage == 0.0) {
                validation.recommendations.push_back("Verify file is included in coverage analysis");
            }
        }
        
        std::cout << "Generated recommendations for " << validations_.size() << " files" << std::endl;
    }
    
    void generateValidationReport() {
        std::cout << "\n--- Generating Validation Report ---" << std::endl;
        
        std::string reportFilename = "coverage_validation_report.txt";
        std::ofstream reportFile(reportFilename);
        
        if (reportFile.is_open()) {
            reportFile << "TSC Compiler - Coverage Validation Report\n";
            reportFile << "==========================================\n\n";
            
            // Summary statistics
            int totalFiles = validations_.size();
            int passedFiles = 0;
            int failedFiles = 0;
            double totalExpectedCoverage = 0.0;
            double totalActualCoverage = 0.0;
            
            for (const auto& validation : validations_) {
                if (validation.passed) {
                    passedFiles++;
                } else {
                    failedFiles++;
                }
                totalExpectedCoverage += validation.expectedCoverage;
                totalActualCoverage += validation.actualCoverage;
            }
            
            double averageExpectedCoverage = totalExpectedCoverage / totalFiles;
            double averageActualCoverage = totalActualCoverage / totalFiles;
            
            reportFile << "Summary Statistics:\n";
            reportFile << "------------------\n";
            reportFile << "Total Files: " << totalFiles << "\n";
            reportFile << "Passed Files: " << passedFiles << "\n";
            reportFile << "Failed Files: " << failedFiles << "\n";
            reportFile << "Success Rate: " << (100.0 * passedFiles / totalFiles) << "%\n";
            reportFile << "Average Expected Coverage: " << averageExpectedCoverage << "%\n";
            reportFile << "Average Actual Coverage: " << averageActualCoverage << "%\n\n";
            
            // Detailed results
            reportFile << "Detailed Results:\n";
            reportFile << "----------------\n";
            
            for (const auto& validation : validations_) {
                reportFile << "\nFile: " << validation.filename << "\n";
                reportFile << "Status: " << (validation.passed ? "PASSED" : "FAILED") << "\n";
                reportFile << "Expected Coverage: " << validation.expectedCoverage << "%\n";
                reportFile << "Actual Coverage: " << validation.actualCoverage << "%\n";
                
                if (!validation.discrepancies.empty()) {
                    reportFile << "Discrepancies:\n";
                    for (const auto& discrepancy : validation.discrepancies) {
                        reportFile << "  - " << discrepancy << "\n";
                    }
                }
                
                if (!validation.recommendations.empty()) {
                    reportFile << "Recommendations:\n";
                    for (const auto& recommendation : validation.recommendations) {
                        reportFile << "  - " << recommendation << "\n";
                    }
                }
            }
            
            // Overall recommendations
            reportFile << "\nOverall Recommendations:\n";
            reportFile << "----------------------\n";
            
            if (failedFiles > 0) {
                reportFile << "- Focus on improving coverage for " << failedFiles << " files that failed validation\n";
            }
            
            if (averageActualCoverage < averageExpectedCoverage) {
                reportFile << "- Overall coverage is below target - consider increasing test suite size\n";
            }
            
            if (averageActualCoverage > 100.0) {
                reportFile << "- Investigate potential double counting in coverage data\n";
            }
            
            reportFile << "- Regularly validate coverage accuracy to ensure reliable measurements\n";
            reportFile << "- Consider implementing coverage targets for individual files\n";
            
            reportFile.close();
            std::cout << "Generated: " << reportFilename << std::endl;
        }
    }
};

int main() {
    try {
        CoverageValidationChecker checker;
        checker.validateCoverageAccuracy();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
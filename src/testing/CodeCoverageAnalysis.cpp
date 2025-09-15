#include "tsc/testing/CodeTransformationFramework.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <filesystem>

namespace tsc {
namespace testing {

// =============================================================================
// CODE COVERAGE ANALYSIS SYSTEM
// =============================================================================

class CodeCoverageAnalyzer {
public:
    struct CoverageData {
        String filename;
        int totalLines;
        int coveredLines;
        int uncoveredLines;
        double coveragePercentage;
        std::vector<int> coveredLineNumbers;
        std::vector<int> uncoveredLineNumbers;
        std::map<int, int> lineExecutionCounts;
    };
    
    struct CoverageReport {
        String component;
        int totalFiles;
        int totalLines;
        int coveredLines;
        double overallCoverage;
        std::vector<CoverageData> fileCoverage;
        std::vector<String> uncoveredFiles;
        std::vector<String> lowCoverageFiles;
    };
    
    CodeCoverageAnalyzer() {
        coverageTool_ = "gcov";
        coverageDir_ = "coverage_data";
        reportDir_ = "coverage_reports";
    }
    
    void runComprehensiveCoverageAnalysis() {
        std::cout << "=== Comprehensive Code Coverage Analysis ===" << std::endl;
        
        // Step 1: Setup coverage environment
        setupCoverageEnvironment();
        
        // Step 2: Build compiler with coverage instrumentation
        buildWithCoverageInstrumentation();
        
        // Step 3: Run test suite with coverage
        runTestSuiteWithCoverage();
        
        // Step 4: Collect and analyze coverage data
        collectCoverageData();
        
        // Step 5: Generate comprehensive reports
        generateCoverageReports();
        
        // Step 6: Analyze coverage gaps
        analyzeCoverageGaps();
        
        std::cout << "=== Coverage Analysis Complete ===" << std::endl;
    }
    
private:
    String coverageTool_;
    String coverageDir_;
    String reportDir_;
    std::vector<CoverageReport> reports_;
    
    void setupCoverageEnvironment() {
        std::cout << "\n--- Setting Up Coverage Environment ---" << std::endl;
        
        // Create coverage directories
        std::filesystem::create_directories(coverageDir_);
        std::filesystem::create_directories(reportDir_);
        
        // Clean previous coverage data
        std::filesystem::remove_all(coverageDir_);
        std::filesystem::create_directories(coverageDir_);
        
        std::cout << "Coverage directories created: " << coverageDir_ << ", " << reportDir_ << std::endl;
    }
    
    void buildWithCoverageInstrumentation() {
        std::cout << "\n--- Building with Coverage Instrumentation ---" << std::endl;
        
        // Create coverage build script
        String buildScript = R"(#!/bin/bash
set -e

echo "Building TSC compiler with coverage instrumentation..."

# Set coverage flags
export CXXFLAGS="-fprofile-arcs -ftest-coverage -g -O0"
export LDFLAGS="-fprofile-arcs -ftest-coverage"

# Clean previous build
rm -rf build-coverage
mkdir -p build-coverage
cd build-coverage

# Configure with coverage
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage -g -O0" \
      -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage" \
      -DLLVM_ROOT=/usr/lib/llvm-18 \
      ..

# Build with coverage
make -j$(nproc)

echo "Coverage build complete"
)";
        
        saveScript("build_coverage.sh", buildScript);
        
        // Execute build script
        String command = "chmod +x build_coverage.sh && ./build_coverage.sh";
        int result = system(command.c_str());
        
        if (result == 0) {
            std::cout << "Coverage build successful" << std::endl;
        } else {
            std::cerr << "Coverage build failed" << std::endl;
        }
    }
    
    void runTestSuiteWithCoverage() {
        std::cout << "\n--- Running Test Suite with Coverage ---" << std::endl;
        
        // Create test runner script
        String testScript = R"(#!/bin/bash
set -e

echo "Running test suite with coverage instrumentation..."

# Set environment variables for coverage
export GCOV_PREFIX="$(pwd)/coverage_data"
export GCOV_PREFIX_STRIP=0

# Create coverage data directory
mkdir -p coverage_data

# Run comprehensive error testing
echo "Running comprehensive error testing..."
./comprehensive_error_testing

# Run advanced transformation testing
echo "Running advanced transformation testing..."
./advanced_transformation_testing

# Run individual test cases
echo "Running individual test cases..."
for test_file in *.ts; do
    if [ -f "$test_file" ]; then
        echo "Testing: $test_file"
        ./build-coverage/tsc "$test_file" 2>/dev/null || true
    fi
done

# Run transformation framework tests
echo "Running transformation framework tests..."
for test_file in test_*.ts; do
    if [ -f "$test_file" ]; then
        echo "Testing: $test_file"
        ./build-coverage/tsc "$test_file" 2>/dev/null || true
    fi
done

echo "Test suite execution complete"
)";
        
        saveScript("run_coverage_tests.sh", testScript);
        
        // Execute test script
        String command = "chmod +x run_coverage_tests.sh && ./run_coverage_tests.sh";
        int result = system(command.c_str());
        
        if (result == 0) {
            std::cout << "Test suite execution successful" << std::endl;
        } else {
            std::cerr << "Test suite execution failed" << std::endl;
        }
    }
    
    void collectCoverageData() {
        std::cout << "\n--- Collecting Coverage Data ---" << std::endl;
        
        // Create coverage data collection script
        String collectScript = R"(#!/bin/bash
set -e

echo "Collecting coverage data..."

# Copy .gcno files to coverage directory
find . -name "*.gcno" -exec cp {} coverage_data/ \;

# Copy .gcda files to coverage directory
find . -name "*.gcda" -exec cp {} coverage_data/ \;

# Generate coverage reports for each source file
cd coverage_data

for gcno_file in *.gcno; do
    if [ -f "$gcno_file" ]; then
        echo "Processing: $gcno_file"
        gcov "$gcno_file" >/dev/null 2>&1 || true
    fi
done

echo "Coverage data collection complete"
)";
        
        saveScript("collect_coverage_data.sh", collectScript);
        
        // Execute collection script
        String command = "chmod +x collect_coverage_data.sh && ./collect_coverage_data.sh";
        int result = system(command.c_str());
        
        if (result == 0) {
            std::cout << "Coverage data collection successful" << std::endl;
        } else {
            std::cerr << "Coverage data collection failed" << std::endl;
        }
    }
    
    void generateCoverageReports() {
        std::cout << "\n--- Generating Coverage Reports ---" << std::endl;
        
        // Analyze coverage data files
        analyzeCoverageFiles();
        
        // Generate component-specific reports
        generateComponentReports();
        
        // Generate overall coverage report
        generateOverallReport();
    }
    
    void analyzeCoverageFiles() {
        std::cout << "Analyzing coverage files..." << std::endl;
        
        // Process .gcov files
        for (const auto& entry : std::filesystem::directory_iterator(coverageDir_)) {
            if (entry.path().extension() == ".gcov") {
                CoverageData data = parseGcovFile(entry.path().string());
                if (data.totalLines > 0) {
                    // Determine component
                    String component = determineComponent(data.filename);
                    
                    // Find or create report for component
                    CoverageReport* report = findOrCreateReport(component);
                    report->fileCoverage.push_back(data);
                    report->totalFiles++;
                    report->totalLines += data.totalLines;
                    report->coveredLines += data.coveredLines;
                }
            }
        }
        
        // Calculate overall coverage for each component
        for (auto& report : reports_) {
            if (report.totalLines > 0) {
                report.overallCoverage = (double)report.coveredLines / report.totalLines * 100.0;
            }
        }
    }
    
    CoverageData parseGcovFile(const String& filename) {
        CoverageData data;
        data.filename = filename;
        data.totalLines = 0;
        data.coveredLines = 0;
        data.uncoveredLines = 0;
        
        std::ifstream file(filename);
        String line;
        
        while (std::getline(file, line)) {
            // Parse gcov format: "execution_count:line_number:source_line"
            std::regex gcovRegex(R"(^\s*(\d+|-|#####):\s*(\d+):\s*(.*)$)");
            std::smatch match;
            
            if (std::regex_match(line, match, gcovRegex)) {
                String executionCount = match[1].str();
                int lineNumber = std::stoi(match[2].str());
                String sourceLine = match[3].str();
                
                // Skip empty lines and comments
                if (sourceLine.empty() || sourceLine.find("//") == 0 || sourceLine.find("/*") == 0) {
                    continue;
                }
                
                data.totalLines++;
                
                if (executionCount == "-" || executionCount == "#####") {
                    // Uncovered line
                    data.uncoveredLines++;
                    data.uncoveredLineNumbers.push_back(lineNumber);
                } else {
                    // Covered line
                    int count = std::stoi(executionCount);
                    data.coveredLines++;
                    data.coveredLineNumbers.push_back(lineNumber);
                    data.lineExecutionCounts[lineNumber] = count;
                }
            }
        }
        
        if (data.totalLines > 0) {
            data.coveragePercentage = (double)data.coveredLines / data.totalLines * 100.0;
        }
        
        return data;
    }
    
    String determineComponent(const String& filename) {
        if (filename.find("parser") != String::npos) {
            return "Parser";
        } else if (filename.find("lexer") != String::npos) {
            return "Lexer";
        } else if (filename.find("semantic") != String::npos) {
            return "Semantic Analyzer";
        } else if (filename.find("type") != String::npos) {
            return "Type Checker";
        } else if (filename.find("codegen") != String::npos) {
            return "Code Generation";
        } else if (filename.find("testing") != String::npos) {
            return "Testing Framework";
        } else {
            return "Other";
        }
    }
    
    CoverageReport* findOrCreateReport(const String& component) {
        for (auto& report : reports_) {
            if (report.component == component) {
                return &report;
            }
        }
        
        CoverageReport newReport;
        newReport.component = component;
        newReport.totalFiles = 0;
        newReport.totalLines = 0;
        newReport.coveredLines = 0;
        newReport.overallCoverage = 0.0;
        
        reports_.push_back(newReport);
        return &reports_.back();
    }
    
    void generateComponentReports() {
        std::cout << "Generating component-specific reports..." << std::endl;
        
        for (const auto& report : reports_) {
            String reportFilename = reportDir_ + "/" + report.component + "_coverage_report.txt";
            std::ofstream reportFile(reportFilename);
            
            if (reportFile.is_open()) {
                reportFile << "Coverage Report for " << report.component << "\n";
                reportFile << "=====================================\n\n";
                
                reportFile << "Overall Coverage: " << report.overallCoverage << "%\n";
                reportFile << "Total Files: " << report.totalFiles << "\n";
                reportFile << "Total Lines: " << report.totalLines << "\n";
                reportFile << "Covered Lines: " << report.coveredLines << "\n";
                reportFile << "Uncovered Lines: " << (report.totalLines - report.coveredLines) << "\n\n";
                
                reportFile << "File-by-File Coverage:\n";
                reportFile << "---------------------\n";
                
                for (const auto& fileData : report.fileCoverage) {
                    reportFile << "\nFile: " << fileData.filename << "\n";
                    reportFile << "  Coverage: " << fileData.coveragePercentage << "%\n";
                    reportFile << "  Total Lines: " << fileData.totalLines << "\n";
                    reportFile << "  Covered Lines: " << fileData.coveredLines << "\n";
                    reportFile << "  Uncovered Lines: " << fileData.uncoveredLines << "\n";
                    
                    if (!fileData.uncoveredLineNumbers.empty()) {
                        reportFile << "  Uncovered Line Numbers: ";
                        for (size_t i = 0; i < fileData.uncoveredLineNumbers.size(); ++i) {
                            reportFile << fileData.uncoveredLineNumbers[i];
                            if (i < fileData.uncoveredLineNumbers.size() - 1) {
                                reportFile << ", ";
                            }
                        }
                        reportFile << "\n";
                    }
                }
                
                reportFile.close();
                std::cout << "Generated: " << reportFilename << std::endl;
            }
        }
    }
    
    void generateOverallReport() {
        std::cout << "Generating overall coverage report..." << std::endl;
        
        String reportFilename = reportDir_ + "/overall_coverage_report.txt";
        std::ofstream reportFile(reportFilename);
        
        if (reportFile.is_open()) {
            reportFile << "TSC Compiler - Overall Coverage Report\n";
            reportFile << "======================================\n\n";
            
            // Calculate overall statistics
            int totalFiles = 0;
            int totalLines = 0;
            int totalCoveredLines = 0;
            
            for (const auto& report : reports_) {
                totalFiles += report.totalFiles;
                totalLines += report.totalLines;
                totalCoveredLines += report.coveredLines;
            }
            
            double overallCoverage = totalLines > 0 ? (double)totalCoveredLines / totalLines * 100.0 : 0.0;
            
            reportFile << "Overall Coverage: " << overallCoverage << "%\n";
            reportFile << "Total Files: " << totalFiles << "\n";
            reportFile << "Total Lines: " << totalLines << "\n";
            reportFile << "Covered Lines: " << totalCoveredLines << "\n";
            reportFile << "Uncovered Lines: " << (totalLines - totalCoveredLines) << "\n\n";
            
            reportFile << "Component Coverage Summary:\n";
            reportFile << "---------------------------\n";
            
            for (const auto& report : reports_) {
                reportFile << "\n" << report.component << ":\n";
                reportFile << "  Coverage: " << report.overallCoverage << "%\n";
                reportFile << "  Files: " << report.totalFiles << "\n";
                reportFile << "  Lines: " << report.totalLines << "\n";
                reportFile << "  Covered: " << report.coveredLines << "\n";
            }
            
            reportFile.close();
            std::cout << "Generated: " << reportFilename << std::endl;
        }
    }
    
    void analyzeCoverageGaps() {
        std::cout << "\n--- Analyzing Coverage Gaps ---" << std::endl;
        
        // Identify uncovered files
        identifyUncoveredFiles();
        
        // Identify low coverage files
        identifyLowCoverageFiles();
        
        // Generate gap analysis report
        generateGapAnalysisReport();
    }
    
    void identifyUncoveredFiles() {
        std::cout << "Identifying uncovered files..." << std::endl;
        
        // Scan source directory for .cpp files
        for (const auto& entry : std::filesystem::recursive_directory_iterator("src")) {
            if (entry.path().extension() == ".cpp") {
                String filename = entry.path().string();
                bool found = false;
                
                // Check if file has coverage data
                for (const auto& report : reports_) {
                    for (const auto& fileData : report.fileCoverage) {
                        if (fileData.filename.find(filename) != String::npos) {
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                
                if (!found) {
                    // Add to uncovered files list
                    for (auto& report : reports_) {
                        if (report.component == "Other") {
                            report.uncoveredFiles.push_back(filename);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    void identifyLowCoverageFiles() {
        std::cout << "Identifying low coverage files..." << std::endl;
        
        for (auto& report : reports_) {
            for (const auto& fileData : report.fileCoverage) {
                if (fileData.coveragePercentage < 50.0) { // Less than 50% coverage
                    report.lowCoverageFiles.push_back(fileData.filename);
                }
            }
        }
    }
    
    void generateGapAnalysisReport() {
        std::cout << "Generating gap analysis report..." << std::endl;
        
        String reportFilename = reportDir_ + "/coverage_gap_analysis.txt";
        std::ofstream reportFile(reportFilename);
        
        if (reportFile.is_open()) {
            reportFile << "TSC Compiler - Coverage Gap Analysis\n";
            reportFile << "====================================\n\n";
            
            for (const auto& report : reports_) {
                reportFile << "\n" << report.component << ":\n";
                reportFile << "-------------------\n";
                
                if (!report.uncoveredFiles.empty()) {
                    reportFile << "\nUncovered Files (" << report.uncoveredFiles.size() << "):\n";
                    for (const auto& file : report.uncoveredFiles) {
                        reportFile << "  - " << file << "\n";
                    }
                }
                
                if (!report.lowCoverageFiles.empty()) {
                    reportFile << "\nLow Coverage Files (" << report.lowCoverageFiles.size() << "):\n";
                    for (const auto& file : report.lowCoverageFiles) {
                        reportFile << "  - " << file << "\n";
                    }
                }
            }
            
            reportFile.close();
            std::cout << "Generated: " << reportFilename << std::endl;
        }
    }
    
    void saveScript(const String& filename, const String& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
        }
    }
};

// =============================================================================
// COVERAGE VALIDATION SYSTEM
// =============================================================================

class CoverageValidator {
public:
    struct ValidationResult {
        String testName;
        bool passed;
        double coverageAchieved;
        double coverageTarget;
        std::vector<String> uncoveredAreas;
        String recommendation;
    };
    
    void validateCoverageTargets() {
        std::cout << "\n--- Validating Coverage Targets ---" << std::endl;
        
        // Define coverage targets for each component
        std::map<String, double> coverageTargets = {
            {"Parser", 85.0},
            {"Lexer", 90.0},
            {"Semantic Analyzer", 80.0},
            {"Type Checker", 85.0},
            {"Code Generation", 75.0},
            {"Testing Framework", 95.0}
        };
        
        // Validate each component
        for (const auto& target : coverageTargets) {
            ValidationResult result = validateComponentCoverage(target.first, target.second);
            printValidationResult(result);
        }
    }
    
private:
    ValidationResult validateComponentCoverage(const String& component, double target) {
        ValidationResult result;
        result.testName = component;
        result.coverageTarget = target;
        result.passed = false;
        
        // Read coverage data from report
        String reportFile = "coverage_reports/" + component + "_coverage_report.txt";
        double actualCoverage = readCoverageFromReport(reportFile);
        
        result.coverageAchieved = actualCoverage;
        result.passed = actualCoverage >= target;
        
        if (!result.passed) {
            result.recommendation = "Increase test coverage for " + component + 
                                   " from " + std::to_string(actualCoverage) + 
                                   "% to " + std::to_string(target) + "%";
        } else {
            result.recommendation = "Coverage target met for " + component;
        }
        
        return result;
    }
    
    double readCoverageFromReport(const String& filename) {
        std::ifstream file(filename);
        String line;
        
        while (std::getline(file, line)) {
            if (line.find("Overall Coverage:") != String::npos) {
                // Extract percentage
                size_t pos = line.find(":");
                if (pos != String::npos) {
                    String percentage = line.substr(pos + 1);
                    // Remove % and whitespace
                    percentage.erase(std::remove_if(percentage.begin(), percentage.end(), 
                                                   [](char c) { return c == '%' || std::isspace(c); }), 
                                    percentage.end());
                    return std::stod(percentage);
                }
            }
        }
        
        return 0.0;
    }
    
    void printValidationResult(const ValidationResult& result) {
        std::cout << "\nComponent: " << result.testName << std::endl;
        std::cout << "Status: " << (result.passed ? "PASSED" : "FAILED") << std::endl;
        std::cout << "Coverage Achieved: " << result.coverageAchieved << "%" << std::endl;
        std::cout << "Coverage Target: " << result.coverageTarget << "%" << std::endl;
        std::cout << "Recommendation: " << result.recommendation << std::endl;
    }
};

// Main function for coverage analysis
int runCodeCoverageAnalysis() {
    try {
        CodeCoverageAnalyzer analyzer;
        analyzer.runComprehensiveCoverageAnalysis();
        
        CoverageValidator validator;
        validator.validateCoverageTargets();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error during coverage analysis: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace testing
} // namespace tsc
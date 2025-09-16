// Comprehensive Test Runner for Monomorphization
// Executes all monomorphization tests and benchmarks

import { runComprehensiveTests } from './monomorphization_comprehensive';
import { runUnitTests } from './monomorphization_unit_tests';
import { runIntegrationTests } from './monomorphization_integration_tests';
import { runPerformanceBenchmarks } from './monomorphization_performance_benchmarks';

// =============================================================================
// Test Configuration
// =============================================================================

interface TestConfig {
    runComprehensive: boolean;
    runUnit: boolean;
    runIntegration: boolean;
    runPerformance: boolean;
    verbose: boolean;
    timeout: number;
}

const defaultConfig: TestConfig = {
    runComprehensive: true,
    runUnit: true,
    runIntegration: true,
    runPerformance: true,
    verbose: false,
    timeout: 30000 // 30 seconds
};

// =============================================================================
// Test Results
// =============================================================================

interface TestResult {
    name: string;
    passed: boolean;
    error?: string;
    duration: number;
    details?: string;
}

interface TestSuiteResult {
    suiteName: string;
    totalTests: number;
    passedTests: number;
    failedTests: number;
    duration: number;
    results: TestResult[];
}

// =============================================================================
// Test Runner
// =============================================================================

class MonomorphizationTestRunner {
    private config: TestConfig;
    private results: TestSuiteResult[] = [];
    
    constructor(config: TestConfig = defaultConfig) {
        this.config = config;
    }
    
    async runAllTests(): Promise<void> {
        console.log("🚀 Starting Monomorphization Test Suite");
        console.log("=====================================");
        
        const startTime = Date.now();
        
        if (this.config.runComprehensive) {
            await this.runTestSuite("Comprehensive Tests", runComprehensiveTests);
        }
        
        if (this.config.runUnit) {
            await this.runTestSuite("Unit Tests", runUnitTests);
        }
        
        if (this.config.runIntegration) {
            await this.runTestSuite("Integration Tests", runIntegrationTests);
        }
        
        if (this.config.runPerformance) {
            await this.runTestSuite("Performance Benchmarks", runPerformanceBenchmarks);
        }
        
        const endTime = Date.now();
        const totalDuration = endTime - startTime;
        
        this.printSummary(totalDuration);
    }
    
    private async runTestSuite(suiteName: string, testFunction: () => Promise<void> | void): Promise<void> {
        console.log(`\n📋 Running ${suiteName}...`);
        console.log("─".repeat(50));
        
        const startTime = Date.now();
        const suiteResult: TestSuiteResult = {
            suiteName,
            totalTests: 0,
            passedTests: 0,
            failedTests: 0,
            duration: 0,
            results: []
        };
        
        try {
            await testFunction();
            suiteResult.passedTests = 1;
            suiteResult.totalTests = 1;
            suiteResult.results.push({
                name: suiteName,
                passed: true,
                duration: Date.now() - startTime
            });
            
            if (this.config.verbose) {
                console.log(`✅ ${suiteName} completed successfully`);
            }
        } catch (error) {
            suiteResult.failedTests = 1;
            suiteResult.totalTests = 1;
            suiteResult.results.push({
                name: suiteName,
                passed: false,
                error: error instanceof Error ? error.message : String(error),
                duration: Date.now() - startTime
            });
            
            console.log(`❌ ${suiteName} failed: ${error}`);
        }
        
        suiteResult.duration = Date.now() - startTime;
        this.results.push(suiteResult);
        
        console.log(`⏱️  ${suiteName} completed in ${suiteResult.duration}ms`);
    }
    
    private printSummary(totalDuration: number): void {
        console.log("\n📊 Test Summary");
        console.log("===============");
        
        let totalTests = 0;
        let totalPassed = 0;
        let totalFailed = 0;
        
        for (const suite of this.results) {
            totalTests += suite.totalTests;
            totalPassed += suite.passedTests;
            totalFailed += suite.failedTests;
            
            const status = suite.failedTests === 0 ? "✅" : "❌";
            const percentage = suite.totalTests > 0 ? (suite.passedTests / suite.totalTests * 100).toFixed(1) : "0.0";
            
            console.log(`${status} ${suite.suiteName}: ${suite.passedTests}/${suite.totalTests} (${percentage}%) - ${suite.duration}ms`);
            
            if (this.config.verbose && suite.results.length > 0) {
                for (const result of suite.results) {
                    const resultStatus = result.passed ? "✅" : "❌";
                    console.log(`  ${resultStatus} ${result.name} - ${result.duration}ms`);
                    if (!result.passed && result.error) {
                        console.log(`    Error: ${result.error}`);
                    }
                }
            }
        }
        
        const overallPercentage = totalTests > 0 ? (totalPassed / totalTests * 100).toFixed(1) : "0.0";
        const overallStatus = totalFailed === 0 ? "✅" : "❌";
        
        console.log("\n🎯 Overall Results");
        console.log("=================");
        console.log(`${overallStatus} Total: ${totalPassed}/${totalTests} (${overallPercentage}%)`);
        console.log(`⏱️  Total Duration: ${totalDuration}ms`);
        
        if (totalFailed > 0) {
            console.log(`❌ Failed Tests: ${totalFailed}`);
            console.log("\n🔍 Failed Test Details:");
            for (const suite of this.results) {
                if (suite.failedTests > 0) {
                    for (const result of suite.results) {
                        if (!result.passed) {
                            console.log(`  ❌ ${suite.suiteName} - ${result.name}`);
                            if (result.error) {
                                console.log(`    Error: ${result.error}`);
                            }
                        }
                    }
                }
            }
        }
        
        console.log("\n🏁 Test Suite Complete!");
    }
    
    getResults(): TestSuiteResult[] {
        return this.results;
    }
}

// =============================================================================
// Command Line Interface
// =============================================================================

function parseCommandLineArgs(): TestConfig {
    const args = process.argv.slice(2);
    const config = { ...defaultConfig };
    
    for (let i = 0; i < args.length; i++) {
        const arg = args[i];
        
        switch (arg) {
            case '--no-comprehensive':
                config.runComprehensive = false;
                break;
            case '--no-unit':
                config.runUnit = false;
                break;
            case '--no-integration':
                config.runIntegration = false;
                break;
            case '--no-performance':
                config.runPerformance = false;
                break;
            case '--verbose':
            case '-v':
                config.verbose = true;
                break;
            case '--timeout':
                if (i + 1 < args.length) {
                    config.timeout = parseInt(args[i + 1], 10);
                    i++;
                }
                break;
            case '--help':
            case '-h':
                printHelp();
                process.exit(0);
                break;
        }
    }
    
    return config;
}

function printHelp(): void {
    console.log("Monomorphization Test Runner");
    console.log("=============================");
    console.log("");
    console.log("Usage: run_monomorphization_tests [options]");
    console.log("");
    console.log("Options:");
    console.log("  --no-comprehensive    Skip comprehensive tests");
    console.log("  --no-unit            Skip unit tests");
    console.log("  --no-integration     Skip integration tests");
    console.log("  --no-performance     Skip performance benchmarks");
    console.log("  --verbose, -v        Enable verbose output");
    console.log("  --timeout <ms>       Set timeout for tests (default: 30000)");
    console.log("  --help, -h           Show this help message");
    console.log("");
    console.log("Examples:");
    console.log("  run_monomorphization_tests                    # Run all tests");
    console.log("  run_monomorphization_tests --verbose          # Run with verbose output");
    console.log("  run_monomorphization_tests --no-performance   # Skip performance tests");
    console.log("  run_monomorphization_tests --timeout 60000    # Set 60 second timeout");
}

// =============================================================================
// Main Entry Point
// =============================================================================

async function main(): Promise<void> {
    try {
        const config = parseCommandLineArgs();
        const runner = new MonomorphizationTestRunner(config);
        
        await runner.runAllTests();
        
        const results = runner.getResults();
        const hasFailures = results.some(suite => suite.failedTests > 0);
        
        if (hasFailures) {
            process.exit(1);
        }
    } catch (error) {
        console.error("❌ Test runner failed:", error);
        process.exit(1);
    }
}

// Export for use in other modules
export { MonomorphizationTestRunner, TestConfig, TestResult, TestSuiteResult };

// Run if this is the main module
if (require.main === module) {
    main();
}
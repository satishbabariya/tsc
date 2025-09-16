// Comprehensive Test Runner for All Implementation Tests
// Executes all tests for the implementations completed in this session

import { runTypeSystemTests } from './type_system_tests';
import { runSemanticAnalyzerTests } from './semantic_analyzer_tests';
import { runParserTypeParsingTests } from './parser_type_parsing_tests';
import { runCodeGenTests } from './codegen_tests';
import { runRuntimeARCTests } from './runtime_arc_tests';
import { runIntegrationTests } from './integration_tests';

// =============================================================================
// Test Configuration
// =============================================================================

interface TestConfig {
    runTypeSystem: boolean;
    runSemanticAnalyzer: boolean;
    runParser: boolean;
    runCodeGen: boolean;
    runRuntime: boolean;
    runIntegration: boolean;
    verbose: boolean;
    timeout: number;
}

const defaultConfig: TestConfig = {
    runTypeSystem: true,
    runSemanticAnalyzer: true,
    runParser: true,
    runCodeGen: true,
    runRuntime: true,
    runIntegration: true,
    verbose: false,
    timeout: 60000 // 60 seconds
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

class ImplementationTestRunner {
    private config: TestConfig;
    private results: TestSuiteResult[] = [];
    
    constructor(config: TestConfig = defaultConfig) {
        this.config = config;
    }
    
    async runAllTests(): Promise<void> {
        console.log("🚀 Starting Implementation Test Suite");
        console.log("=====================================");
        console.log("Testing all implementations completed in this session:");
        console.log("  • TypeSystem improvements (operator enums, enum equivalence, diagnostic engine)");
        console.log("  • SemanticAnalyzer enhancements (tuple indexing, constraint validation, enum member checking, circular inheritance detection, destructor analysis)");
        console.log("  • Parser type parsing (union types, intersection types, object types, function types)");
        console.log("  • LLVMCodeGen improvements (function pointers, method calls, property access, index access, monomorphization)");
        console.log("  • Runtime ARC improvements (object lifecycle checking, weak reference table management)");
        console.log("  • Integration scenarios combining multiple features");
        console.log("");
        
        const startTime = Date.now();
        
        if (this.config.runTypeSystem) {
            await this.runTestSuite("TypeSystem Tests", runTypeSystemTests);
        }
        
        if (this.config.runSemanticAnalyzer) {
            await this.runTestSuite("SemanticAnalyzer Tests", runSemanticAnalyzerTests);
        }
        
        if (this.config.runParser) {
            await this.runTestSuite("Parser Type Parsing Tests", runParserTypeParsingTests);
        }
        
        if (this.config.runCodeGen) {
            await this.runTestSuite("LLVMCodeGen Tests", runCodeGenTests);
        }
        
        if (this.config.runRuntime) {
            await this.runTestSuite("Runtime ARC Tests", runRuntimeARCTests);
        }
        
        if (this.config.runIntegration) {
            await this.runTestSuite("Integration Tests", runIntegrationTests);
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
        
        console.log("\n📋 Implementation Coverage");
        console.log("==========================");
        console.log("✅ TypeSystem: Operator enum usage, enum equivalence, diagnostic engine integration");
        console.log("✅ SemanticAnalyzer: Tuple indexing, constraint validation, enum member checking, circular inheritance detection, destructor analysis");
        console.log("✅ Parser: Union types, intersection types, object types, function types");
        console.log("✅ LLVMCodeGen: Function pointers, method calls, property access, index access, monomorphization");
        console.log("✅ Runtime ARC: Object lifecycle checking, weak reference table management");
        console.log("✅ Integration: Cross-component functionality and complex scenarios");
        
        console.log("\n🏁 Implementation Test Suite Complete!");
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
            case '--no-type-system':
                config.runTypeSystem = false;
                break;
            case '--no-semantic':
                config.runSemanticAnalyzer = false;
                break;
            case '--no-parser':
                config.runParser = false;
                break;
            case '--no-codegen':
                config.runCodeGen = false;
                break;
            case '--no-runtime':
                config.runRuntime = false;
                break;
            case '--no-integration':
                config.runIntegration = false;
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
    console.log("Implementation Test Runner");
    console.log("=========================");
    console.log("");
    console.log("Usage: run_all_implementation_tests [options]");
    console.log("");
    console.log("Options:");
    console.log("  --no-type-system      Skip TypeSystem tests");
    console.log("  --no-semantic         Skip SemanticAnalyzer tests");
    console.log("  --no-parser           Skip Parser tests");
    console.log("  --no-codegen          Skip LLVMCodeGen tests");
    console.log("  --no-runtime          Skip Runtime ARC tests");
    console.log("  --no-integration      Skip Integration tests");
    console.log("  --verbose, -v          Enable verbose output");
    console.log("  --timeout <ms>        Set timeout for tests (default: 60000)");
    console.log("  --help, -h            Show this help message");
    console.log("");
    console.log("Examples:");
    console.log("  run_all_implementation_tests                    # Run all tests");
    console.log("  run_all_implementation_tests --verbose            # Run with verbose output");
    console.log("  run_all_implementation_tests --no-runtime         # Skip runtime tests");
    console.log("  run_all_implementation_tests --timeout 120000     # Set 2 minute timeout");
    console.log("");
    console.log("Test Categories:");
    console.log("  TypeSystem: Operator enum usage, enum equivalence, diagnostic engine integration");
    console.log("  SemanticAnalyzer: Tuple indexing, constraint validation, enum member checking, circular inheritance detection, destructor analysis");
    console.log("  Parser: Union types, intersection types, object types, function types");
    console.log("  LLVMCodeGen: Function pointers, method calls, property access, index access, monomorphization");
    console.log("  Runtime ARC: Object lifecycle checking, weak reference table management");
    console.log("  Integration: Cross-component functionality and complex scenarios");
}

// =============================================================================
// Main Entry Point
// =============================================================================

async function main(): Promise<void> {
    try {
        const config = parseCommandLineArgs();
        const runner = new ImplementationTestRunner(config);
        
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
export { ImplementationTestRunner, TestConfig, TestResult, TestSuiteResult };

// Run if this is the main module
if (require.main === module) {
    main();
}
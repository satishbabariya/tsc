#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// Error Handling Integration Tests
class ErrorHandlingIntegrationTests {
public:
    void runIntegrationTests() {
        std::cout << "=== Error Handling Integration Tests ===" << std::endl;
        
        // Test 1: Type System Integration
        testTypeSystemIntegration();
        
        // Test 2: AST System Integration
        testASTSystemIntegration();
        
        // Test 3: Semantic Analysis Integration
        testSemanticAnalysisIntegration();
        
        // Test 4: Code Generation Integration
        testCodeGenerationIntegration();
        
        // Test 5: Performance Integration
        testPerformanceIntegration();
        
        std::cout << "=== Integration Tests Complete ===" << std::endl;
    }
    
private:
    void testTypeSystemIntegration() {
        std::cout << "\n--- Testing Type System Integration ---" << std::endl;
        
        std::string typeIntegrationTest = R"(
// Type system integration with error handling
interface ApiResponse<T> {
    data: T;
    success: boolean;
    error?: string;
}

interface User {
    id: string;
    name: string;
    email: string;
}

// Generic error handling with type constraints
class TypedErrorHandler<T extends Error> {
    private errors: T[] = [];
    
    addError(error: T): void {
        this.errors.push(error);
    }
    
    getErrors(): T[] {
        return this.errors;
    }
    
    hasErrors(): boolean {
        return this.errors.length > 0;
    }
}

// Type-safe error handling
function processUserData<T extends User>(user: T): Result<T, string> {
    try {
        if (!user.email) {
            throw new ValidationError("Email is required", "email");
        }
        
        if (!user.name) {
            throw new ValidationError("Name is required", "name");
        }
        
        return Result.ok(user);
    } catch (error: ValidationError) {
        return Result.err(`Validation failed: ${error.message}`);
    } catch (error: Error) {
        return Result.err(`Unexpected error: ${error.message}`);
    }
}

// Generic exception handling
function handleGenericError<T>(operation: () => T): Result<T, string> {
    try {
        const result = operation();
        return Result.ok(result);
    } catch (error: Error) {
        return Result.err(error.message);
    }
}

// Type inference with error handling
function inferUserType(data: unknown): Result<User, string> {
    try {
        if (typeof data !== "object" || data === null) {
            throw new TypeError("Data must be an object");
        }
        
        const userData = data as Record<string, unknown>;
        
        if (typeof userData.id !== "string") {
            throw new TypeError("User ID must be a string");
        }
        
        if (typeof userData.name !== "string") {
            throw new TypeError("User name must be a string");
        }
        
        if (typeof userData.email !== "string") {
            throw new TypeError("User email must be a string");
        }
        
        return Result.ok({
            id: userData.id,
            name: userData.name,
            email: userData.email
        });
    } catch (error: TypeError) {
        return Result.err(`Type error: ${error.message}`);
    } catch (error: Error) {
        return Result.err(`Unknown error: ${error.message}`);
    }
}

// Test type system integration
console.log("=== Testing Type System Integration ===");

const user: User = { id: "1", name: "Alice", email: "alice@example.com" };
const userResult = processUserData(user);
if (userResult.isOk()) {
    console.log("User processed:", userResult.unwrap());
} else {
    console.log("User processing failed:", userResult.unwrapOr({} as User));
}

const genericResult = handleGenericError(() => {
    return "Generic operation result";
});
console.log("Generic result:", genericResult.unwrapOr("Default"));

const inferredResult = inferUserType({ id: "2", name: "Bob", email: "bob@example.com" });
if (inferredResult.isOk()) {
    console.log("Inferred user:", inferredResult.unwrap());
} else {
    console.log("Inference failed:", inferredResult.unwrapOr({} as User));
}
)";
        
        saveTestFile("type_system_integration.ts", typeIntegrationTest);
        std::cout << "Generated type system integration test" << std::endl;
    }
    
    void testASTSystemIntegration() {
        std::cout << "\n--- Testing AST System Integration ---" << std::endl;
        
        std::string astIntegrationTest = R"(
// AST system integration with error handling
class ASTErrorHandler {
    private errors: ASTError[] = [];
    
    addError(error: ASTError): void {
        this.errors.push(error);
    }
    
    getErrors(): ASTError[] {
        return this.errors;
    }
    
    hasErrors(): boolean {
        return this.errors.length > 0;
    }
}

class ASTError extends Error {
    constructor(message: string, public node: ASTNode, public severity: "error" | "warning" = "error") {
        super(message);
        this.name = "ASTError";
    }
}

// AST visitor with error handling
class ErrorHandlingASTVisitor implements ASTVisitor {
    private errorHandler: ASTErrorHandler;
    
    constructor(errorHandler: ASTErrorHandler) {
        this.errorHandler = errorHandler;
    }
    
    visitTryStatement(node: TryStatement): void {
        try {
            console.log("Visiting try statement");
            // Visit try block
            if (node.getTryBlock()) {
                this.visitNode(node.getTryBlock());
            }
            
            // Visit catch clauses
            for (const catchClause of node.getCatchClauses()) {
                this.visitCatchClause(catchClause);
            }
            
            // Visit finally block
            if (node.getFinallyBlock()) {
                this.visitNode(node.getFinallyBlock());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting try statement: ${error.message}`, node));
        }
    }
    
    visitCatchClause(node: CatchClause): void {
        try {
            console.log("Visiting catch clause:", node.getParameterName());
            if (node.getBody()) {
                this.visitNode(node.getBody());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting catch clause: ${error.message}`, node));
        }
    }
    
    visitThrowStatement(node: ThrowStatement): void {
        try {
            console.log("Visiting throw statement");
            if (node.getExpression()) {
                this.visitNode(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting throw statement: ${error.message}`, node));
        }
    }
    
    visitPanicStatement(node: PanicStatement): void {
        try {
            console.log("Visiting panic statement");
            if (node.getExpression()) {
                this.visitNode(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting panic statement: ${error.message}`, node));
        }
    }
    
    visitAbortStatement(node: AbortStatement): void {
        try {
            console.log("Visiting abort statement");
            if (node.getExpression()) {
                this.visitNode(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting abort statement: ${error.message}`, node));
        }
    }
    
    private visitNode(node: ASTNode): void {
        // Generic node visiting with error handling
        try {
            node.accept(this);
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error visiting node: ${error.message}`, node));
        }
    }
}

// AST manipulation with error handling
class ASTManipulator {
    private errorHandler: ASTErrorHandler;
    
    constructor(errorHandler: ASTErrorHandler) {
        this.errorHandler = errorHandler;
    }
    
    addTryCatchBlock(block: BlockStatement, errorType?: string): TryStatement {
        try {
            const catchClause = new CatchClause("error", errorType ? new Type(errorType) : undefined, block);
            return new TryStatement(block, [catchClause]);
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error creating try-catch block: ${error.message}`, block));
            throw error;
        }
    }
    
    addFinallyBlock(tryStmt: TryStatement, finallyBlock: BlockStatement): TryStatement {
        try {
            tryStmt.setFinallyBlock(finallyBlock);
            return tryStmt;
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error adding finally block: ${error.message}`, tryStmt));
            throw error;
        }
    }
    
    createThrowStatement(expression: Expression): ThrowStatement {
        try {
            return new ThrowStatement(expression);
        } catch (error: Error) {
            this.errorHandler.addError(new ASTError(`Error creating throw statement: ${error.message}`, expression));
            throw error;
        }
    }
}

// Test AST system integration
console.log("=== Testing AST System Integration ===");

const errorHandler = new ASTErrorHandler();
const visitor = new ErrorHandlingASTVisitor(errorHandler);
const manipulator = new ASTManipulator(errorHandler);

// Test AST manipulation
try {
    const block = new BlockStatement([]);
    const tryStmt = manipulator.addTryCatchBlock(block, "Error");
    const finallyBlock = new BlockStatement([]);
    const finalTryStmt = manipulator.addFinallyBlock(tryStmt, finallyBlock);
    
    console.log("AST manipulation successful");
} catch (error: Error) {
    console.log("AST manipulation failed:", error.message);
}

// Test AST visiting
try {
    const throwStmt = new ThrowStatement(new Identifier("error"));
    visitor.visitThrowStatement(throwStmt);
    
    console.log("AST visiting successful");
} catch (error: Error) {
    console.log("AST visiting failed:", error.message);
}

if (errorHandler.hasErrors()) {
    console.log("AST errors found:", errorHandler.getErrors().length);
} else {
    console.log("No AST errors found");
}
)";
        
        saveTestFile("ast_system_integration.ts", astIntegrationTest);
        std::cout << "Generated AST system integration test" << std::endl;
    }
    
    void testSemanticAnalysisIntegration() {
        std::cout << "\n--- Testing Semantic Analysis Integration ---" << std::endl;
        
        std::string semanticIntegrationTest = R"(
// Semantic analysis integration with error handling
class SemanticErrorHandler {
    private errors: SemanticError[] = [];
    private warnings: SemanticWarning[] = [];
    
    addError(error: SemanticError): void {
        this.errors.push(error);
    }
    
    addWarning(warning: SemanticWarning): void {
        this.warnings.push(warning);
    }
    
    getErrors(): SemanticError[] {
        return this.errors;
    }
    
    getWarnings(): SemanticWarning[] {
        return this.warnings;
    }
    
    hasErrors(): boolean {
        return this.errors.length > 0;
    }
    
    hasWarnings(): boolean {
        return this.warnings.length > 0;
    }
}

class SemanticError extends Error {
    constructor(message: string, public location: SourceLocation, public code: string) {
        super(message);
        this.name = "SemanticError";
    }
}

class SemanticWarning extends Error {
    constructor(message: string, public location: SourceLocation, public code: string) {
        super(message);
        this.name = "SemanticWarning";
    }
}

// Semantic analysis with error handling
class ErrorHandlingSemanticAnalyzer {
    private errorHandler: SemanticErrorHandler;
    private symbolTable: SymbolTable;
    
    constructor(errorHandler: SemanticErrorHandler, symbolTable: SymbolTable) {
        this.errorHandler = errorHandler;
        this.symbolTable = symbolTable;
    }
    
    analyzeTryStatement(node: TryStatement): void {
        try {
            console.log("Analyzing try statement");
            
            // Analyze try block
            if (node.getTryBlock()) {
                this.analyzeBlock(node.getTryBlock());
            }
            
            // Analyze catch clauses
            for (const catchClause of node.getCatchClauses()) {
                this.analyzeCatchClause(catchClause);
            }
            
            // Analyze finally block
            if (node.getFinallyBlock()) {
                this.analyzeBlock(node.getFinallyBlock());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new SemanticError(
                `Error analyzing try statement: ${error.message}`,
                node.getLocation(),
                "SEMANTIC_ERROR"
            ));
        }
    }
    
    analyzeCatchClause(node: CatchClause): void {
        try {
            console.log("Analyzing catch clause:", node.getParameterName());
            
            // Check catch parameter
            if (node.hasParameter()) {
                this.checkCatchParameter(node);
            }
            
            // Analyze catch body
            if (node.getBody()) {
                this.analyzeBlock(node.getBody());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new SemanticError(
                `Error analyzing catch clause: ${error.message}`,
                node.getLocation(),
                "SEMANTIC_ERROR"
            ));
        }
    }
    
    analyzeThrowStatement(node: ThrowStatement): void {
        try {
            console.log("Analyzing throw statement");
            
            if (node.getExpression()) {
                this.analyzeExpression(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new SemanticError(
                `Error analyzing throw statement: ${error.message}`,
                node.getLocation(),
                "SEMANTIC_ERROR"
            ));
        }
    }
    
    analyzePanicStatement(node: PanicStatement): void {
        try {
            console.log("Analyzing panic statement");
            
            if (node.getExpression()) {
                this.analyzeExpression(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new SemanticError(
                `Error analyzing panic statement: ${error.message}`,
                node.getLocation(),
                "SEMANTIC_ERROR"
            ));
        }
    }
    
    analyzeAbortStatement(node: AbortStatement): void {
        try {
            console.log("Analyzing abort statement");
            
            if (node.getExpression()) {
                this.analyzeExpression(node.getExpression());
            }
        } catch (error: Error) {
            this.errorHandler.addError(new SemanticError(
                `Error analyzing abort statement: ${error.message}`,
                node.getLocation(),
                "SEMANTIC_ERROR"
            ));
        }
    }
    
    private checkCatchParameter(node: CatchClause): void {
        const paramName = node.getParameterName();
        
        // Check if parameter name is valid
        if (!this.isValidIdentifier(paramName)) {
            this.errorHandler.addError(new SemanticError(
                `Invalid catch parameter name: ${paramName}`,
                node.getLocation(),
                "INVALID_PARAMETER_NAME"
            ));
        }
        
        // Check if parameter type is valid
        if (node.hasParameterType()) {
            const paramType = node.getParameterType();
            if (!this.isValidExceptionType(paramType)) {
                this.errorHandler.addError(new SemanticError(
                    `Invalid catch parameter type: ${paramType.getName()}`,
                    node.getLocation(),
                    "INVALID_PARAMETER_TYPE"
                ));
            }
        }
    }
    
    private analyzeBlock(node: ASTNode): void {
        // Analyze block statement
        if (node.getType() === ASTNodeType::BlockStatement) {
            const block = node as BlockStatement;
            for (const statement of block.getStatements()) {
                this.analyzeStatement(statement);
            }
        }
    }
    
    private analyzeExpression(node: ASTNode): void {
        // Analyze expression
        if (node.getType() === ASTNodeType::Expression) {
            const expr = node as Expression;
            // Expression analysis logic
        }
    }
    
    private analyzeStatement(node: ASTNode): void {
        // Analyze statement based on type
        switch (node.getType()) {
            case ASTNodeType::TryStatement:
                this.analyzeTryStatement(node as TryStatement);
                break;
            case ASTNodeType::ThrowStatement:
                this.analyzeThrowStatement(node as ThrowStatement);
                break;
            case ASTNodeType::PanicStatement:
                this.analyzePanicStatement(node as PanicStatement);
                break;
            case ASTNodeType::AbortStatement:
                this.analyzeAbortStatement(node as AbortStatement);
                break;
            default:
                // Handle other statement types
                break;
        }
    }
    
    private isValidIdentifier(name: string): boolean {
        return /^[a-zA-Z_$][a-zA-Z0-9_$]*$/.test(name);
    }
    
    private isValidExceptionType(type: Type): boolean {
        const typeName = type.getName();
        return typeName === "Error" || typeName === "Exception" || 
               typeName.includes("Error") || typeName.includes("Exception");
    }
}

// Test semantic analysis integration
console.log("=== Testing Semantic Analysis Integration ===");

const errorHandler = new SemanticErrorHandler();
const symbolTable = new SymbolTable();
const analyzer = new ErrorHandlingSemanticAnalyzer(errorHandler, symbolTable);

// Test semantic analysis
try {
    const tryStmt = new TryStatement(
        new BlockStatement([]),
        [new CatchClause("error", new Type("Error"), new BlockStatement([]))],
        new BlockStatement([])
    );
    
    analyzer.analyzeTryStatement(tryStmt);
    console.log("Semantic analysis successful");
} catch (error: Error) {
    console.log("Semantic analysis failed:", error.message);
}

if (errorHandler.hasErrors()) {
    console.log("Semantic errors found:", errorHandler.getErrors().length);
} else {
    console.log("No semantic errors found");
}

if (errorHandler.hasWarnings()) {
    console.log("Semantic warnings found:", errorHandler.getWarnings().length);
} else {
    console.log("No semantic warnings found");
}
)";
        
        saveTestFile("semantic_analysis_integration.ts", semanticIntegrationTest);
        std::cout << "Generated semantic analysis integration test" << std::endl;
    }
    
    void testCodeGenerationIntegration() {
        std::cout << "\n--- Testing Code Generation Integration ---" << std::endl;
        
        std::string codeGenIntegrationTest = R"(
// Code generation integration with error handling
class ErrorHandlingCodeGenerator {
    private output: string[] = [];
    private errorHandler: CodeGenErrorHandler;
    
    constructor(errorHandler: CodeGenErrorHandler) {
        this.errorHandler = errorHandler;
    }
    
    generateTryStatement(node: TryStatement): void {
        try {
            console.log("Generating try statement");
            
            this.output.push("try {");
            this.indent();
            
            // Generate try block
            if (node.getTryBlock()) {
                this.generateBlock(node.getTryBlock());
            }
            
            this.unindent();
            this.output.push("}");
            
            // Generate catch clauses
            for (const catchClause of node.getCatchClauses()) {
                this.generateCatchClause(catchClause);
            }
            
            // Generate finally block
            if (node.getFinallyBlock()) {
                this.output.push("finally {");
                this.indent();
                this.generateBlock(node.getFinallyBlock());
                this.unindent();
                this.output.push("}");
            }
        } catch (error: Error) {
            this.errorHandler.addError(new CodeGenError(
                `Error generating try statement: ${error.message}`,
                node.getLocation()
            ));
        }
    }
    
    generateCatchClause(node: CatchClause): void {
        try {
            console.log("Generating catch clause:", node.getParameterName());
            
            this.output.push("catch (");
            if (node.hasParameter()) {
                this.output.push(node.getParameterName());
                if (node.hasParameterType()) {
                    this.output.push(": " + node.getParameterType().getName());
                }
            }
            this.output.push(") {");
            this.indent();
            
            // Generate catch body
            if (node.getBody()) {
                this.generateBlock(node.getBody());
            }
            
            this.unindent();
            this.output.push("}");
        } catch (error: Error) {
            this.errorHandler.addError(new CodeGenError(
                `Error generating catch clause: ${error.message}`,
                node.getLocation()
            ));
        }
    }
    
    generateThrowStatement(node: ThrowStatement): void {
        try {
            console.log("Generating throw statement");
            
            this.output.push("throw");
            if (node.getExpression()) {
                this.output.push(" ");
                this.generateExpression(node.getExpression());
            }
            this.output.push(";");
        } catch (error: Error) {
            this.errorHandler.addError(new CodeGenError(
                `Error generating throw statement: ${error.message}`,
                node.getLocation()
            ));
        }
    }
    
    generatePanicStatement(node: PanicStatement): void {
        try {
            console.log("Generating panic statement");
            
            this.output.push("panic(");
            if (node.getExpression()) {
                this.generateExpression(node.getExpression());
            } else if (node.getMessage()) {
                this.output.push(`"${node.getMessage()}"`);
            }
            this.output.push(");");
        } catch (error: Error) {
            this.errorHandler.addError(new CodeGenError(
                `Error generating panic statement: ${error.message}`,
                node.getLocation()
            ));
        }
    }
    
    generateAbortStatement(node: AbortStatement): void {
        try {
            console.log("Generating abort statement");
            
            this.output.push("abort(");
            if (node.getExpression()) {
                this.generateExpression(node.getExpression());
            } else if (node.getReason()) {
                this.output.push(`"${node.getReason()}"`);
            }
            this.output.push(");");
        } catch (error: Error) {
            this.errorHandler.addError(new CodeGenError(
                `Error generating abort statement: ${error.message}`,
                node.getLocation()
            ));
        }
    }
    
    private generateBlock(node: ASTNode): void {
        if (node.getType() === ASTNodeType::BlockStatement) {
            const block = node as BlockStatement;
            for (const statement of block.getStatements()) {
                this.generateStatement(statement);
            }
        }
    }
    
    private generateExpression(node: ASTNode): void {
        if (node.getType() === ASTNodeType::Expression) {
            const expr = node as Expression;
            // Expression generation logic
            this.output.push(expr.toString());
        }
    }
    
    private generateStatement(node: ASTNode): void {
        switch (node.getType()) {
            case ASTNodeType::TryStatement:
                this.generateTryStatement(node as TryStatement);
                break;
            case ASTNodeType::ThrowStatement:
                this.generateThrowStatement(node as ThrowStatement);
                break;
            case ASTNodeType::PanicStatement:
                this.generatePanicStatement(node as PanicStatement);
                break;
            case ASTNodeType::AbortStatement:
                this.generateAbortStatement(node as AbortStatement);
                break;
            default:
                // Handle other statement types
                break;
        }
    }
    
    private indent(): void {
        // Indentation logic
    }
    
    private unindent(): void {
        // Unindentation logic
    }
    
    getGeneratedCode(): string {
        return this.output.join("\n");
    }
}

class CodeGenErrorHandler {
    private errors: CodeGenError[] = [];
    
    addError(error: CodeGenError): void {
        this.errors.push(error);
    }
    
    getErrors(): CodeGenError[] {
        return this.errors;
    }
    
    hasErrors(): boolean {
        return this.errors.length > 0;
    }
}

class CodeGenError extends Error {
    constructor(message: string, public location: SourceLocation) {
        super(message);
        this.name = "CodeGenError";
    }
}

// Test code generation integration
console.log("=== Testing Code Generation Integration ===");

const errorHandler = new CodeGenErrorHandler();
const generator = new ErrorHandlingCodeGenerator(errorHandler);

// Test code generation
try {
    const tryStmt = new TryStatement(
        new BlockStatement([]),
        [new CatchClause("error", new Type("Error"), new BlockStatement([]))],
        new BlockStatement([])
    );
    
    generator.generateTryStatement(tryStmt);
    const generatedCode = generator.getGeneratedCode();
    console.log("Generated code:", generatedCode);
    console.log("Code generation successful");
} catch (error: Error) {
    console.log("Code generation failed:", error.message);
}

if (errorHandler.hasErrors()) {
    console.log("Code generation errors found:", errorHandler.getErrors().length);
} else {
    console.log("No code generation errors found");
}
)";
        
        saveTestFile("code_generation_integration.ts", codeGenIntegrationTest);
        std::cout << "Generated code generation integration test" << std::endl;
    }
    
    void testPerformanceIntegration() {
        std::cout << "\n--- Testing Performance Integration ---" << std::endl;
        
        std::string performanceIntegrationTest = R"(
// Performance integration with error handling
class PerformanceMonitor {
    private startTime: number = 0;
    private endTime: number = 0;
    private memoryUsage: NodeJS.MemoryUsage | null = null;
    
    start(): void {
        this.startTime = performance.now();
        this.memoryUsage = process.memoryUsage();
    }
    
    end(): PerformanceMetrics {
        this.endTime = performance.now();
        const endMemoryUsage = process.memoryUsage();
        
        return {
            executionTime: this.endTime - this.startTime,
            memoryDelta: endMemoryUsage.heapUsed - (this.memoryUsage?.heapUsed || 0),
            peakMemory: endMemoryUsage.heapUsed,
            startMemory: this.memoryUsage?.heapUsed || 0
        };
    }
}

interface PerformanceMetrics {
    executionTime: number;
    memoryDelta: number;
    peakMemory: number;
    startMemory: number;
}

// Performance testing with error handling
class ErrorHandlingPerformanceTest {
    private monitor: PerformanceMonitor;
    
    constructor() {
        this.monitor = new PerformanceMonitor();
    }
    
    testTryCatchPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();
        
        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 100 === 0) {
                    throw new Error(`Error at iteration ${i}`);
                }
                // Simulate work
                Math.random();
            } catch (error: Error) {
                // Handle error
                console.log("Caught error:", error.message);
            }
        }
        
        return this.monitor.end();
    }
    
    testErrorPropagationPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();
        
        for (let i = 0; i < iterations; i++) {
            const result = this.simulateErrorPropagation(i);
            if (result.isErr()) {
                // Handle error
                console.log("Error propagated:", result.unwrapOr(""));
            }
        }
        
        return this.monitor.end();
    }
    
    testPanicAbortPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();
        
        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 1000 === 0) {
                    this.simulatePanic(`Panic at iteration ${i}`);
                }
                // Simulate work
                Math.random();
            } catch (error: Error) {
                // Handle panic
                console.log("Panic handled:", error.message);
            }
        }
        
        return this.monitor.end();
    }
    
    testCustomExceptionPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();
        
        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 50 === 0) {
                    throw new CustomPerformanceError(`Custom error at iteration ${i}`, i);
                }
                // Simulate work
                Math.random();
            } catch (error: CustomPerformanceError) {
                // Handle custom error
                console.log("Custom error handled:", error.message, error.code);
            }
        }
        
        return this.monitor.end();
    }
    
    private simulateErrorPropagation(value: number): Result<number, string> {
        if (value % 100 === 0) {
            return Result.err(`Error at value ${value}`);
        }
        return Result.ok(value * 2);
    }
    
    private simulatePanic(message: string): never {
        throw new Error(`PANIC: ${message}`);
    }
}

class CustomPerformanceError extends Error {
    constructor(message: string, public code: number) {
        super(message);
        this.name = "CustomPerformanceError";
    }
}

// Performance comparison
class PerformanceComparison {
    static compareErrorHandlingMethods(iterations: number): void {
        const test = new ErrorHandlingPerformanceTest();
        
        console.log("=== Performance Comparison ===");
        console.log(`Testing with ${iterations} iterations`);
        
        // Test try-catch performance
        const tryCatchMetrics = test.testTryCatchPerformance(iterations);
        console.log("Try-Catch Performance:");
        console.log(`  Execution time: ${tryCatchMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${tryCatchMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${tryCatchMetrics.peakMemory} bytes`);
        
        // Test error propagation performance
        const errorPropagationMetrics = test.testErrorPropagationPerformance(iterations);
        console.log("Error Propagation Performance:");
        console.log(`  Execution time: ${errorPropagationMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${errorPropagationMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${errorPropagationMetrics.peakMemory} bytes`);
        
        // Test panic/abort performance
        const panicAbortMetrics = test.testPanicAbortPerformance(iterations);
        console.log("Panic/Abort Performance:");
        console.log(`  Execution time: ${panicAbortMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${panicAbortMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${panicAbortMetrics.peakMemory} bytes`);
        
        // Test custom exception performance
        const customExceptionMetrics = test.testCustomExceptionPerformance(iterations);
        console.log("Custom Exception Performance:");
        console.log(`  Execution time: ${customExceptionMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${customExceptionMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${customExceptionMetrics.peakMemory} bytes`);
        
        // Calculate performance ratios
        const tryCatchTime = tryCatchMetrics.executionTime;
        const errorPropagationTime = errorPropagationMetrics.executionTime;
        const panicAbortTime = panicAbortMetrics.executionTime;
        const customExceptionTime = customExceptionMetrics.executionTime;
        
        console.log("\nPerformance Ratios (relative to try-catch):");
        console.log(`  Error Propagation: ${(errorPropagationTime / tryCatchTime).toFixed(2)}x`);
        console.log(`  Panic/Abort: ${(panicAbortTime / tryCatchTime).toFixed(2)}x`);
        console.log(`  Custom Exception: ${(customExceptionTime / tryCatchTime).toFixed(2)}x`);
    }
}

// Run performance tests
console.log("=== Testing Performance Integration ===");

// Test with different iteration counts
PerformanceComparison.compareErrorHandlingMethods(1000);
PerformanceComparison.compareErrorHandlingMethods(10000);
PerformanceComparison.compareErrorHandlingMethods(100000);
)";
        
        saveTestFile("performance_integration.ts", performanceIntegrationTest);
        std::cout << "Generated performance integration test" << std::endl;
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }
};

int main() {
    try {
        ErrorHandlingIntegrationTests tests;
        tests.runIntegrationTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
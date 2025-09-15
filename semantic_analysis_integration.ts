
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

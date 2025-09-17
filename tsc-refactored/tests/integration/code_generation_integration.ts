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

    getGeneratedCode(): string {
        return this.output.join("\n");
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

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

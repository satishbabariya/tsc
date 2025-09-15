
// Exception system testing
class CustomError extends Error {
    constructor(message: string) {
        super(message);
        this.name = "CustomError";
    }
}

class ValidationError extends Error {
    constructor(message: string, field: string) {
        super(message);
        this.name = "ValidationError";
        this.field = field;
    }
    
    field: string;
}

// Basic try-catch-finally
function basicExceptionHandling(): void {
    try {
        console.log("Executing try block");
        throw new Error("Something went wrong");
    } catch (error) {
        console.log("Caught error:", error.message);
    } finally {
        console.log("Finally block executed");
    }
}

// Multiple catch clauses
function multipleCatchHandling(): void {
    try {
        const random = Math.random();
        if (random < 0.3) {
            throw new CustomError("Custom error occurred");
        } else if (random < 0.6) {
            throw new ValidationError("Validation failed", "email");
        } else {
            throw new Error("Generic error");
        }
    } catch (error: CustomError) {
        console.log("Caught custom error:", error.message);
    } catch (error: ValidationError) {
        console.log("Caught validation error:", error.message, "Field:", error.field);
    } catch (error: Error) {
        console.log("Caught generic error:", error.message);
    } finally {
        console.log("Cleanup completed");
    }
}

// Nested try-catch
function nestedExceptionHandling(): void {
    try {
        console.log("Outer try block");
        try {
            console.log("Inner try block");
            throw new Error("Inner error");
        } catch (innerError) {
            console.log("Inner catch:", innerError.message);
            throw new Error("Re-thrown error");
        }
    } catch (outerError) {
        console.log("Outer catch:", outerError.message);
    }
}

// Exception in finally block
function exceptionInFinally(): void {
    try {
        console.log("Try block");
    } catch (error) {
        console.log("Catch block");
    } finally {
        try {
            console.log("Finally block");
            throw new Error("Error in finally");
        } catch (finallyError) {
            console.log("Error in finally caught:", finallyError.message);
        }
    }
}

// Resource management with try-finally
function resourceManagement(): void {
    let resource: string | null = null;
    try {
        resource = "acquired resource";
        console.log("Using resource:", resource);
        // Simulate work that might fail
        if (Math.random() > 0.5) {
            throw new Error("Work failed");
        }
    } catch (error) {
        console.log("Work failed:", error.message);
    } finally {
        if (resource) {
            console.log("Releasing resource:", resource);
            resource = null;
        }
    }
}

// Test exception system
console.log("=== Testing Exception System ===");
basicExceptionHandling();
multipleCatchHandling();
nestedExceptionHandling();
exceptionInFinally();
resourceManagement();

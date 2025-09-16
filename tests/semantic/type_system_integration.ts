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

const user: User = {id: "1", name: "Alice", email: "alice@example.com"};
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

const inferredResult = inferUserType({id: "2", name: "Bob", email: "bob@example.com"});
if (inferredResult.isOk()) {
    console.log("Inferred user:", inferredResult.unwrap());
} else {
    console.log("Inference failed:", inferredResult.unwrapOr({} as User));
}

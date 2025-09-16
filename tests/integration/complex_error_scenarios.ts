// Complex error scenarios testing
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

interface DatabaseError {
    code: string;
    message: string;
    query?: string;
}

interface NetworkError {
    status: number;
    message: string;
    url: string;
}

// Complex error handling with multiple error types
class ComplexErrorHandler {
    static async handleApiCall<T>(url: string): Promise<ApiResponse<T>> {
        try {
            // Simulate API call
            await new Promise(resolve => setTimeout(resolve, 100));

            if (url.includes("error")) {
                throw new NetworkError(500, "Internal server error", url);
            }

            if (url.includes("notfound")) {
                throw new NetworkError(404, "Resource not found", url);
            }

            return {
                data: {} as T,
                success: true
            };
        } catch (error) {
            if (error instanceof NetworkError) {
                return {
                    data: {} as T,
                    success: false,
                    error: `Network error: ${error.message} (${error.status})`
                };
            }

            return {
                data: {} as T,
                success: false,
                error: `Unknown error: ${error}`
            };
        }
    }

    static async handleDatabaseOperation<T>(query: string): Promise<T> {
        try {
            // Simulate database operation
            await new Promise(resolve => setTimeout(resolve, 50));

            if (query.includes("error")) {
                throw new DatabaseError("DB_ERROR", "Database operation failed", query);
            }

            if (query.includes("timeout")) {
                throw new DatabaseError("DB_TIMEOUT", "Database operation timed out", query);
            }

            return {} as T;
        } catch (error) {
            if (error instanceof DatabaseError) {
                throw new Error(`Database error [${error.code}]: ${error.message}`);
            }
            throw error;
        }
    }
}

// Error recovery strategies
class ErrorRecovery {
    static async retryWithBackoff<T>(
        operation: () => Promise<T>,
        maxRetries: number = 3,
        baseDelay: number = 1000
    ): Promise<T> {
        let lastError: Error | null = null;

        for (let attempt = 0; attempt < maxRetries; attempt++) {
            try {
                return await operation();
            } catch (error) {
                lastError = error as Error;

                if (attempt === maxRetries - 1) {
                    throw lastError;
                }

                const delay = baseDelay * Math.pow(2, attempt);
                console.log(`Attempt ${attempt + 1} failed, retrying in ${delay}ms`);
                await new Promise(resolve => setTimeout(resolve, delay));
            }
        }

        throw lastError!;
    }

    static async fallbackOperation<T>(
        primaryOperation: () => Promise<T>,
        fallbackOperation: () => Promise<T>
    ): Promise<T> {
        try {
            return await primaryOperation();
        } catch (error) {
            console.log("Primary operation failed, trying fallback");
            try {
                return await fallbackOperation();
            } catch (fallbackError) {
                throw new Error(`Both primary and fallback operations failed. Primary: ${error}, Fallback: ${fallbackError}`);
            }
        }
    }
}

// Circuit breaker pattern
class CircuitBreaker {
    private failures: number = 0;
    private lastFailureTime: number = 0;
    private state: "CLOSED" | "OPEN" | "HALF_OPEN" = "CLOSED";

    constructor(
        private failureThreshold: number = 5,
        private timeout: number = 60000
    ) {
    }

    async execute<T>(operation: () => Promise<T>): Promise<T> {
        if (this.state === "OPEN") {
            if (Date.now() - this.lastFailureTime > this.timeout) {
                this.state = "HALF_OPEN";
            } else {
                throw new Error("Circuit breaker is OPEN");
            }
        }

        try {
            const result = await operation();
            this.onSuccess();
            return result;
        } catch (error) {
            this.onFailure();
            throw error;
        }
    }

    private onSuccess(): void {
        this.failures = 0;
        this.state = "CLOSED";
    }

    private onFailure(): void {
        this.failures++;
        this.lastFailureTime = Date.now();

        if (this.failures >= this.failureThreshold) {
            this.state = "OPEN";
        }
    }
}

// Error aggregation
class ErrorAggregator {
    private errors: Error[] = [];

    addError(error: Error): void {
        this.errors.push(error);
    }

    hasErrors(): boolean {
        return this.errors.length > 0;
    }

    getErrors(): Error[] {
        return [...this.errors];
    }

    getCombinedMessage(): string {
        return this.errors.map(e => e.message).join("; ");
    }

    clear(): void {
        this.errors = [];
    }
}

// Complex error scenario testing
async function testComplexErrorScenarios(): Promise<void> {
    console.log("=== Testing Complex Error Scenarios ===");

    // Test API error handling
    const apiResult = await ComplexErrorHandler.handleApiCall<User>("https://api.example.com/users");
    console.log("API result:", apiResult);

    const apiErrorResult = await ComplexErrorHandler.handleApiCall<User>("https://api.example.com/error");
    console.log("API error result:", apiErrorResult);

    // Test database error handling
    try {
        await ComplexErrorHandler.handleDatabaseOperation<User>("SELECT * FROM users");
        console.log("Database operation succeeded");
    } catch (error) {
        console.log("Database operation failed:", error.message);
    }

    try {
        await ComplexErrorHandler.handleDatabaseOperation<User>("SELECT * FROM error");
        console.log("Database operation succeeded");
    } catch (error) {
        console.log("Database operation failed:", error.message);
    }

    // Test error recovery
    const recoveryResult = await ErrorRecovery.retryWithBackoff(async () => {
        if (Math.random() > 0.7) {
            throw new Error("Random failure");
        }
        return "Success";
    });
    console.log("Recovery result:", recoveryResult);

    // Test fallback operation
    const fallbackResult = await ErrorRecovery.fallbackOperation(
        async () => {
            throw new Error("Primary operation failed");
        },
        async () => {
            return "Fallback success";
        }
    );
    console.log("Fallback result:", fallbackResult);

    // Test circuit breaker
    const circuitBreaker = new CircuitBreaker(3, 5000);

    try {
        const circuitResult = await circuitBreaker.execute(async () => {
            if (Math.random() > 0.5) {
                throw new Error("Circuit breaker test failure");
            }
            return "Circuit breaker success";
        });
        console.log("Circuit breaker result:", circuitResult);
    } catch (error) {
        console.log("Circuit breaker error:", error.message);
    }

    // Test error aggregation
    const errorAggregator = new ErrorAggregator();
    errorAggregator.addError(new Error("First error"));
    errorAggregator.addError(new Error("Second error"));
    errorAggregator.addError(new Error("Third error"));

    if (errorAggregator.hasErrors()) {
        console.log("Aggregated errors:", errorAggregator.getCombinedMessage());
    }
}

// Run complex error scenario tests
testComplexErrorScenarios();

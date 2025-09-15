
// Custom exceptions testing
abstract class BaseException extends Error {
    abstract readonly code: number;
    abstract readonly category: string;
    
    constructor(message: string) {
        super(message);
        this.name = this.constructor.name;
    }
    
    abstract getDetails(): Record<string, any>;
}

class NetworkException extends BaseException {
    readonly code = 1000;
    readonly category = "Network";
    
    constructor(message: string, public url: string, public statusCode?: number) {
        super(message);
    }
    
    getDetails(): Record<string, any> {
        return {
            url: this.url,
            statusCode: this.statusCode,
            timestamp: new Date().toISOString()
        };
    }
}

class DatabaseException extends BaseException {
    readonly code = 2000;
    readonly category = "Database";
    
    constructor(message: string, public query: string, public table?: string) {
        super(message);
    }
    
    getDetails(): Record<string, any> {
        return {
            query: this.query,
            table: this.table,
            timestamp: new Date().toISOString()
        };
    }
}

class ValidationException extends BaseException {
    readonly code = 3000;
    readonly category = "Validation";
    
    constructor(message: string, public field: string, public value: any) {
        super(message);
    }
    
    getDetails(): Record<string, any> {
        return {
            field: this.field,
            value: this.value,
            timestamp: new Date().toISOString()
        };
    }
}

// Exception factory
class ExceptionFactory {
    static createNetworkException(url: string, statusCode: number): NetworkException {
        return new NetworkException(`Network request failed for ${url}`, url, statusCode);
    }
    
    static createDatabaseException(query: string, table: string): DatabaseException {
        return new DatabaseException(`Database query failed: ${query}`, query, table);
    }
    
    static createValidationException(field: string, value: any): ValidationException {
        return new ValidationException(`Validation failed for field ${field}`, field, value);
    }
}

// Exception handler
class ExceptionHandler {
    static handle(exception: BaseException): void {
        console.log(`Exception [${exception.category}] ${exception.code}: ${exception.message}`);
        console.log("Details:", exception.getDetails());
        
        switch (exception.category) {
            case "Network":
                this.handleNetworkException(exception as NetworkException);
                break;
            case "Database":
                this.handleDatabaseException(exception as DatabaseException);
                break;
            case "Validation":
                this.handleValidationException(exception as ValidationException);
                break;
            default:
                console.log("Unknown exception category");
        }
    }
    
    private static handleNetworkException(exception: NetworkException): void {
        console.log(`Network error for URL: ${exception.url}`);
        if (exception.statusCode) {
            console.log(`Status code: ${exception.statusCode}`);
        }
    }
    
    private static handleDatabaseException(exception: DatabaseException): void {
        console.log(`Database error for query: ${exception.query}`);
        if (exception.table) {
            console.log(`Table: ${exception.table}`);
        }
    }
    
    private static handleValidationException(exception: ValidationException): void {
        console.log(`Validation error for field: ${exception.field}`);
        console.log(`Value: ${exception.value}`);
    }
}

// Test custom exceptions
function testCustomExceptions(): void {
    try {
        // Simulate different error scenarios
        const random = Math.random();
        if (random < 0.33) {
            throw ExceptionFactory.createNetworkException("https://api.example.com", 500);
        } else if (random < 0.66) {
            throw ExceptionFactory.createDatabaseException("SELECT * FROM users", "users");
        } else {
            throw ExceptionFactory.createValidationException("email", "invalid-email");
        }
    } catch (exception: BaseException) {
        ExceptionHandler.handle(exception);
    }
}

// Exception chaining
function testExceptionChaining(): void {
    try {
        try {
            throw new NetworkException("Initial network error", "https://api.example.com", 404);
        } catch (networkError) {
            throw new DatabaseException("Database operation failed", "SELECT * FROM users", "users");
        }
    } catch (databaseError) {
        console.log("Caught chained exception:", databaseError.message);
        console.log("Original cause might be network related");
    }
}

// Exception with custom properties
class CustomBusinessException extends BaseException {
    readonly code = 4000;
    readonly category = "Business";
    
    constructor(message: string, public operation: string, public userId?: string) {
        super(message);
    }
    
    getDetails(): Record<string, any> {
        return {
            operation: this.operation,
            userId: this.userId,
            timestamp: new Date().toISOString()
        };
    }
}

// Test custom exceptions
console.log("=== Testing Custom Exceptions ===");
testCustomExceptions();
testExceptionChaining();

// Test business exception
try {
    throw new CustomBusinessException("User operation failed", "updateProfile", "user123");
} catch (businessError: CustomBusinessException) {
    ExceptionHandler.handle(businessError);
}

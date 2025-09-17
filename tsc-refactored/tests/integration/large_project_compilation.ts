// Large project compilation benchmark with error handling
// This simulates a large TypeScript project with extensive error handling

// Core error handling infrastructure
abstract class BaseError extends Error {
    abstract readonly code: number;
    abstract readonly category: string;

    constructor(message: string) {
        super(message);
        this.name = this.constructor.name;
    }

    abstract getDetails(): Record<string, any>;
}

class NetworkError extends BaseError {
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

class DatabaseError extends BaseError {
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

class ValidationError extends BaseError {
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

// Result type for explicit error handling
class Result<T, E> {
    private constructor(
        private readonly success: boolean,
        private readonly value?: T,
        private readonly error?: E
    ) {
    }

    static ok<T>(value: T): Result<T, never> {
        return new Result(true, value);
    }

    static err<E>(error: E): Result<never, E> {
        return new Result(false, undefined, error);
    }

    isOk(): boolean {
        return this.success;
    }

    isErr(): boolean {
        return !this.success;
    }

    unwrap(): T {
        if (this.success) {
            return this.value!;
        }
        throw new Error(`Called unwrap on Err: ${this.error}`);
    }

    unwrapOr(defaultValue: T): T {
        return this.success ? this.value! : defaultValue;
    }

    map<U>(fn: (value: T) => U): Result<U, E> {
        if (this.success) {
            return Result.ok(fn(this.value!));
        }
        return Result.err(this.error!);
    }

    mapErr<F>(fn: (error: E) => F): Result<T, F> {
        if (this.success) {
            return Result.ok(this.value!);
        }
        return Result.err(fn(this.error!));
    }
}

// Large project modules
class UserService {
    async getUser(id: string): Promise<Result<User, BaseError>> {
        try {
            // Simulate database operation
            await new Promise(resolve => setTimeout(resolve, 10));

            if (!id || id.length === 0) {
                throw new ValidationError("User ID is required", "id", id);
            }

            if (id === "error") {
                throw new DatabaseError("User not found", "SELECT * FROM users WHERE id = ?", "users");
            }

            return Result.ok({
                id,
                name: "John Doe",
                email: "john@example.com"
            });
        } catch (error: BaseError) {
            return Result.err(error);
        } catch (error: Error) {
            return Result.err(new DatabaseError(`Unexpected error: ${error.message}`, "getUser", "users"));
        }
    }

    async createUser(userData: Partial<User>): Promise<Result<User, BaseError>> {
        try {
            // Validate user data
            if (!userData.name) {
                throw new ValidationError("Name is required", "name", userData.name);
            }

            if (!userData.email) {
                throw new ValidationError("Email is required", "email", userData.email);
            }

            // Simulate database operation
            await new Promise(resolve => setTimeout(resolve, 15));

            return Result.ok({
                id: Math.random().toString(36).substr(2, 9),
                name: userData.name,
                email: userData.email
            });
        } catch (error: BaseError) {
            return Result.err(error);
        } catch (error: Error) {
            return Result.err(new DatabaseError(`Unexpected error: ${error.message}`, "createUser", "users"));
        }
    }
}

class ApiService {
    async fetchData(url: string): Promise<Result<any, BaseError>> {
        try {
            // Simulate network request
            await new Promise(resolve => setTimeout(resolve, 20));

            if (!url || !url.startsWith("http")) {
                throw new ValidationError("Invalid URL", "url", url);
            }

            if (url.includes("error")) {
                throw new NetworkError("Network request failed", url, 500);
            }

            return Result.ok({data: "success", url});
        } catch (error: BaseError) {
            return Result.err(error);
        } catch (error: Error) {
            return Result.err(new NetworkError(`Unexpected error: ${error.message}`, url));
        }
    }
}

class ValidationService {
    validateEmail(email: string): Result<string, BaseError> {
        try {
            if (!email) {
                throw new ValidationError("Email is required", "email", email);
            }

            const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
            if (!emailRegex.test(email)) {
                throw new ValidationError("Invalid email format", "email", email);
            }

            return Result.ok(email.toLowerCase());
        } catch (error: BaseError) {
            return Result.err(error);
        }
    }

    validatePassword(password: string): Result<string, BaseError> {
        try {
            if (!password) {
                throw new ValidationError("Password is required", "password", password);
            }

            if (password.length < 8) {
                throw new ValidationError("Password must be at least 8 characters", "password", password);
            }

            return Result.ok(password);
        } catch (error: BaseError) {
            return Result.err(error);
        }
    }
}

// Complex business logic with error handling
class BusinessLogicService {
    private userService: UserService;
    private apiService: ApiService;
    private validationService: ValidationService;

    constructor() {
        this.userService = new UserService();
        this.apiService = new ApiService();
        this.validationService = new ValidationService();
    }

    async processUserRegistration(userData: Partial<User>): Promise<Result<User, BaseError>> {
        try {
            // Validate input
            const emailResult = this.validationService.validateEmail(userData.email || "");
            if (emailResult.isErr()) {
                return emailResult;
            }

            const passwordResult = this.validationService.validatePassword(userData.password || "");
            if (passwordResult.isErr()) {
                return passwordResult;
            }

            // Create user
            const createResult = await this.userService.createUser({
                name: userData.name,
                email: emailResult.unwrap()
            });

            if (createResult.isErr()) {
                return createResult;
            }

            // Fetch additional data
            const apiResult = await this.apiService.fetchData("https://api.example.com/user-data");
            if (apiResult.isErr()) {
                // Log error but don't fail registration
                console.log("Failed to fetch additional data:", apiResult.unwrapOr(""));
            }

            return createResult;
        } catch (error: Error) {
            return Result.err(new ValidationError(`Unexpected error: ${error.message}`, "processUserRegistration", userData));
        }
    }

    async processUserLogin(email: string, password: string): Promise<Result<User, BaseError>> {
        try {
            // Validate input
            const emailResult = this.validationService.validateEmail(email);
            if (emailResult.isErr()) {
                return emailResult;
            }

            const passwordResult = this.validationService.validatePassword(password);
            if (passwordResult.isErr()) {
                return passwordResult;
            }

            // Get user
            const userResult = await this.userService.getUser(emailResult.unwrap());
            if (userResult.isErr()) {
                return userResult;
            }

            return userResult;
        } catch (error: Error) {
            return Result.err(new ValidationError(`Unexpected error: ${error.message}`, "processUserLogin", {
                email,
                password
            }));
        }
    }
}

// Test large project compilation
console.log("=== Large Project Compilation Benchmark ===");

const businessLogic = new BusinessLogicService();

// Test user registration
businessLogic.processUserRegistration({
    name: "Alice",
    email: "alice@example.com",
    password: "securepassword123"
}).then(result => {
    if (result.isOk()) {
        console.log("User registration successful:", result.unwrap());
    } else {
        console.log("User registration failed:", result.unwrapOr({} as User));
    }
});

// Test user login
businessLogic.processUserLogin("alice@example.com", "securepassword123").then(result => {
    if (result.isOk()) {
        console.log("User login successful:", result.unwrap());
    } else {
        console.log("User login failed:", result.unwrapOr({} as User));
    }
});

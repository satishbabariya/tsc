
// Error propagation testing
class Result<T, E> {
    private constructor(
        private readonly success: boolean,
        private readonly value?: T,
        private readonly error?: E
    ) {}
    
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

// Error propagation with Result type
function divide(a: number, b: number): Result<number, string> {
    if (b === 0) {
        return Result.err("Division by zero");
    }
    return Result.ok(a / b);
}

function sqrt(value: number): Result<number, string> {
    if (value < 0) {
        return Result.err("Cannot take square root of negative number");
    }
    return Result.ok(Math.sqrt(value));
}

function processNumbers(a: number, b: number): Result<number, string> {
    const divisionResult = divide(a, b);
    if (divisionResult.isErr()) {
        return divisionResult;
    }
    
    const sqrtResult = sqrt(divisionResult.unwrap());
    if (sqrtResult.isErr()) {
        return sqrtResult;
    }
    
    return Result.ok(sqrtResult.unwrap());
}

// Error propagation with optional chaining
class User {
    constructor(public name: string, public email?: string) {}
}

class UserService {
    private users: Map<string, User> = new Map();
    
    addUser(user: User): Result<User, string> {
        if (this.users.has(user.name)) {
            return Result.err(`User ${user.name} already exists`);
        }
        this.users.set(user.name, user);
        return Result.ok(user);
    }
    
    getUser(name: string): Result<User, string> {
        const user = this.users.get(name);
        if (!user) {
            return Result.err(`User ${name} not found`);
        }
        return Result.ok(user);
    }
    
    getUserEmail(name: string): Result<string, string> {
        const userResult = this.getUser(name);
        if (userResult.isErr()) {
            return userResult;
        }
        
        const user = userResult.unwrap();
        if (!user.email) {
            return Result.err(`User ${name} has no email`);
        }
        
        return Result.ok(user.email);
    }
}

// Error propagation with async operations
async function fetchUserData(userId: string): Promise<Result<User, string>> {
    try {
        // Simulate async operation
        await new Promise(resolve => setTimeout(resolve, 100));
        
        if (userId === "invalid") {
            return Result.err("Invalid user ID");
        }
        
        return Result.ok(new User("John Doe", "john@example.com"));
    } catch (error) {
        return Result.err(`Network error: ${error}`);
    }
}

async function processUserData(userId: string): Promise<Result<string, string>> {
    const userResult = await fetchUserData(userId);
    if (userResult.isErr()) {
        return userResult;
    }
    
    const user = userResult.unwrap();
    if (!user.email) {
        return Result.err("User has no email");
    }
    
    return Result.ok(user.email.toUpperCase());
}

// Error propagation with function composition
function compose<T, U, V>(
    f: (x: T) => Result<U, string>,
    g: (x: U) => Result<V, string>
): (x: T) => Result<V, string> {
    return (x: T) => {
        const result = f(x);
        if (result.isErr()) {
            return result;
        }
        return g(result.unwrap());
    };
}

// Test error propagation
function testErrorPropagation(): void {
    console.log("=== Testing Error Propagation ===");
    
    // Test Result type
    const result1 = processNumbers(16, 2);
    if (result1.isOk()) {
        console.log("Result:", result1.unwrap());
    } else {
        console.log("Error:", result1.unwrapOr(0));
    }
    
    const result2 = processNumbers(16, 0);
    if (result2.isErr()) {
        console.log("Error:", result2.unwrapOr(0));
    }
    
    // Test UserService
    const userService = new UserService();
    const addResult = userService.addUser(new User("Alice", "alice@example.com"));
    if (addResult.isOk()) {
        console.log("User added:", addResult.unwrap().name);
    }
    
    const emailResult = userService.getUserEmail("Alice");
    if (emailResult.isOk()) {
        console.log("User email:", emailResult.unwrap());
    }
    
    // Test function composition
    const composedFunction = compose(
        (x: number) => divide(x, 2),
        (x: number) => sqrt(x)
    );
    
    const composedResult = composedFunction(16);
    if (composedResult.isOk()) {
        console.log("Composed result:", composedResult.unwrap());
    } else {
        console.log("Composed error:", composedResult.unwrapOr(0));
    }
}

// Test async error propagation
async function testAsyncErrorPropagation(): Promise<void> {
    console.log("=== Testing Async Error Propagation ===");
    
    const result1 = await processUserData("valid");
    if (result1.isOk()) {
        console.log("Processed email:", result1.unwrap());
    } else {
        console.log("Processing error:", result1.unwrapOr(""));
    }
    
    const result2 = await processUserData("invalid");
    if (result2.isErr()) {
        console.log("Processing error:", result2.unwrapOr(""));
    }
}

// Run tests
testErrorPropagation();
testAsyncErrorPropagation();

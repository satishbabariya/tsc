// Complex function scenarios testing
interface ApiResponse<T> {
    data: T;
    status: number;
    message: string;
}

// Generic function with overloads
function apiCall<T>(url: string): Promise<ApiResponse<T>>;
function apiCall<T>(url: string, options: { method: string; body?: any }): Promise<ApiResponse<T>>;
function apiCall<T>(url: string, options?: { method: string; body?: any }): Promise<ApiResponse<T>> {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve({
                data: {} as T,
                status: 200,
                message: "Success"
            });
        }, 100);
    });
}

// Test generic function with overloads
apiCall<User>("/api/users").then(response => {
    console.log("API response:", response);
});

// Function with complex parameter types
function processData<T extends Record<string, any>>(
    data: T,
    transformer: (item: T) => T,
    validator?: (item: T) => boolean
): T {
    const transformed = transformer(data);
    if (validator && !validator(transformed)) {
        throw new Error("Validation failed");
    }
    return transformed;
}

interface User {
    id: number;
    name: string;
    email: string;
}

const userData: User = {id: 1, name: "Alice", email: "alice@example.com"};

const processedUser = processData(
    userData,
    (user) => ({...user, name: user.name.toUpperCase()}),
    (user) => user.email.includes("@")
);

console.log("Processed user:", processedUser);

// Function with callback and error handling
function withErrorHandling<T>(
    operation: () => T,
    onError: (error: Error) => void
): T | null {
    try {
        return operation();
    } catch (error) {
        onError(error as Error);
        return null;
    }
}

const result = withErrorHandling(
    () => {
        // Simulate operation that might fail
        if (Math.random() > 0.5) {
            throw new Error("Random failure");
        }
        return "Success!";
    },
    (error) => console.error("Error occurred:", error.message)
);

console.log("Operation result:", result);

// Function with conditional return types
function getValue<T>(value: T): T extends string ? string : T extends number ? number : T {
    return value as any;
}

const stringValue = getValue("hello");
const numberValue = getValue(42);
const booleanValue = getValue(true);

console.log("String value:", stringValue);
console.log("Number value:", numberValue);
console.log("Boolean value:", booleanValue);

// Function with method chaining
class QueryBuilder<T> {
    private conditions: Array<(item: T) => boolean> = [];

    where = (condition: (item: T) => boolean): QueryBuilder<T> => {
        this.conditions.push(condition);
        return this;
    };

    execute = (data: T[]): T[] => {
        return data.filter(item =>
            this.conditions.every(condition => condition(item))
        );
    };
}

const users: User[] = [
    {id: 1, name: "Alice", email: "alice@example.com"},
    {id: 2, name: "Bob", email: "bob@example.com"},
    {id: 3, name: "Charlie", email: "charlie@example.com"}
];

const query = new QueryBuilder<User>();
const filteredUsers = query
    .where(user => user.id > 1)
    .where(user => user.name.startsWith("B"))
    .execute(users);

console.log("Filtered users:", filteredUsers);

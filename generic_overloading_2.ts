
// Complex generic overloading scenarios
interface ApiResponse<T> {
    data: T;
    status: number;
    message: string;
}

interface ErrorResponse {
    error: string;
    status: number;
}

class ApiClient {
    // Overload 1: GET request
    request<T>(url: string): Promise<ApiResponse<T>>;
    
    // Overload 2: POST request with data
    request<T, U>(url: string, method: "POST", data: T): Promise<ApiResponse<U>>;
    
    // Overload 3: PUT request with data
    request<T, U>(url: string, method: "PUT", data: T): Promise<ApiResponse<U>>;
    
    // Overload 4: DELETE request
    request<T>(url: string, method: "DELETE"): Promise<ApiResponse<T>>;
    
    // Implementation
    async request<T, U = T>(url: string, method?: string, data?: T): Promise<ApiResponse<U>> {
        // Simulated API call
        return {
            data: {} as U,
            status: 200,
            message: "Success"
        };
    }
}

// Generic utility functions with overloading
function parse<T>(input: string): T;
function parse<T>(input: string, parser: (input: string) => T): T;
function parse<T>(input: string, parser?: (input: string) => T): T {
    if (parser) {
        return parser(input);
    } else {
        // Default JSON parsing
        return JSON.parse(input) as T;
    }
}

// Test the complex overloading
const apiClient = new ApiClient();

// Test different request types
const getResponse = await apiClient.request<User>("/api/users/1");
const postResponse = await apiClient.request<CreateUserRequest, User>("/api/users", "POST", { name: "John", email: "john@example.com" });
const putResponse = await apiClient.request<UpdateUserRequest, User>("/api/users/1", "PUT", { name: "Jane" });
const deleteResponse = await apiClient.request<void>("/api/users/1", "DELETE");

console.log("GET response:", getResponse);
console.log("POST response:", postResponse);
console.log("PUT response:", putResponse);
console.log("DELETE response:", deleteResponse);

// Test parsing overloading
const parsedJson = parse<User>('{"id": 1, "name": "John"}');
const parsedCustom = parse<number>("42", input => parseInt(input));

console.log("Parsed JSON:", parsedJson);
console.log("Parsed custom:", parsedCustom);

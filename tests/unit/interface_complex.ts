// Complex interface combinations
interface DatabaseConfig {
    host: string;
    port: number;
    database: string;
    username?: string;
    password?: string;
    ssl: boolean | "required" | "optional";
}

interface APIResponse<T> {
    success: boolean;
    data: T | null;
    error?: string;
    timestamp: string;
}

interface UserService {
    createUser(userData: { name: string; email: string }): Promise<APIResponse<User>>;
    getUser(id: string | number): Promise<APIResponse<User>>;
    updateUser(id: string | number, updates: Partial<User>): Promise<APIResponse<User>>;
    deleteUser(id: string | number): Promise<APIResponse<void>>;
}

interface ValidationRule {
    field: string;
    required?: boolean;
    type: "string" | "number" | "boolean" | "email";
    minLength?: number;
    maxLength?: number;
    min?: number;
    max?: number;
}
// Interface with optional properties
interface UserProfile {
    id: string;
    name: string;
    email?: string;  // Optional
    age?: number;    // Optional
    phone?: string;  // Optional
}

interface Settings {
    theme: "light" | "dark";
    language?: string;
    notifications?: boolean;
    autoSave?: boolean;
}

// All optional properties
interface OptionalConfig {
    host?: string;
    port?: number;
    ssl?: boolean;
}
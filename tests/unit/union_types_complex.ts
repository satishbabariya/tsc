// Complex union type tests with nested structures
class User {
    id: string | number;
    profile: UserProfile | null;
}

class UserProfile {
    name: string;
    status: "active" | "inactive" | "pending";
    preferences: (string | number)[];
}

// Complex function signatures
function processUser(user: User | null): string | number {
    if (user === null) return "no user";
    return user.id;
}

function updateProfile(
    user: User,
    updates: Partial<UserProfile> | null
): "success" | "error" | null {
    if (updates === null) return null;
    // Update logic here
    return "success";
}

// Nested union types
let complexUnion: (string | number)[] | (boolean | null)[] | User[];
let deeplyNested: ((string | number)[] | User) | null;

// Union with function types
type Handler = (value: string | number) => string | number;
let myHandler: Handler | null;

// Interface with union types
interface Config {
    mode: "development" | "production" | "test";
    timeout: number | null;
    features: (string | boolean)[];
}

let config: Config | null;
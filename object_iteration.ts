
// Object iteration testing
interface User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
}

function testObjectIteration() {
    const users: User[] = [
        { id: 1, name: "Alice", email: "alice@example.com", isActive: true },
        { id: 2, name: "Bob", email: "bob@example.com", isActive: false },
        { id: 3, name: "Charlie", email: "charlie@example.com", isActive: true },
        { id: 4, name: "Diana", email: "diana@example.com", isActive: true }
    ];
    
    // Iterate over users
    const activeUsers: User[] = [];
    for (const user of users) {
        if (user.isActive) {
            activeUsers.push(user);
        }
    }
    
    console.log("Active users:", activeUsers);
    
    // Extract emails
    const emails: string[] = [];
    for (const user of users) {
        emails.push(user.email);
    }
    
    console.log("Emails:", emails);
    
    // Count active users
    let activeCount = 0;
    for (const user of users) {
        if (user.isActive) {
            activeCount++;
        }
    }
    
    console.log("Active user count:", activeCount);
}

// Test with object properties
function testObjectProperties() {
    const config = {
        apiUrl: "https://api.example.com",
        timeout: 5000,
        retries: 3,
        debug: true
    };
    
    const configValues: any[] = [];
    for (const value of Object.values(config)) {
        configValues.push(value);
    }
    
    console.log("Config values:", configValues);
    
    const configKeys: string[] = [];
    for (const key of Object.keys(config)) {
        configKeys.push(key);
    }
    
    console.log("Config keys:", configKeys);
    
    const configEntries: [string, any][] = [];
    for (const entry of Object.entries(config)) {
        configEntries.push(entry);
    }
    
    console.log("Config entries:", configEntries);
}

// Test with Map and Set
function testMapAndSet() {
    // Map iteration
    const userMap = new Map<number, string>();
    userMap.set(1, "Alice");
    userMap.set(2, "Bob");
    userMap.set(3, "Charlie");
    
    const mapValues: string[] = [];
    for (const value of userMap.values()) {
        mapValues.push(value);
    }
    
    console.log("Map values:", mapValues);
    
    const mapKeys: number[] = [];
    for (const key of userMap.keys()) {
        mapKeys.push(key);
    }
    
    console.log("Map keys:", mapKeys);
    
    const mapEntries: [number, string][] = [];
    for (const entry of userMap.entries()) {
        mapEntries.push(entry);
    }
    
    console.log("Map entries:", mapEntries);
    
    // Set iteration
    const numberSet = new Set<number>([1, 2, 3, 4, 5, 1, 2, 3]);
    const uniqueNumbers: number[] = [];
    
    for (const num of numberSet) {
        uniqueNumbers.push(num);
    }
    
    console.log("Unique numbers:", uniqueNumbers);
}

// Run tests
testObjectIteration();
testObjectProperties();
testMapAndSet();

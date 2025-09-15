
// Generic method with constructor constraint
class DatabaseConnection {
    connected: boolean;
    
    constructor() {
        this.connected = false;
    }
    
    connect(): void {
        this.connected = true;
    }
    
    disconnect(): void {
        this.connected = false;
    }
}

class CacheManager {
    cache: Map<string, any>;
    
    constructor() {
        this.cache = new Map();
    }
    
    set(key: string, value: any): void {
        this.cache.set(key, value);
    }
    
    get(key: string): any {
        return this.cache.get(key);
    }
}

// Generic factory method with constructor constraint
function createInstance<T extends new () => any>(constructor: T): InstanceType<T> {
    return new constructor();
}

// Generic method with constructor constraint
function initializeService<T extends new () => { connect(): void }>(ServiceClass: T): InstanceType<T> {
    const instance = new ServiceClass();
    instance.connect();
    return instance;
}

// Test the constructor constraints
const dbConnection = createInstance(DatabaseConnection);
const cacheManager = createInstance(CacheManager);

const initializedDB = initializeService(DatabaseConnection);
const initializedCache = initializeService(CacheManager);

console.log("Database connection:", dbConnection);
console.log("Cache manager:", cacheManager);
console.log("Initialized DB:", initializedDB);
console.log("Initialized cache:", initializedCache);

// Modules
// Demonstrates module system concepts and patterns

// Note: This example shows module patterns that would work in a full TypeScript environment
// The TSC compiler may not fully support all module features yet

// Basic module pattern using objects
let MathUtils = {
    PI: 3.14159,
    
    add: function(a: number, b: number): number {
        return a + b;
    },
    
    multiply: function(a: number, b: number): number {
        return a * b;
    },
    
    circleArea: function(radius: number): number {
        return this.PI * radius * radius;
    }
};

// Namespace pattern
namespace Geometry {
    export interface Point {
        x: number;
        y: number;
    }
    
    export interface Circle {
        center: Point;
        radius: number;
    }
    
    export function distance(p1: Point, p2: Point): number {
        let dx = p2.x - p1.x;
        let dy = p2.y - p1.y;
        return Math.sqrt(dx * dx + dy * dy);
    }
    
    export function circleArea(circle: Circle): number {
        return Math.PI * circle.radius * circle.radius;
    }
}

// Using the namespace
let point1: Geometry.Point = { x: 0, y: 0 };
let point2: Geometry.Point = { x: 3, y: 4 };
let dist: number = Geometry.distance(point1, point2);

let circle: Geometry.Circle = {
    center: point1,
    radius: 5
};
let area: number = Geometry.circleArea(circle);

// Module pattern with private members
let UserModule = (function() {
    // Private variables
    let users: any[] = [];
    let nextId = 1;
    
    // Private functions
    function validateUser(user: any): boolean {
        return user && user.name && user.email;
    }
    
    // Public API
    return {
        addUser: function(user: any): any {
            if (!validateUser(user)) {
                throw new Error("Invalid user data");
            }
            
            let newUser = {
                id: nextId++,
                name: user.name,
                email: user.email,
                createdAt: new Date()
            };
            
            users.push(newUser);
            return newUser;
        },
        
        getUserById: function(id: number): any {
            return users.find(user => user.id === id);
        },
        
        getAllUsers: function(): any[] {
            return [...users];
        },
        
        getUserCount: function(): number {
            return users.length;
        }
    };
})();

// Using the module
let user1 = UserModule.addUser({ name: "Alice", email: "alice@example.com" });
let user2 = UserModule.addUser({ name: "Bob", email: "bob@example.com" });
let allUsers = UserModule.getAllUsers();
let userCount = UserModule.getUserCount();

// Class-based module pattern
class DatabaseModule {
    private static instance: DatabaseModule;
    private data: Map<string, any> = new Map();
    
    private constructor() {}
    
    static getInstance(): DatabaseModule {
        if (!DatabaseModule.instance) {
            DatabaseModule.instance = new DatabaseModule();
        }
        return DatabaseModule.instance;
    }
    
    set(key: string, value: any): void {
        this.data.set(key, value);
    }
    
    get(key: string): any {
        return this.data.get(key);
    }
    
    has(key: string): boolean {
        return this.data.has(key);
    }
    
    delete(key: string): boolean {
        return this.data.delete(key);
    }
    
    clear(): void {
        this.data.clear();
    }
    
    size(): number {
        return this.data.size;
    }
}

// Using the singleton module
let db = DatabaseModule.getInstance();
db.set("user:1", { name: "Alice", age: 30 });
db.set("user:2", { name: "Bob", age: 25 });

let user = db.get("user:1");
let hasUser = db.has("user:1");
let dbSize = db.size();

// Event emitter module pattern
let EventEmitter = (function() {
    let listeners: Map<string, Function[]> = new Map();
    
    return {
        on: function(event: string, callback: Function): void {
            if (!listeners.has(event)) {
                listeners.set(event, []);
            }
            listeners.get(event)!.push(callback);
        },
        
        emit: function(event: string, data?: any): void {
            let eventListeners = listeners.get(event);
            if (eventListeners) {
                eventListeners.forEach(callback => {
                    try {
                        callback(data);
                    } catch (error) {
                        _print("Error in event listener:", error);
                    }
                });
            }
        },
        
        off: function(event: string, callback: Function): void {
            let eventListeners = listeners.get(event);
            if (eventListeners) {
                let index = eventListeners.indexOf(callback);
                if (index >= 0) {
                    eventListeners.splice(index, 1);
                }
            }
        },
        
        removeAllListeners: function(event?: string): void {
            if (event) {
                listeners.delete(event);
            } else {
                listeners.clear();
            }
        }
    };
})();

// Using the event emitter
EventEmitter.on("user.created", function(user: any) {
    _print("User created:", user.name);
});

EventEmitter.on("user.updated", function(user: any) {
    _print("User updated:", user.name);
});

EventEmitter.emit("user.created", { name: "Alice", email: "alice@example.com" });
EventEmitter.emit("user.updated", { name: "Alice", email: "alice.new@example.com" });

// Configuration module pattern
let ConfigModule = (function() {
    let config: any = {
        database: {
            host: "localhost",
            port: 5432,
            name: "myapp"
        },
        api: {
            baseUrl: "https://api.example.com",
            timeout: 5000
        },
        features: {
            enableLogging: true,
            enableCaching: false
        }
    };
    
    return {
        get: function(key: string): any {
            return key.split('.').reduce((obj, k) => obj && obj[k], config);
        },
        
        set: function(key: string, value: any): void {
            let keys = key.split('.');
            let lastKey = keys.pop()!;
            let target = keys.reduce((obj, k) => {
                if (!obj[k]) obj[k] = {};
                return obj[k];
            }, config);
            target[lastKey] = value;
        },
        
        getAll: function(): any {
            return JSON.parse(JSON.stringify(config));
        },
        
        reset: function(): void {
            config = {
                database: {
                    host: "localhost",
                    port: 5432,
                    name: "myapp"
                },
                api: {
                    baseUrl: "https://api.example.com",
                    timeout: 5000
                },
                features: {
                    enableLogging: true,
                    enableCaching: false
                }
            };
        }
    };
})();

// Using the configuration module
let dbHost = ConfigModule.get("database.host");
let apiTimeout = ConfigModule.get("api.timeout");
let enableLogging = ConfigModule.get("features.enableLogging");

ConfigModule.set("database.host", "production.db.com");
ConfigModule.set("features.enableCaching", true);

// Utility module pattern
let StringUtils = {
    capitalize: function(str: string): string {
        return str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();
    },
    
    camelCase: function(str: string): string {
        return str.replace(/-([a-z])/g, function(g) {
            return g[1].toUpperCase();
        });
    },
    
    kebabCase: function(str: string): string {
        return str.replace(/([A-Z])/g, '-$1').toLowerCase();
    },
    
    truncate: function(str: string, length: number): string {
        if (str.length <= length) return str;
        return str.slice(0, length) + "...";
    },
    
    pad: function(str: string, length: number, char: string = " "): string {
        while (str.length < length) {
            str = char + str;
        }
        return str;
    }
};

// Using the utility module
let capitalized = StringUtils.capitalize("hello world");
let camelCased = StringUtils.camelCase("hello-world");
let kebabCased = StringUtils.kebabCase("helloWorld");
let truncated = StringUtils.truncate("This is a long string", 10);
let padded = StringUtils.pad("42", 5, "0");

// Math module pattern
let MathModule = {
    constants: {
        PI: Math.PI,
        E: Math.E,
        LN2: Math.LN2,
        LN10: Math.LN10
    },
    
    operations: {
        add: (a: number, b: number) => a + b,
        subtract: (a: number, b: number) => a - b,
        multiply: (a: number, b: number) => a * b,
        divide: (a: number, b: number) => a / b,
        power: (a: number, b: number) => Math.pow(a, b),
        sqrt: (a: number) => Math.sqrt(a),
        abs: (a: number) => Math.abs(a),
        round: (a: number) => Math.round(a),
        floor: (a: number) => Math.floor(a),
        ceil: (a: number) => Math.ceil(a)
    },
    
    statistics: {
        mean: function(numbers: number[]): number {
            return numbers.reduce((sum, num) => sum + num, 0) / numbers.length;
        },
        
        median: function(numbers: number[]): number {
            let sorted = [...numbers].sort((a, b) => a - b);
            let mid = Math.floor(sorted.length / 2);
            return sorted.length % 2 === 0 
                ? (sorted[mid - 1] + sorted[mid]) / 2 
                : sorted[mid];
        },
        
        mode: function(numbers: number[]): number {
            let frequency: Map<number, number> = new Map();
            numbers.forEach(num => {
                frequency.set(num, (frequency.get(num) || 0) + 1);
            });
            
            let maxFreq = 0;
            let mode = numbers[0];
            frequency.forEach((freq, num) => {
                if (freq > maxFreq) {
                    maxFreq = freq;
                    mode = num;
                }
            });
            
            return mode;
        }
    }
};

// Using the math module
let pi = MathModule.constants.PI;
let sum = MathModule.operations.add(5, 3);
let mean = MathModule.statistics.mean([1, 2, 3, 4, 5]);
let median = MathModule.statistics.median([1, 2, 3, 4, 5]);
let mode = MathModule.statistics.mode([1, 2, 2, 3, 3, 3, 4]);

// This example demonstrates comprehensive module patterns
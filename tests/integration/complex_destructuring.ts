
// Complex destructuring testing
interface User {
    id: number;
    name: string;
    email: string;
    profile: {
        age: number;
        address: {
            street: string;
            city: string;
            country: string;
        };
        preferences: {
            theme: string;
            notifications: boolean;
        };
    };
    orders: Array<{
        id: number;
        items: string[];
        total: number;
    }>;
}

function testComplexDestructuring() {
    const user: User = {
        id: 1,
        name: "Alice",
        email: "alice@example.com",
        profile: {
            age: 30,
            address: {
                street: "123 Main St",
                city: "New York",
                country: "USA"
            },
            preferences: {
                theme: "dark",
                notifications: true
            }
        },
        orders: [
            { id: 1, items: ["laptop", "mouse"], total: 1099.99 },
            { id: 2, items: ["keyboard"], total: 79.99 }
        ]
    };
    
    // Complex nested destructuring
    const {
        id,
        name,
        profile: {
            age,
            address: { city, country },
            preferences: { theme }
        },
        orders: [firstOrder, ...otherOrders]
    } = user;
    
    console.log("ID:", id);
    console.log("Name:", name);
    console.log("Age:", age);
    console.log("City:", city);
    console.log("Country:", country);
    console.log("Theme:", theme);
    console.log("First order:", firstOrder);
    console.log("Other orders:", otherOrders);
}

// Test with function parameters
function testComplexFunctionParameters() {
    function processUser({
        id,
        name,
        profile: {
            age,
            address: { city }
        },
        orders
    }: User) {
        console.log("Processing user:", id, name, age, city);
        console.log("Order count:", orders.length);
    }
    
    const user: User = {
        id: 2,
        name: "Bob",
        email: "bob@example.com",
        profile: {
            age: 25,
            address: {
                street: "456 Oak Ave",
                city: "Boston",
                country: "USA"
            },
            preferences: {
                theme: "light",
                notifications: false
            }
        },
        orders: [
            { id: 3, items: ["book"], total: 19.99 }
        ]
    };
    
    processUser(user);
}

// Test with array of objects destructuring
function testArrayOfObjectsDestructuring() {
    const users: User[] = [
        {
            id: 1,
            name: "Alice",
            email: "alice@example.com",
            profile: {
                age: 30,
                address: { street: "123 Main St", city: "New York", country: "USA" },
                preferences: { theme: "dark", notifications: true }
            },
            orders: []
        },
        {
            id: 2,
            name: "Bob",
            email: "bob@example.com",
            profile: {
                age: 25,
                address: { street: "456 Oak Ave", city: "Boston", country: "USA" },
                preferences: { theme: "light", notifications: false }
            },
            orders: []
        }
    ];
    
    // Destructure first user
    const [firstUser, ...otherUsers] = users;
    const { name: firstName, profile: { age: firstAge } } = firstUser;
    
    console.log("First user name:", firstName);
    console.log("First user age:", firstAge);
    console.log("Other users count:", otherUsers.length);
}

// Test with mixed destructuring
function testMixedDestructuring() {
    const data = {
        users: [
            { name: "Alice", age: 30 },
            { name: "Bob", age: 25 }
        ],
        settings: {
            theme: "dark",
            language: "en"
        },
        metadata: {
            version: "1.0.0",
            timestamp: Date.now()
        }
    };
    
    const {
        users: [firstUser, secondUser],
        settings: { theme },
        metadata: { version }
    } = data;
    
    console.log("First user:", firstUser);
    console.log("Second user:", secondUser);
    console.log("Theme:", theme);
    console.log("Version:", version);
}

// Run tests
testComplexDestructuring();
testComplexFunctionParameters();
testArrayOfObjectsDestructuring();
testMixedDestructuring();

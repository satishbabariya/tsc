
// Object literals testing
function testObjectLiterals() {
    // Basic object literal
    const person = {
        name: "Alice",
        age: 30,
        city: "New York"
    };
    
    console.log("Person:", person);
    
    // Object literal with computed properties
    const key = "dynamicKey";
    const dynamicObject = {
        [key]: "dynamic value",
        [`computed_${key}`]: "computed value"
    };
    
    console.log("Dynamic object:", dynamicObject);
    
    // Object literal with method shorthand
    const calculator = {
        add(a: number, b: number): number {
            return a + b;
        },
        subtract(a: number, b: number): number {
            return a - b;
        },
        multiply(a: number, b: number): number {
            return a * b;
        }
    };
    
    console.log("Calculator add:", calculator.add(5, 3));
    console.log("Calculator subtract:", calculator.subtract(5, 3));
    console.log("Calculator multiply:", calculator.multiply(5, 3));
    
    // Object literal with getter and setter
    const user = {
        _name: "Bob",
        get name(): string {
            return this._name;
        },
        set name(value: string) {
            this._name = value;
        }
    };
    
    console.log("User name:", user.name);
    user.name = "Charlie";
    console.log("Updated user name:", user.name);
}

// Test with nested object literals
function testNestedObjectLiterals() {
    const company = {
        name: "Tech Corp",
        employees: [
            { name: "Alice", role: "Developer", salary: 80000 },
            { name: "Bob", role: "Designer", salary: 70000 },
            { name: "Charlie", role: "Manager", salary: 90000 }
        ],
        departments: {
            engineering: {
                head: "Alice",
                size: 5
            },
            design: {
                head: "Bob",
                size: 3
            }
        }
    };
    
    console.log("Company:", company);
    console.log("Employees:", company.employees);
    console.log("Departments:", company.departments);
}

// Test with object literal types
interface User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
}

function testObjectLiteralTypes() {
    const users: User[] = [
        { id: 1, name: "Alice", email: "alice@example.com", isActive: true },
        { id: 2, name: "Bob", email: "bob@example.com", isActive: false },
        { id: 3, name: "Charlie", email: "charlie@example.com", isActive: true }
    ];
    
    console.log("Users:", users);
    
    // Create new user object
    const newUser: User = {
        id: 4,
        name: "Diana",
        email: "diana@example.com",
        isActive: true
    };
    
    console.log("New user:", newUser);
}

// Run tests
testObjectLiterals();
testNestedObjectLiterals();
testObjectLiteralTypes();

// Objects
// Demonstrates object literals, property access, and nested objects

// Basic object literal
let person: object = {
    name: "Alice",
    age: 30,
    isActive: true
};

// Object with explicit property types
let user: {
    id: number;
    username: string;
    email: string;
    isVerified: boolean;
} = {
    id: 1,
    username: "alice123",
    email: "alice@example.com",
    isVerified: true
};

// Property access
let userName: string = user.username;        // "alice123"
let userAge: number = person.age;            // 30

// Property modification
user.email = "alice.new@example.com";
person.isActive = false;

// Computed property access
let propertyName: string = "username";
let dynamicValue: string = user[propertyName];

// Nested objects
let company: object = {
    name: "Tech Corp",
    address: {
        street: "123 Main St",
        city: "New York",
        state: "NY",
        zipCode: "10001"
    },
    employees: [
        {name: "Alice", role: "Developer"},
        {name: "Bob", role: "Designer"}
    ]
};

// Accessing nested properties
let companyName: string = company.name;
let street: string = company.address.street;
let city: string = company.address.city;

// Object with methods
let calculator: object = {
    value: 0,
    add: function (n: number): void {
        this.value += n;
    },
    multiply: function (n: number): void {
        this.value *= n;
    },
    getValue: function (): number {
        return this.value;
    }
};

// Method calls
calculator.add(5);
calculator.multiply(2);
let result: number = calculator.getValue();

// Object with different property types
let mixedObject: object = {
    stringProp: "Hello",
    numberProp: 42,
    booleanProp: true,
    arrayProp: [1, 2, 3],
    objectProp: {nested: "value"},
    nullProp: null,
    undefinedProp: undefined
};

// Object destructuring (manual property extraction)
let name: string = person.name;
let age: number = person.age;
let isActive: boolean = person.isActive;

// Object with optional properties
let config: object = {
    host: "localhost",
    port: 3000,
    timeout: 5000
    // debug is optional
};

// Adding properties dynamically
let dynamicObject: object = {};
dynamicObject.newProperty = "New Value";
dynamicObject.anotherProperty = 123;

// Object iteration
for (let key in person) {
    let value: any = person[key];
    // Process each property
}

// Object with getter and setter (if supported)
let bankAccount: object = {
    _balance: 0,
    get balance(): number {
        return this._balance;
    },
    set balance(amount: number) {
        if (amount >= 0) {
            this._balance = amount;
        }
    }
};

// Using getter and setter
bankAccount.balance = 1000;
let currentBalance: number = bankAccount.balance;

// Object with computed property names
let prop1: string = "firstName";
let prop2: string = "lastName";

let person2: object = {
    [prop1]: "John",
    [prop2]: "Doe",
    ["full" + "Name"]: "John Doe"
};

// Object with spread operator (if supported)
let baseConfig: object = {host: "localhost", port: 3000};
let extendedConfig: object = {...baseConfig, timeout: 5000, debug: true};

// Object comparison
function areEqual(obj1: object, obj2: object): boolean {
    // Simple property-by-property comparison
    for (let key in obj1) {
        if (obj1[key] !== obj2[key]) {
            return false;
        }
    }
    for (let key in obj2) {
        if (!(key in obj1)) {
            return false;
        }
    }
    return true;
}

let obj1: object = {a: 1, b: 2};
let obj2: object = {a: 1, b: 2};
let obj3: object = {a: 1, b: 3};

let equal1: boolean = areEqual(obj1, obj2);  // true
let equal2: boolean = areEqual(obj1, obj3);  // false

// Object cloning (shallow copy)
function clone(obj: object): object {
    let copy: object = {};
    for (let key in obj) {
        copy[key] = obj[key];
    }
    return copy;
}

let original: object = {x: 1, y: 2};
let copy: object = clone(original);

// Object with function properties
let mathUtils: object = {
    PI: 3.14159,
    add: function (a: number, b: number): number {
        return a + b;
    },
    multiply: function (a: number, b: number): number {
        return a * b;
    },
    circleArea: function (radius: number): number {
        return this.PI * radius * radius;
    }
};

let sum: number = mathUtils.add(5, 3);
let product: number = mathUtils.multiply(4, 7);
let area: number = mathUtils.circleArea(5);

// This example demonstrates comprehensive object functionality
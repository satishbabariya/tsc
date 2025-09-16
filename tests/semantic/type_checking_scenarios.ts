// Type checking scenarios for for...of loops
interface Product {
    id: number;
    name: string;
    price: number;
    category: string;
}

interface Order {
    id: number;
    userId: number;
    products: Product[];
    total: number;
}

function testTypeChecking() {
    const products: Product[] = [
        {id: 1, name: "Laptop", price: 999.99, category: "Electronics"},
        {id: 2, name: "Mouse", price: 29.99, category: "Electronics"},
        {id: 3, name: "Book", price: 19.99, category: "Books"}
    ];

    // Type-safe iteration
    const productNames: string[] = [];
    for (const product of products) {
        productNames.push(product.name);
    }

    console.log("Product names:", productNames);

    // Type-safe filtering
    const expensiveProducts: Product[] = [];
    for (const product of products) {
        if (product.price > 50) {
            expensiveProducts.push(product);
        }
    }

    console.log("Expensive products:", expensiveProducts);

    // Type-safe transformation
    const productPrices: number[] = [];
    for (const product of products) {
        productPrices.push(product.price);
    }

    console.log("Product prices:", productPrices);
}

// Test with generic types
function testGenericTypes<T>(items: T[]): T[] {
    const results: T[] = [];

    for (const item of items) {
        results.push(item);
    }

    return results;
}

// Test with union types
function testUnionTypes() {
    const mixedArray: (string | number | boolean)[] = [
        "hello", 42, true, "world", 100, false
    ];

    const strings: string[] = [];
    const numbers: number[] = [];
    const booleans: boolean[] = [];

    for (const item of mixedArray) {
        if (typeof item === "string") {
            strings.push(item);
        } else if (typeof item === "number") {
            numbers.push(item);
        } else if (typeof item === "boolean") {
            booleans.push(item);
        }
    }

    console.log("Strings:", strings);
    console.log("Numbers:", numbers);
    console.log("Booleans:", booleans);
}

// Test with optional properties
interface User {
    id: number;
    name: string;
    email?: string;
    age?: number;
}

function testOptionalProperties() {
    const users: User[] = [
        {id: 1, name: "Alice", email: "alice@example.com", age: 30},
        {id: 2, name: "Bob", age: 25},
        {id: 3, name: "Charlie", email: "charlie@example.com"}
    ];

    const emails: string[] = [];
    const ages: number[] = [];

    for (const user of users) {
        if (user.email) {
            emails.push(user.email);
        }
        if (user.age) {
            ages.push(user.age);
        }
    }

    console.log("Emails:", emails);
    console.log("Ages:", ages);
}

// Test with nested objects
interface Company {
    name: string;
    employees: User[];
    departments: string[];
}

function testNestedObjects() {
    const companies: Company[] = [
        {
            name: "Tech Corp",
            employees: [
                {id: 1, name: "Alice", email: "alice@techcorp.com", age: 30},
                {id: 2, name: "Bob", email: "bob@techcorp.com", age: 25}
            ],
            departments: ["Engineering", "Marketing", "Sales"]
        },
        {
            name: "Startup Inc",
            employees: [
                {id: 3, name: "Charlie", email: "charlie@startup.com", age: 28}
            ],
            departments: ["Engineering", "Design"]
        }
    ];

    const allEmployees: User[] = [];
    const allDepartments: string[] = [];

    for (const company of companies) {
        for (const employee of company.employees) {
            allEmployees.push(employee);
        }
        for (const department of company.departments) {
            allDepartments.push(department);
        }
    }

    console.log("All employees:", allEmployees);
    console.log("All departments:", allDepartments);
}

// Run tests
testTypeChecking();
testUnionTypes();
testOptionalProperties();
testNestedObjects();

// Test generic function
const stringArray = testGenericTypes(["hello", "world", "typescript"]);
const numberArray = testGenericTypes([1, 2, 3, 4, 5]);
const booleanArray = testGenericTypes([true, false, true]);

console.log("Generic string array:", stringArray);
console.log("Generic number array:", numberArray);
console.log("Generic boolean array:", booleanArray);

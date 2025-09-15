
// Complex type scenarios with mismatches
interface User {
    id: number;
    name: string;
    email: string;
    active: boolean;
}

interface Product {
    id: number;
    name: string;
    price: number;
    inStock: boolean;
}

function processUsers(users: User[]): User[] {
    return users.filter(user => user.active);
}

function processProducts(products: Product[]): Product[] {
    return products.filter(product => product.inStock);
}

function testComplexTypeMismatches(): void {
    let users: User[] = [
        { id: 1, name: "John", email: "john@example.com", active: true },
        { id: 2, name: "Jane", email: "jane@example.com", active: false }
    ];
    
    let products: Product[] = [
        { id: 1, name: "Laptop", price: 999, inStock: true },
        { id: 2, name: "Mouse", price: 25, inStock: false }
    ];
    
    // Type mismatches with complex objects
    let result1 = processUsers(products); // Product[] passed to User[] parameter
    let result2 = processProducts(users); // User[] passed to Product[] parameter
    
    // Mixed arrays
    let mixedArray = [...users, ...products];
    let result3 = processUsers(mixedArray); // mixed array passed to User[] parameter
    let result4 = processProducts(mixedArray); // mixed array passed to Product[] parameter
}

// Generic functions with type mismatches
function genericProcess<T>(items: T[], processor: (item: T) => T): T[] {
    return items.map(processor);
}

function testGenericTypeMismatches(): void {
    let numbers = [1, 2, 3];
    let strings = ["a", "b", "c"];
    
    // Type mismatches in generic functions
    let result1 = genericProcess(numbers, (n: string) => n.toUpperCase()); // number[] with string processor
    let result2 = genericProcess(strings, (s: number) => s * 2); // string[] with number processor
    let result3 = genericProcess(numbers, (n: boolean) => !n); // number[] with boolean processor
}

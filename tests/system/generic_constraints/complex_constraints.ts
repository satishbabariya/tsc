// Complex Generic Type Constraint System Tests
// Tests advanced constraint scenarios with interfaces, inheritance, and nested generics

// Base interfaces for testing
interface Identifiable {
    id: number;
}

interface Named {
    name: string;
}

interface Timestamped {
    timestamp: number;
}

// Complex interface combining multiple interfaces
interface User extends Identifiable, Named, Timestamped {
    email: string;
    active: boolean;
}

// Test 1: Interface constraints
function processIdentifiable<T extends Identifiable>(item: T): number {
    return item.id;
}

function processNamed<T extends Named>(item: T): string {
    return item.name;
}

// Test 2: Multiple interface constraints
function processUser<T extends Identifiable & Named>(user: T): string {
    return `${user.name} (ID: ${user.id})`;
}

// Test 3: Complex inheritance constraints
function processTimestamped<T extends Timestamped>(item: T): number {
    return item.timestamp;
}

// Test 4: Generic class with constraints
class GenericContainer<T extends Identifiable> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    findById(id: number): T | undefined {
        return this.items.find(item => item.id === id);
    }
    
    getAll(): T[] {
        return [...this.items];
    }
}

// Test 5: Generic method in class
class DataProcessor {
    process<T extends Named>(item: T): string {
        return `Processing: ${item.name}`;
    }
    
    validate<T extends Identifiable>(item: T): boolean {
        return item.id > 0;
    }
}

// Test 6: Function with generic return type
function createWrapper<T extends Identifiable>(item: T): { data: T; processed: boolean } {
    return {
        data: item,
        processed: true
    };
}

// Test 7: Nested generic constraints
function processNested<T extends Identifiable, U extends Named>(item1: T, item2: U): string {
    return `${item1.id}: ${item2.name}`;
}

// Test data
const testUser: User = {
    id: 1,
    name: "John Doe",
    email: "john@example.com",
    timestamp: 1234567890,
    active: true
};

const testItem: Identifiable = {
    id: 42
};

const testNamed: Named = {
    name: "Test Item"
};

// Test functions
function testComplexConstraints(): void {
    console.log("=== Complex Constraint Tests ===");
    
    // Interface constraint tests
    let idResult = processIdentifiable<User>(testUser);
    console.log(`ID result: ${idResult}`);
    
    let nameResult = processNamed<User>(testUser);
    console.log(`Name result: ${nameResult}`);
    
    // Multiple interface constraints
    let userResult = processUser<User>(testUser);
    console.log(`User result: ${userResult}`);
    
    // Timestamp constraint
    let timestampResult = processTimestamped<User>(testUser);
    console.log(`Timestamp result: ${timestampResult}`);
    
    // Generic class tests
    let container = new GenericContainer<User>();
    container.add(testUser);
    let foundUser = container.findById(1);
    console.log(`Found user: ${foundUser ? foundUser.name : "Not found"}`);
    
    // Generic method tests
    let processor = new DataProcessor();
    let processResult = processor.process<User>(testUser);
    console.log(`Process result: ${processResult}`);
    
    let validateResult = processor.validate<User>(testUser);
    console.log(`Validate result: ${validateResult}`);
    
    // Wrapper function test
    let wrapper = createWrapper<User>(testUser);
    console.log(`Wrapper result: ${wrapper.data.name}, processed: ${wrapper.processed}`);
    
    // Nested constraints
    let nestedResult = processNested<User, Named>(testUser, testNamed);
    console.log(`Nested result: ${nestedResult}`);
}

// Test constraint violations (should fail compilation)
function testComplexConstraintViolations(): void {
    console.log("=== Complex Constraint Violation Tests (Expected Failures) ===");
    
    // These should cause compilation errors:
    // let invalidId = processIdentifiable<string>("test");           // string doesn't extend Identifiable
    // let invalidName = processNamed<number>(123);                  // number doesn't extend Named
    // let invalidUser = processUser<Identifiable>(testItem);       // Identifiable doesn't extend Named
    // let invalidContainer = new GenericContainer<string>();         // string doesn't extend Identifiable
    // let invalidProcess = processor.process<number>(123);         // number doesn't extend Named
    
    console.log("Complex constraint violation tests are commented out as they should fail compilation");
}

function main(): void {
    testComplexConstraints();
    testComplexConstraintViolations();
}

main();
// Interface Generic Type Constraints Regression Tests
// These tests ensure that interface-based generic constraints continue to work
// and prevent regressions in complex constraint scenarios

// Base interfaces for testing (CRITICAL - must not regress)
interface Identifiable {
    id: number;
}

interface Named {
    name: string;
}

interface Timestamped {
    timestamp: number;
}

// Complex interface combining multiple interfaces (CRITICAL - must not regress)
interface User extends Identifiable, Named, Timestamped {
    email: string;
    active: boolean;
}

// Test 1: Single interface constraints (CRITICAL - must not regress)
function processIdentifiable<T extends Identifiable>(item: T): number {
    return item.id;
}

function processNamed<T extends Named>(item: T): string {
    return item.name;
}

function processTimestamped<T extends Timestamped>(item: T): number {
    return item.timestamp;
}

// Test 2: Multiple interface constraints (CRITICAL - must not regress)
function processUser<T extends Identifiable & Named>(user: T): string {
    return user.name + " (ID: " + user.id.toString() + ")";
}

// Test 3: Generic class with interface constraints (CRITICAL - must not regress)
class GenericContainer<T extends Identifiable> {
    private item: T;
    
    constructor(item: T) {
        this.item = item;
    }
    
    setItem(item: T): void {
        this.item = item;
    }
    
    getItem(): T {
        return this.item;
    }
    
    getId(): number {
        return this.item.id;
    }
}

// Test 4: Generic method in class (CRITICAL - must not regress)
class DataProcessor {
    process<T extends Named>(item: T): string {
        return "Processing: " + item.name;
    }
    
    validate<T extends Identifiable>(item: T): boolean {
        return item.id > 0;
    }
}

// Test 5: Function with generic return type (CRITICAL - must not regress)
function createWrapper<T extends Identifiable>(item: T): { data: T; processed: boolean } {
    return {
        data: item,
        processed: true
    };
}

// Test 6: Nested generic constraints (CRITICAL - must not regress)
function processNested<T extends Identifiable, U extends Named>(item1: T, item2: U): string {
    return item1.id.toString() + ": " + item2.name;
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

// Test functions that validate interface constraint functionality
function testInterfaceConstraintsRegression(): void {
    // These tests MUST pass - they represent core interface constraint functionality
    
    // Single interface constraint tests
    let idResult = processIdentifiable<User>(testUser);
    let nameResult = processNamed<User>(testUser);
    let timestampResult = processTimestamped<User>(testUser);
    
    // Multiple interface constraint tests
    let userResult = processUser<User>(testUser);
    
    // Generic class tests
    let container = new GenericContainer<User>(testUser);
    let retrievedItem = container.getItem();
    let containerId = container.getId();
    
    // Generic method tests
    let processor = new DataProcessor();
    let processResult = processor.process<User>(testUser);
    let validateResult = processor.validate<User>(testUser);
    
    // Wrapper function test
    let wrapper = createWrapper<User>(testUser);
    let wrapperData = wrapper.data;
    let wrapperProcessed = wrapper.processed;
    
    // Nested constraints
    let nestedResult = processNested<User, Named>(testUser, testNamed);
    
    // All tests completed successfully
    let allTestsPassed = true;
}

function main(): void {
    testInterfaceConstraintsRegression();
}

main();
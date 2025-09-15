#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// Destructuring and Object Features Testing Suite
class DestructuringTestingSuite {
public:
    void runComprehensiveTesting() {
        std::cout << "=== Destructuring and Object Features Testing Suite ===" << std::endl;
        
        // Test 1: Array Destructuring
        testArrayDestructuring();
        
        // Test 2: Object Destructuring
        testObjectDestructuring();
        
        // Test 3: Object Literals
        testObjectLiterals();
        
        // Test 4: Object Property Access
        testObjectPropertyAccess();
        
        // Test 5: Object Method Calls
        testObjectMethodCalls();
        
        // Test 6: Complex Destructuring
        testComplexDestructuring();
        
        // Test 7: Error Scenarios
        testErrorScenarios();
        
        std::cout << "=== Destructuring and Object Features Testing Complete ===" << std::endl;
    }
    
private:
    void testArrayDestructuring() {
        std::cout << "\n--- Testing Array Destructuring ---" << std::endl;
        
        std::string arrayTest = R"(
// Array destructuring testing
function testArrayDestructuring() {
    // Basic array destructuring
    const numbers = [1, 2, 3, 4, 5];
    const [first, second, third] = numbers;
    
    console.log("First:", first);
    console.log("Second:", second);
    console.log("Third:", third);
    
    // Array destructuring with rest
    const [a, b, ...rest] = numbers;
    console.log("A:", a);
    console.log("B:", b);
    console.log("Rest:", rest);
    
    // Array destructuring with default values
    const [x = 0, y = 0, z = 0] = [1, 2];
    console.log("X:", x);
    console.log("Y:", y);
    console.log("Z:", z);
    
    // Array destructuring with skipped elements
    const [, , thirdElement] = numbers;
    console.log("Third element:", thirdElement);
    
    // Array destructuring in function parameters
    function processArray([first, second, ...rest]: number[]) {
        console.log("First:", first);
        console.log("Second:", second);
        console.log("Rest:", rest);
    }
    
    processArray([10, 20, 30, 40, 50]);
}

// Test with different data types
function testArrayDestructuringTypes() {
    const mixedArray = ["hello", 42, true, { name: "test" }];
    const [str, num, bool, obj] = mixedArray;
    
    console.log("String:", str);
    console.log("Number:", num);
    console.log("Boolean:", bool);
    console.log("Object:", obj);
}

// Test with nested arrays
function testNestedArrayDestructuring() {
    const nestedArray = [[1, 2], [3, 4], [5, 6]];
    const [[a, b], [c, d], [e, f]] = nestedArray;
    
    console.log("A:", a, "B:", b);
    console.log("C:", c, "D:", d);
    console.log("E:", e, "F:", f);
}

// Run tests
testArrayDestructuring();
testArrayDestructuringTypes();
testNestedArrayDestructuring();
)";
        
        saveTestFile("array_destructuring.ts", arrayTest);
        std::cout << "Generated array destructuring test" << std::endl;
    }
    
    void testObjectDestructuring() {
        std::cout << "\n--- Testing Object Destructuring ---" << std::endl;
        
        std::string objectTest = R"(
// Object destructuring testing
interface Person {
    name: string;
    age: number;
    city: string;
    country: string;
}

function testObjectDestructuring() {
    const person: Person = {
        name: "Alice",
        age: 30,
        city: "New York",
        country: "USA"
    };
    
    // Basic object destructuring
    const { name, age } = person;
    console.log("Name:", name);
    console.log("Age:", age);
    
    // Object destructuring with renaming
    const { name: personName, age: personAge } = person;
    console.log("Person name:", personName);
    console.log("Person age:", personAge);
    
    // Object destructuring with default values
    const { name: userName = "Unknown", age: userAge = 0, email = "no-email" } = person;
    console.log("User name:", userName);
    console.log("User age:", userAge);
    console.log("Email:", email);
    
    // Object destructuring with rest
    const { name: fullName, ...otherProps } = person;
    console.log("Full name:", fullName);
    console.log("Other props:", otherProps);
}

// Test with nested objects
function testNestedObjectDestructuring() {
    const user = {
        id: 1,
        profile: {
            name: "Bob",
            age: 25,
            address: {
                street: "123 Main St",
                city: "Boston",
                state: "MA"
            }
        },
        settings: {
            theme: "dark",
            notifications: true
        }
    };
    
    const { 
        id, 
        profile: { 
            name, 
            address: { city, state } 
        },
        settings: { theme }
    } = user;
    
    console.log("ID:", id);
    console.log("Name:", name);
    console.log("City:", city);
    console.log("State:", state);
    console.log("Theme:", theme);
}

// Test with function parameters
function testObjectDestructuringParameters() {
    function processUser({ name, age, city }: Person) {
        console.log("Processing user:", name, age, city);
    }
    
    function processUserWithDefaults({ name = "Unknown", age = 0, city = "Unknown" }: Partial<Person>) {
        console.log("Processing user with defaults:", name, age, city);
    }
    
    const user: Person = {
        name: "Charlie",
        age: 35,
        city: "San Francisco",
        country: "USA"
    };
    
    processUser(user);
    processUserWithDefaults({});
}

// Run tests
testObjectDestructuring();
testNestedObjectDestructuring();
testObjectDestructuringParameters();
)";
        
        saveTestFile("object_destructuring.ts", objectTest);
        std::cout << "Generated object destructuring test" << std::endl;
    }
    
    void testObjectLiterals() {
        std::cout << "\n--- Testing Object Literals ---" << std::endl;
        
        std::string objectLiteralTest = R"(
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
)";
        
        saveTestFile("object_literals.ts", objectLiteralTest);
        std::cout << "Generated object literals test" << std::endl;
    }
    
    void testObjectPropertyAccess() {
        std::cout << "\n--- Testing Object Property Access ---" << std::endl;
        
        std::string propertyAccessTest = R"(
// Object property access testing
interface Product {
    id: number;
    name: string;
    price: number;
    category: string;
    inStock: boolean;
}

function testObjectPropertyAccess() {
    const product: Product = {
        id: 1,
        name: "Laptop",
        price: 999.99,
        category: "Electronics",
        inStock: true
    };
    
    // Dot notation access
    console.log("Product name:", product.name);
    console.log("Product price:", product.price);
    console.log("Product category:", product.category);
    
    // Bracket notation access
    console.log("Product name (bracket):", product["name"]);
    console.log("Product price (bracket):", product["price"]);
    console.log("Product category (bracket):", product["category"]);
    
    // Dynamic property access
    const propertyName = "name";
    console.log("Dynamic property access:", product[propertyName]);
    
    // Computed property access
    const dynamicKey = "price";
    console.log("Computed property access:", product[`${dynamicKey}`]);
}

// Test with nested property access
function testNestedPropertyAccess() {
    const user = {
        id: 1,
        profile: {
            personal: {
                name: "Alice",
                age: 30
            },
            contact: {
                email: "alice@example.com",
                phone: "555-1234"
            }
        },
        settings: {
            theme: "dark",
            notifications: {
                email: true,
                push: false
            }
        }
    };
    
    // Nested dot notation
    console.log("User name:", user.profile.personal.name);
    console.log("User email:", user.profile.contact.email);
    console.log("User theme:", user.settings.theme);
    
    // Nested bracket notation
    console.log("User age:", user["profile"]["personal"]["age"]);
    console.log("User phone:", user["profile"]["contact"]["phone"]);
    
    // Mixed notation
    console.log("Email notifications:", user.settings["notifications"]["email"]);
    console.log("Push notifications:", user["settings"].notifications.push);
}

// Test with array-like objects
function testArrayLikePropertyAccess() {
    const arrayLike = {
        0: "first",
        1: "second",
        2: "third",
        length: 3
    };
    
    // Access like array
    console.log("First element:", arrayLike[0]);
    console.log("Second element:", arrayLike[1]);
    console.log("Third element:", arrayLike[2]);
    console.log("Length:", arrayLike.length);
    
    // Iterate like array
    for (let i = 0; i < arrayLike.length; i++) {
        console.log(`Element ${i}:`, arrayLike[i]);
    }
}

// Test with optional chaining simulation
function testOptionalPropertyAccess() {
    const user = {
        profile: {
            name: "Alice",
            address: {
                street: "123 Main St",
                city: "New York"
            }
        }
    };
    
    // Safe property access
    const userName = user.profile?.name;
    const userStreet = user.profile?.address?.street;
    const userCountry = user.profile?.address?.country;
    
    console.log("User name:", userName);
    console.log("User street:", userStreet);
    console.log("User country:", userCountry);
}

// Run tests
testObjectPropertyAccess();
testNestedPropertyAccess();
testArrayLikePropertyAccess();
testOptionalPropertyAccess();
)";
        
        saveTestFile("object_property_access.ts", propertyAccessTest);
        std::cout << "Generated object property access test" << std::endl;
    }
    
    void testObjectMethodCalls() {
        std::cout << "\n--- Testing Object Method Calls ---" << std::endl;
        
        std::string methodCallTest = R"(
// Object method calls testing
class Calculator {
    private value: number;
    
    constructor(initialValue: number = 0) {
        this.value = initialValue;
    }
    
    add(num: number): Calculator {
        this.value += num;
        return this;
    }
    
    subtract(num: number): Calculator {
        this.value -= num;
        return this;
    }
    
    multiply(num: number): Calculator {
        this.value *= num;
        return this;
    }
    
    divide(num: number): Calculator {
        if (num !== 0) {
            this.value /= num;
        }
        return this;
    }
    
    getValue(): number {
        return this.value;
    }
    
    reset(): Calculator {
        this.value = 0;
        return this;
    }
}

function testObjectMethodCalls() {
    const calc = new Calculator(10);
    
    // Method chaining
    const result = calc
        .add(5)
        .multiply(2)
        .subtract(3)
        .divide(2)
        .getValue();
    
    console.log("Calculation result:", result);
    
    // Individual method calls
    calc.reset();
    calc.add(10);
    console.log("After add:", calc.getValue());
    
    calc.multiply(3);
    console.log("After multiply:", calc.getValue());
    
    calc.subtract(5);
    console.log("After subtract:", calc.getValue());
}

// Test with object literal methods
function testObjectLiteralMethods() {
    const mathUtils = {
        add(a: number, b: number): number {
            return a + b;
        },
        
        subtract(a: number, b: number): number {
            return a - b;
        },
        
        multiply(a: number, b: number): number {
            return a * b;
        },
        
        divide(a: number, b: number): number {
            return b !== 0 ? a / b : 0;
        },
        
        power(base: number, exponent: number): number {
            return Math.pow(base, exponent);
        },
        
        sqrt(num: number): number {
            return Math.sqrt(num);
        }
    };
    
    console.log("Add:", mathUtils.add(5, 3));
    console.log("Subtract:", mathUtils.subtract(5, 3));
    console.log("Multiply:", mathUtils.multiply(5, 3));
    console.log("Divide:", mathUtils.divide(15, 3));
    console.log("Power:", mathUtils.power(2, 3));
    console.log("Square root:", mathUtils.sqrt(16));
}

// Test with this binding
function testThisBinding() {
    const person = {
        name: "Alice",
        age: 30,
        
        greet(): string {
            return `Hello, I'm ${this.name} and I'm ${this.age} years old`;
        },
        
        celebrateBirthday(): void {
            this.age++;
        },
        
        introduce(): string {
            return this.greet();
        }
    };
    
    console.log("Greeting:", person.greet());
    console.log("Introduction:", person.introduce());
    
    person.celebrateBirthday();
    console.log("After birthday:", person.greet());
}

// Test with method borrowing
function testMethodBorrowing() {
    const obj1 = {
        name: "Object 1",
        getName(): string {
            return this.name;
        }
    };
    
    const obj2 = {
        name: "Object 2"
    };
    
    // Borrow method from obj1
    const getName = obj1.getName;
    const boundGetName = getName.bind(obj2);
    
    console.log("Obj1 name:", obj1.getName());
    console.log("Obj2 name (borrowed):", boundGetName());
}

// Test with arrow functions and this
function testArrowFunctionThis() {
    const counter = {
        count: 0,
        
        // Regular method
        increment(): void {
            this.count++;
        },
        
        // Arrow function method (preserves this from outer scope)
        incrementArrow: (): void => {
            // this.count++; // This would cause an error
        },
        
        // Method that returns arrow function
        getIncrementer() {
            return () => {
                this.count++;
            };
        },
        
        getCount(): number {
            return this.count;
        }
    };
    
    counter.increment();
    console.log("Count after increment:", counter.getCount());
    
    const incrementer = counter.getIncrementer();
    incrementer();
    console.log("Count after arrow incrementer:", counter.getCount());
}

// Run tests
testObjectMethodCalls();
testObjectLiteralMethods();
testThisBinding();
testMethodBorrowing();
testArrowFunctionThis();
)";
        
        saveTestFile("object_method_calls.ts", methodCallTest);
        std::cout << "Generated object method calls test" << std::endl;
    }
    
    void testComplexDestructuring() {
        std::cout << "\n--- Testing Complex Destructuring ---" << std::endl;
        
        std::string complexTest = R"(
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
)";
        
        saveTestFile("complex_destructuring.ts", complexTest);
        std::cout << "Generated complex destructuring test" << std::endl;
    }
    
    void testErrorScenarios() {
        std::cout << "\n--- Testing Error Scenarios ---" << std::endl;
        
        std::string errorTest = R"(
// Error scenarios testing
function testErrorScenarios() {
    // Test with null/undefined
    const nullValue = null;
    const undefinedValue = undefined;
    
    // These should cause errors
    // const { prop } = nullValue; // Error: Cannot destructure property 'prop' of 'null'
    // const { prop } = undefinedValue; // Error: Cannot destructure property 'prop' of 'undefined'
    
    // Safe destructuring with defaults
    const { prop = "default" } = nullValue || {};
    console.log("Safe destructuring:", prop);
    
    // Test with non-object types
    const stringValue = "hello";
    const numberValue = 42;
    
    // These should cause errors
    // const { prop } = stringValue; // Error: Cannot destructure property 'prop' of 'string'
    // const { prop } = numberValue; // Error: Cannot destructure property 'prop' of 'number'
    
    // Test with missing properties
    const incompleteObject = { name: "Alice" };
    
    // Destructuring non-existent properties
    const { name, age = 0, city = "Unknown" } = incompleteObject;
    console.log("Name:", name);
    console.log("Age (default):", age);
    console.log("City (default):", city);
}

// Test with type mismatches
function testTypeMismatches() {
    const user = {
        name: "Alice",
        age: 30,
        isActive: true
    };
    
    // These should cause type errors
    // const { name }: number = user; // Error: Type 'string' is not assignable to type 'number'
    // const { age }: string = user; // Error: Type 'number' is not assignable to type 'string'
    
    // Correct destructuring with types
    const { name }: { name: string } = user;
    const { age }: { age: number } = user;
    const { isActive }: { isActive: boolean } = user;
    
    console.log("Name:", name);
    console.log("Age:", age);
    console.log("Is active:", isActive);
}

// Test with invalid destructuring patterns
function testInvalidPatterns() {
    const array = [1, 2, 3, 4, 5];
    
    // These should cause errors
    // const { prop } = array; // Error: Cannot destructure property 'prop' of 'number[]'
    // const [a, b, c] = { prop: "value" }; // Error: Object is not iterable
    
    // Correct patterns
    const [first, second, third] = array;
    console.log("First:", first);
    console.log("Second:", second);
    console.log("Third:", third);
    
    const obj = { prop: "value" };
    const { prop } = obj;
    console.log("Prop:", prop);
}

// Test with reserved keywords
function testReservedKeywords() {
    const data = {
        class: "test-class",
        function: "test-function",
        const: "test-const"
    };
    
    // These should cause errors
    // const { class } = data; // Error: 'class' is a reserved keyword
    // const { function } = data; // Error: 'function' is a reserved keyword
    // const { const } = data; // Error: 'const' is a reserved keyword
    
    // Correct way with renaming
    const { class: className, function: functionName, const: constName } = data;
    console.log("Class name:", className);
    console.log("Function name:", functionName);
    console.log("Const name:", constName);
}

// Test with circular references
function testCircularReferences() {
    const obj1: any = { name: "Object 1" };
    const obj2: any = { name: "Object 2" };
    
    obj1.ref = obj2;
    obj2.ref = obj1;
    
    // Destructuring should work fine
    const { name: name1, ref: { name: name2 } } = obj1;
    console.log("Name 1:", name1);
    console.log("Name 2:", name2);
}

// Run tests
testErrorScenarios();
testTypeMismatches();
testInvalidPatterns();
testReservedKeywords();
testCircularReferences();
)";
        
        saveTestFile("error_scenarios.ts", errorTest);
        std::cout << "Generated error scenarios test" << std::endl;
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }
};

int main() {
    try {
        DestructuringTestingSuite suite;
        suite.runComprehensiveTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
// Type inference accuracy testing
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
    };
}

function testTypeInferenceAccuracy() {
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
            }
        }
    };

    // Test array destructuring type inference
    const numbers: number[] = [1, 2, 3, 4, 5];
    const [first, second, ...rest] = numbers;

    // first should be inferred as number
    console.log("First type:", typeof first);
    console.log("Second type:", typeof second);
    console.log("Rest type:", typeof rest);

    // Test object destructuring type inference
    const {name, profile: {age, address: {city}}} = user;

    // name should be inferred as string
    console.log("Name type:", typeof name);
    console.log("Age type:", typeof age);
    console.log("City type:", typeof city);

    // Test with generic functions
    function processItems<T>(items: T[]): T[] {
        const [first, ...rest] = items;
        return [first, ...rest];
    }

    const stringItems = ["a", "b", "c"];
    const processedStrings = processItems(stringItems);
    console.log("Processed strings:", processedStrings);

    const numberItems = [1, 2, 3];
    const processedNumbers = processItems(numberItems);
    console.log("Processed numbers:", processedNumbers);
}

// Test with complex type inference
function testComplexTypeInference() {
    // Test with union types
    const mixedArray: (string | number)[] = ["hello", 42, "world", 100];
    const [first, second, ...rest] = mixedArray;

    console.log("Mixed first type:", typeof first);
    console.log("Mixed second type:", typeof second);
    console.log("Mixed rest type:", typeof rest);

    // Test with optional properties
    interface OptionalUser {
        name: string;
        age?: number;
        email?: string;
    }

    const optionalUser: OptionalUser = {name: "Bob"};
    const {name, age = 0, email = "no-email"} = optionalUser;

    console.log("Optional name type:", typeof name);
    console.log("Optional age type:", typeof age);
    console.log("Optional email type:", typeof email);

    // Test with array of objects
    const users: User[] = [
        {
            id: 1,
            name: "Alice",
            email: "alice@example.com",
            profile: {
                age: 30,
                address: {
                    street: "123 Main St",
                    city: "New York",
                    country: "USA"
                }
            }
        },
        {
            id: 2,
            name: "Bob",
            email: "bob@example.com",
            profile: {
                age: 25,
                address: {
                    street: "456 Oak Ave",
                    city: "Boston",
                    country: "USA"
                }
            }
        }
    ];

    const [firstUser, ...otherUsers] = users;
    const {name: firstName, profile: {age: firstAge}} = firstUser;

    console.log("First user name type:", typeof firstName);
    console.log("First user age type:", typeof firstAge);
    console.log("Other users type:", typeof otherUsers);
}

// Test with type guards
function testTypeGuards() {
    function isString(value: unknown): value is string {
        return typeof value === "string";
    }

    function isNumber(value: unknown): value is number {
        return typeof value === "number";
    }

    const mixedData: unknown[] = ["hello", 42, "world", 100];
    const [first, second, ...rest] = mixedData;

    if (isString(first)) {
        console.log("First is string:", first.toUpperCase());
    }

    if (isNumber(second)) {
        console.log("Second is number:", second * 2);
    }

    // Test with destructuring in type guards
    function processMixedData(data: unknown[]) {
        const [first, ...rest] = data;

        if (isString(first)) {
            console.log("First is string:", first.length);
        } else if (isNumber(first)) {
            console.log("First is number:", first * 2);
        }
    }

    processMixedData(mixedData);
}

// Run tests
testTypeInferenceAccuracy();
testComplexTypeInference();
testTypeGuards();

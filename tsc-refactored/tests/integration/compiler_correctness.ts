// Compiler correctness testing
function testCompilerCorrectness() {
    // Test that existing functionality still works
    const existingArray = [1, 2, 3, 4, 5];
    const existingObject = {name: "Alice", age: 30};

    // Test existing array operations
    const arrayLength = existingArray.length;
    const arrayPush = existingArray.push(6);
    const arrayPop = existingArray.pop();

    console.log("Array length:", arrayLength);
    console.log("Array push result:", arrayPush);
    console.log("Array pop result:", arrayPop);

    // Test existing object operations
    const objectKeys = Object.keys(existingObject);
    const objectValues = Object.values(existingObject);
    const objectEntries = Object.entries(existingObject);

    console.log("Object keys:", objectKeys);
    console.log("Object values:", objectValues);
    console.log("Object entries:", objectEntries);

    // Test existing function calls
    function existingFunction(a: number, b: number): number {
        return a + b;
    }

    const functionResult = existingFunction(5, 3);
    console.log("Function result:", functionResult);

    // Test existing class operations
    class ExistingClass {
        private value: number;

        constructor(value: number) {
            this.value = value;
        }

        getValue(): number {
            return this.value;
        }

        setValue(value: number): void {
            this.value = value;
        }
    }

    const existingInstance = new ExistingClass(10);
    const existingValue = existingInstance.getValue();
    existingInstance.setValue(20);
    const newValue = existingInstance.getValue();

    console.log("Existing value:", existingValue);
    console.log("New value:", newValue);
}

// Test with mixed old and new features
function testMixedFeatures() {
    // Test destructuring with existing arrays
    const existingArray = [1, 2, 3, 4, 5];
    const [first, second, ...rest] = existingArray;

    console.log("Destructured from existing array:", first, second, rest);

    // Test destructuring with existing objects
    const existingObject = {name: "Alice", age: 30, city: "New York"};
    const {name, age, ...otherProps} = existingObject;

    console.log("Destructured from existing object:", name, age, otherProps);

    // Test object literals with existing functions
    function existingFunction(a: number, b: number): number {
        return a + b;
    }

    const calculator = {
        add: existingFunction,
        subtract(a: number, b: number): number {
            return a - b;
        }
    };

    const addResult = calculator.add(5, 3);
    const subtractResult = calculator.subtract(5, 3);

    console.log("Calculator add:", addResult);
    console.log("Calculator subtract:", subtractResult);

    // Test property access with existing objects
    const existingUser = {
        id: 1,
        name: "Alice",
        profile: {
            age: 30,
            address: {
                city: "New York"
            }
        }
    };

    const userId = existingUser.id;
    const userName = existingUser.name;
    const userAge = existingUser.profile.age;
    const userCity = existingUser.profile.address.city;

    console.log("User info:", userId, userName, userAge, userCity);
}

// Test with existing TypeScript features
function testExistingTypeScriptFeatures() {
    // Test with existing interfaces
    interface ExistingInterface {
        name: string;
        age: number;
        city: string;
    }

    const existingUser: ExistingInterface = {
        name: "Alice",
        age: 30,
        city: "New York"
    };

    // Test destructuring with existing interface
    const {name, age, city} = existingUser;
    console.log("Destructured from interface:", name, age, city);

    // Test with existing generics
    function existingGeneric<T>(value: T): T {
        return value;
    }

    const genericResult = existingGeneric("hello");
    console.log("Generic result:", genericResult);

    // Test with existing union types
    type ExistingUnion = string | number | boolean;

    function processUnion(value: ExistingUnion): string {
        if (typeof value === "string") {
            return value.toUpperCase();
        } else if (typeof value === "number") {
            return value.toString();
        } else {
            return value ? "true" : "false";
        }
    }

    const unionResult1 = processUnion("hello");
    const unionResult2 = processUnion(42);
    const unionResult3 = processUnion(true);

    console.log("Union results:", unionResult1, unionResult2, unionResult3);
}

// Test with existing async/await
function testExistingAsyncFeatures() {
    // Test with existing async functions
    async function existingAsyncFunction(): Promise<string> {
        return "async result";
    }

    // Test destructuring with async results
    existingAsyncFunction().then(result => {
        console.log("Async result:", result);
    });

    // Test with existing Promise
    const existingPromise = Promise.resolve([1, 2, 3, 4, 5]);

    existingPromise.then(array => {
        const [first, second, ...rest] = array;
        console.log("Destructured from promise:", first, second, rest);
    });
}

// Run tests
testCompilerCorrectness();
testMixedFeatures();
testExistingTypeScriptFeatures();
testExistingAsyncFeatures();

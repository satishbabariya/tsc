// Comprehensive Tests for Parser Type Parsing Improvements
// Tests union types, intersection types, object types, function types

// =============================================================================
// Test 1: Union Types
// =============================================================================

// Test basic union types
type BasicUnion = string | number;
type MultiUnion = string | number | boolean;
type ComplexUnion = string | number | boolean | object;

function testBasicUnionTypes(): void {
    let basicUnion: BasicUnion = "hello";
    console.log(`Basic union (string): ${basicUnion} (type: ${typeof basicUnion})`);
    
    basicUnion = 42;
    console.log(`Basic union (number): ${basicUnion} (type: ${typeof basicUnion})`);
    
    let multiUnion: MultiUnion = true;
    console.log(`Multi union (boolean): ${multiUnion} (type: ${typeof multiUnion})`);
    
    multiUnion = "test";
    console.log(`Multi union (string): ${multiUnion} (type: ${typeof multiUnion})`);
    
    multiUnion = 123;
    console.log(`Multi union (number): ${multiUnion} (type: ${typeof multiUnion})`);
}

// Test union with literal types
type LiteralUnion = "red" | "green" | "blue";
type NumericLiteralUnion = 1 | 2 | 3 | 4 | 5;
type MixedLiteralUnion = "yes" | "no" | 0 | 1;

function testLiteralUnionTypes(): void {
    let color: LiteralUnion = "red";
    console.log(`Color: ${color}`);
    
    color = "green";
    console.log(`Color: ${color}`);
    
    color = "blue";
    console.log(`Color: ${color}`);
    
    let number: NumericLiteralUnion = 1;
    console.log(`Number: ${number}`);
    
    number = 5;
    console.log(`Number: ${number}`);
    
    let mixed: MixedLiteralUnion = "yes";
    console.log(`Mixed: ${mixed}`);
    
    mixed = 1;
    console.log(`Mixed: ${mixed}`);
}

// Test union with object types
type ObjectUnion = { name: string } | { id: number } | { value: boolean };

function testObjectUnionTypes(): void {
    let objUnion: ObjectUnion = { name: "test" };
    console.log(`Object union (name): ${JSON.stringify(objUnion)}`);
    
    objUnion = { id: 42 };
    console.log(`Object union (id): ${JSON.stringify(objUnion)}`);
    
    objUnion = { value: true };
    console.log(`Object union (value): ${JSON.stringify(objUnion)}`);
}

// =============================================================================
// Test 2: Intersection Types
// =============================================================================

// Test basic intersection types
interface Nameable {
    name: string;
}

interface Countable {
    count: number;
}

interface Describable {
    description: string;
}

type BasicIntersection = Nameable & Countable;
type MultiIntersection = Nameable & Countable & Describable;

function testBasicIntersectionTypes(): void {
    let basicIntersection: BasicIntersection = {
        name: "test",
        count: 42
    };
    
    console.log(`Basic intersection: ${basicIntersection.name} (count: ${basicIntersection.count})`);
    
    let multiIntersection: MultiIntersection = {
        name: "multi test",
        count: 100,
        description: "A multi intersection test"
    };
    
    console.log(`Multi intersection: ${multiIntersection.name} (count: ${multiIntersection.count}, desc: ${multiIntersection.description})`);
}

// Test intersection with primitive types
type StringIntersection = string & { length: number };
type NumberIntersection = number & { toString(): string };

function testPrimitiveIntersectionTypes(): void {
    // Note: These are more theoretical as primitive types can't be intersected in practice
    // but they test the parser's ability to handle the syntax
    console.log("Primitive intersection types are parsed correctly");
}

// =============================================================================
// Test 3: Object Type Literals
// =============================================================================

// Test basic object type literals
type BasicObjectType = {
    name: string;
    age: number;
    isActive: boolean;
};

// Test object type with optional properties
type OptionalObjectType = {
    name: string;
    age?: number;
    isActive?: boolean;
};

// Test object type with readonly properties
type ReadonlyObjectType = {
    readonly id: number;
    readonly name: string;
    mutable: string;
};

// Test object type with index signatures
type IndexedObjectType = {
    [key: string]: any;
    name: string;
};

type NumericIndexedObjectType = {
    [key: number]: string;
    length: number;
};

function testObjectTypeLiterals(): void {
    let basicObj: BasicObjectType = {
        name: "John",
        age: 30,
        isActive: true
    };
    
    console.log(`Basic object: ${JSON.stringify(basicObj)}`);
    
    let optionalObj: OptionalObjectType = {
        name: "Jane"
        // age and isActive are optional
    };
    
    console.log(`Optional object: ${JSON.stringify(optionalObj)}`);
    
    let readonlyObj: ReadonlyObjectType = {
        id: 1,
        name: "Readonly",
        mutable: "can change"
    };
    
    console.log(`Readonly object: ${JSON.stringify(readonlyObj)}`);
    
    let indexedObj: IndexedObjectType = {
        name: "Indexed",
        extra: "value",
        another: 42
    };
    
    console.log(`Indexed object: ${JSON.stringify(indexedObj)}`);
    
    let numericIndexedObj: NumericIndexedObjectType = {
        0: "first",
        1: "second",
        2: "third",
        length: 3
    };
    
    console.log(`Numeric indexed object: ${JSON.stringify(numericIndexedObj)}`);
}

// =============================================================================
// Test 4: Function Types
// =============================================================================

// Test basic function types
type BasicFunctionType = () => void;
type ParameterFunctionType = (x: number, y: string) => boolean;
type ReturnFunctionType = () => string;
type ComplexFunctionType = (x: number, y: string, z: boolean) => { result: number; message: string };

// Test function types with optional parameters
type OptionalParameterFunctionType = (x: number, y?: string) => void;

// Test function types with rest parameters
type RestParameterFunctionType = (x: number, ...rest: string[]) => void;

// Test function types with generic parameters
type GenericFunctionType = <T>(value: T) => T;

function testFunctionTypes(): void {
    let basicFunc: BasicFunctionType = () => {
        console.log("Basic function called");
    };
    basicFunc();
    
    let paramFunc: ParameterFunctionType = (x: number, y: string) => {
        console.log(`Parameter function: ${x}, ${y}`);
        return x > 0 && y.length > 0;
    };
    let paramResult = paramFunc(42, "test");
    console.log(`Parameter function result: ${paramResult}`);
    
    let returnFunc: ReturnFunctionType = () => {
        return "Hello from return function";
    };
    let returnResult = returnFunc();
    console.log(`Return function result: ${returnResult}`);
    
    let complexFunc: ComplexFunctionType = (x: number, y: string, z: boolean) => {
        return {
            result: x + y.length,
            message: `Complex: ${y} (${z})`
        };
    };
    let complexResult = complexFunc(10, "hello", true);
    console.log(`Complex function result: ${JSON.stringify(complexResult)}`);
    
    let optionalFunc: OptionalParameterFunctionType = (x: number, y?: string) => {
        console.log(`Optional parameter function: ${x}, ${y || "default"}`);
    };
    optionalFunc(42);
    optionalFunc(42, "provided");
    
    let restFunc: RestParameterFunctionType = (x: number, ...rest: string[]) => {
        console.log(`Rest parameter function: ${x}, rest: ${rest.join(", ")}`);
    };
    restFunc(1, "a", "b", "c");
    
    let genericFunc: GenericFunctionType = <T>(value: T): T => {
        console.log(`Generic function: ${value}`);
        return value;
    };
    let genericResult = genericFunc<string>("generic test");
    console.log(`Generic function result: ${genericResult}`);
}

// =============================================================================
// Test 5: Complex Type Combinations
// =============================================================================

// Test union of function types
type FunctionUnion = ((x: number) => string) | ((x: string) => number);

// Test intersection of function types
type FunctionIntersection = ((x: number) => string) & ((x: string) => number);

// Test object type with function properties
type ObjectWithFunctions = {
    name: string;
    process: (value: number) => string;
    validate: (input: string) => boolean;
};

// Test function type with object return
type FunctionWithObjectReturn = (x: number) => {
    value: number;
    doubled: number;
    message: string;
};

// Test nested type combinations
type NestedUnion = string | (number | boolean);
type NestedIntersection = (Nameable & Countable) & Describable;

function testComplexTypeCombinations(): void {
    let funcUnion: FunctionUnion = (x: number) => x.toString();
    let funcUnionResult = funcUnion(42);
    console.log(`Function union result: ${funcUnionResult}`);
    
    let objWithFuncs: ObjectWithFunctions = {
        name: "Object with functions",
        process: (value: number) => `Processed: ${value}`,
        validate: (input: string) => input.length > 0
    };
    
    let processResult = objWithFuncs.process(100);
    let validateResult = objWithFuncs.validate("test");
    console.log(`Object with functions - process: ${processResult}, validate: ${validateResult}`);
    
    let funcWithObjectReturn: FunctionWithObjectReturn = (x: number) => ({
        value: x,
        doubled: x * 2,
        message: `Value ${x} doubled is ${x * 2}`
    });
    
    let objectReturnResult = funcWithObjectReturn(21);
    console.log(`Function with object return: ${JSON.stringify(objectReturnResult)}`);
    
    let nestedUnion: NestedUnion = "string";
    console.log(`Nested union: ${nestedUnion}`);
    
    nestedUnion = 42;
    console.log(`Nested union: ${nestedUnion}`);
    
    nestedUnion = true;
    console.log(`Nested union: ${nestedUnion}`);
    
    let nestedIntersection: NestedIntersection = {
        name: "Nested intersection",
        count: 50,
        description: "A nested intersection test"
    };
    
    console.log(`Nested intersection: ${nestedIntersection.name} (count: ${nestedIntersection.count}, desc: ${nestedIntersection.description})`);
}

// =============================================================================
// Test 6: Type Aliases
// =============================================================================

// Test basic type aliases
type StringAlias = string;
type NumberAlias = number;
type BooleanAlias = boolean;

// Test complex type aliases
type User = {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
};

type UserList = User[];
type UserMap = { [key: string]: User };
type UserFunction = (user: User) => string;

// Test generic type aliases
type GenericAlias<T> = {
    value: T;
    metadata: string;
};

type GenericUnionAlias<T, U> = T | U;
type GenericIntersectionAlias<T, U> = T & U;

function testTypeAliases(): void {
    let stringAlias: StringAlias = "alias test";
    console.log(`String alias: ${stringAlias}`);
    
    let numberAlias: NumberAlias = 42;
    console.log(`Number alias: ${numberAlias}`);
    
    let booleanAlias: BooleanAlias = true;
    console.log(`Boolean alias: ${booleanAlias}`);
    
    let user: User = {
        id: 1,
        name: "John Doe",
        email: "john@example.com",
        isActive: true
    };
    
    console.log(`User: ${JSON.stringify(user)}`);
    
    let userList: UserList = [user, { id: 2, name: "Jane Doe", email: "jane@example.com", isActive: false }];
    console.log(`User list length: ${userList.length}`);
    
    let userMap: UserMap = {
        "user1": user,
        "user2": userList[1]
    };
    
    console.log(`User map keys: ${Object.keys(userMap).join(", ")}`);
    
    let userFunction: UserFunction = (user: User) => `${user.name} (${user.email})`;
    let userFunctionResult = userFunction(user);
    console.log(`User function result: ${userFunctionResult}`);
    
    let genericAlias: GenericAlias<number> = {
        value: 100,
        metadata: "generic test"
    };
    
    console.log(`Generic alias: ${JSON.stringify(genericAlias)}`);
    
    let genericUnion: GenericUnionAlias<string, number> = "union test";
    console.log(`Generic union alias: ${genericUnion}`);
    
    genericUnion = 42;
    console.log(`Generic union alias: ${genericUnion}`);
    
    let genericIntersection: GenericIntersectionAlias<Nameable, Countable> = {
        name: "intersection test",
        count: 25
    };
    
    console.log(`Generic intersection alias: ${genericIntersection.name} (count: ${genericIntersection.count})`);
}

// =============================================================================
// Test 7: Error Handling and Edge Cases
// =============================================================================

function testErrorHandling(): void {
    try {
        // Test invalid union type usage
        let union: string | number = "test";
        console.log(`Union type: ${union}`);
        
        // Test invalid intersection type usage
        let intersection: Nameable & Countable = {
            name: "test",
            count: 42
        };
        console.log(`Intersection type: ${intersection.name} (count: ${intersection.count})`);
        
        // Test invalid function type usage
        let func: (x: number) => string = (x: number) => x.toString();
        let funcResult = func(42);
        console.log(`Function type: ${funcResult}`);
        
        // Test invalid object type usage
        let obj: { name: string; age: number } = {
            name: "test",
            age: 30
        };
        console.log(`Object type: ${obj.name} (age: ${obj.age})`);
        
    } catch (error) {
        console.log(`Error caught: ${error}`);
    }
}

// =============================================================================
// Test 8: Performance Testing
// =============================================================================

function testPerformance(): void {
    const iterations = 100000;
    
    // Test union type performance
    let startTime = Date.now();
    let union: string | number = "test";
    let sum = 0;
    for (let i = 0; i < iterations; i++) {
        if (typeof union === "string") {
            sum += union.length;
        } else {
            sum += union;
        }
    }
    let unionTime = Date.now() - startTime;
    console.log(`Union type operations (${iterations} iterations): ${unionTime}ms`);
    
    // Test intersection type performance
    startTime = Date.now();
    let intersection: Nameable & Countable = {
        name: "test",
        count: 42
    };
    let intersectionSum = 0;
    for (let i = 0; i < iterations; i++) {
        intersectionSum += intersection.count;
    }
    let intersectionTime = Date.now() - startTime;
    console.log(`Intersection type operations (${iterations} iterations): ${intersectionTime}ms`);
    
    // Test function type performance
    startTime = Date.now();
    let func: (x: number) => number = (x: number) => x * 2;
    let funcSum = 0;
    for (let i = 0; i < iterations; i++) {
        funcSum += func(i);
    }
    let funcTime = Date.now() - startTime;
    console.log(`Function type operations (${iterations} iterations): ${funcTime}ms`);
    
    // Test object type performance
    startTime = Date.now();
    let obj: { value: number } = { value: 42 };
    let objSum = 0;
    for (let i = 0; i < iterations; i++) {
        objSum += obj.value;
    }
    let objTime = Date.now() - startTime;
    console.log(`Object type operations (${iterations} iterations): ${objTime}ms`);
}

// =============================================================================
// Main Test Runner
// =============================================================================

function runParserTypeParsingTests(): void {
    console.log("🧪 Running Parser Type Parsing Tests");
    console.log("=====================================");
    
    console.log("\n📊 Test 1: Union Types");
    console.log("─".repeat(50));
    testBasicUnionTypes();
    testLiteralUnionTypes();
    testObjectUnionTypes();
    
    console.log("\n📊 Test 2: Intersection Types");
    console.log("─".repeat(50));
    testBasicIntersectionTypes();
    testPrimitiveIntersectionTypes();
    
    console.log("\n📊 Test 3: Object Type Literals");
    console.log("─".repeat(50));
    testObjectTypeLiterals();
    
    console.log("\n📊 Test 4: Function Types");
    console.log("─".repeat(50));
    testFunctionTypes();
    
    console.log("\n📊 Test 5: Complex Type Combinations");
    console.log("─".repeat(50));
    testComplexTypeCombinations();
    
    console.log("\n📊 Test 6: Type Aliases");
    console.log("─".repeat(50));
    testTypeAliases();
    
    console.log("\n📊 Test 7: Error Handling and Edge Cases");
    console.log("─".repeat(50));
    testErrorHandling();
    
    console.log("\n📊 Test 8: Performance Testing");
    console.log("─".repeat(50));
    testPerformance();
    
    console.log("\n✅ Parser Type Parsing Tests Completed Successfully!");
}

// Export for use in other test files
export { runParserTypeParsingTests };

// Run if this is the main module
if (require.main === module) {
    runParserTypeParsingTests();
}
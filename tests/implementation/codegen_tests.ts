// Comprehensive Tests for LLVMCodeGen Improvements
// Tests function pointers, method calls, property access, index access, monomorphization

// =============================================================================
// Test 1: Function Pointers
// =============================================================================

// Test basic function pointer types
type BasicFunctionPointer = () => void;
type ParameterFunctionPointer = (x: number, y: string) => boolean;
type ReturnFunctionPointer = () => string;
type ComplexFunctionPointer = (x: number, y: string, z: boolean) => { result: number; message: string };

// Test function pointer variables
let basicFuncPtr: BasicFunctionPointer;
let paramFuncPtr: ParameterFunctionPointer;
let returnFuncPtr: ReturnFunctionPointer;
let complexFuncPtr: ComplexFunctionPointer;

function testBasicFunctionPointers(): void {
    // Assign functions to pointers
    basicFuncPtr = () => {
        console.log("Basic function pointer called");
    };
    
    paramFuncPtr = (x: number, y: string) => {
        console.log(`Parameter function pointer: ${x}, ${y}`);
        return x > 0 && y.length > 0;
    };
    
    returnFuncPtr = () => {
        return "Hello from return function pointer";
    };
    
    complexFuncPtr = (x: number, y: string, z: boolean) => {
        return {
            result: x + y.length,
            message: `Complex: ${y} (${z})`
        };
    };
    
    // Call function pointers
    basicFuncPtr();
    
    let paramResult = paramFuncPtr(42, "test");
    console.log(`Parameter function pointer result: ${paramResult}`);
    
    let returnResult = returnFuncPtr();
    console.log(`Return function pointer result: ${returnResult}`);
    
    let complexResult = complexFuncPtr(10, "hello", true);
    console.log(`Complex function pointer result: ${JSON.stringify(complexResult)}`);
}

// Test function pointer arrays
type FunctionPointerArray = (() => void)[];
type ParameterFunctionPointerArray = ((x: number) => string)[];

function testFunctionPointerArrays(): void {
    let funcArray: FunctionPointerArray = [
        () => console.log("Function 1"),
        () => console.log("Function 2"),
        () => console.log("Function 3")
    ];
    
    for (let i = 0; i < funcArray.length; i++) {
        funcArray[i]();
    }
    
    let paramFuncArray: ParameterFunctionPointerArray = [
        (x: number) => `Number: ${x}`,
        (x: number) => `Double: ${x * 2}`,
        (x: number) => `Square: ${x * x}`
    ];
    
    for (let i = 0; i < paramFuncArray.length; i++) {
        let result = paramFuncArray[i](i + 1);
        console.log(`Parameter function array result: ${result}`);
    }
}

// =============================================================================
// Test 2: Method Calls
// =============================================================================

// Test basic class with methods
class TestClass {
    private value: number;
    private name: string;
    
    constructor(value: number, name: string) {
        this.value = value;
        this.name = name;
    }
    
    // Test basic method
    getValue(): number {
        return this.value;
    }
    
    getName(): string {
        return this.name;
    }
    
    // Test method with parameters
    setValue(newValue: number): void {
        this.value = newValue;
    }
    
    setName(newName: string): void {
        this.name = newName;
    }
    
    // Test method with return value
    getInfo(): { value: number; name: string } {
        return {
            value: this.value,
            name: this.name
        };
    }
    
    // Test method with complex parameters
    processData(data: { input: number; multiplier: number }): { result: number; message: string } {
        let result = data.input * data.multiplier;
        return {
            result: result,
            message: `Processed ${data.input} * ${data.multiplier} = ${result}`
        };
    }
}

// Test inheritance with method calls
class DerivedClass extends TestClass {
    private additional: string;
    
    constructor(value: number, name: string, additional: string) {
        super(value, name);
        this.additional = additional;
    }
    
    getAdditional(): string {
        return this.additional;
    }
    
    setAdditional(newAdditional: string): void {
        this.additional = newAdditional;
    }
    
    // Override parent method
    getInfo(): { value: number; name: string; additional: string } {
        let parentInfo = super.getInfo();
        return {
            value: parentInfo.value,
            name: parentInfo.name,
            additional: this.additional
        };
    }
}

function testMethodCalls(): void {
    let testObj = new TestClass(42, "test object");
    
    // Test basic method calls
    let value = testObj.getValue();
    let name = testObj.getName();
    console.log(`Basic method calls - value: ${value}, name: ${name}`);
    
    // Test method calls with parameters
    testObj.setValue(100);
    testObj.setName("updated name");
    
    let updatedValue = testObj.getValue();
    let updatedName = testObj.getName();
    console.log(`Updated method calls - value: ${updatedValue}, name: ${updatedName}`);
    
    // Test method calls with return values
    let info = testObj.getInfo();
    console.log(`Method with return value: ${JSON.stringify(info)}`);
    
    // Test method calls with complex parameters
    let processResult = testObj.processData({ input: 10, multiplier: 5 });
    console.log(`Method with complex parameters: ${JSON.stringify(processResult)}`);
    
    // Test inheritance method calls
    let derivedObj = new DerivedClass(200, "derived object", "additional data");
    
    let derivedValue = derivedObj.getValue();
    let derivedName = derivedObj.getName();
    let derivedAdditional = derivedObj.getAdditional();
    console.log(`Derived method calls - value: ${derivedValue}, name: ${derivedName}, additional: ${derivedAdditional}`);
    
    // Test overridden method
    let derivedInfo = derivedObj.getInfo();
    console.log(`Overridden method: ${JSON.stringify(derivedInfo)}`);
}

// =============================================================================
// Test 3: Property Access
// =============================================================================

// Test basic property access
class PropertyTestClass {
    public publicProperty: string;
    private privateProperty: number;
    protected protectedProperty: boolean;
    
    constructor(publicProp: string, privateProp: number, protectedProp: boolean) {
        this.publicProperty = publicProp;
        this.privateProperty = privateProp;
        this.protectedProperty = protectedProp;
    }
    
    // Test getter methods
    getPrivateProperty(): number {
        return this.privateProperty;
    }
    
    getProtectedProperty(): boolean {
        return this.protectedProperty;
    }
    
    // Test setter methods
    setPrivateProperty(value: number): void {
        this.privateProperty = value;
    }
    
    setProtectedProperty(value: boolean): void {
        this.protectedProperty = value;
    }
}

// Test property access with inheritance
class PropertyDerivedClass extends PropertyTestClass {
    public derivedProperty: string;
    
    constructor(publicProp: string, privateProp: number, protectedProp: boolean, derivedProp: string) {
        super(publicProp, privateProp, protectedProp);
        this.derivedProperty = derivedProp;
    }
    
    // Test access to protected property
    getProtectedFromDerived(): boolean {
        return this.protectedProperty;
    }
    
    setProtectedFromDerived(value: boolean): void {
        this.protectedProperty = value;
    }
}

function testPropertyAccess(): void {
    let propObj = new PropertyTestClass("public", 42, true);
    
    // Test public property access
    console.log(`Public property: ${propObj.publicProperty}`);
    propObj.publicProperty = "updated public";
    console.log(`Updated public property: ${propObj.publicProperty}`);
    
    // Test private property access through methods
    let privateValue = propObj.getPrivateProperty();
    console.log(`Private property: ${privateValue}`);
    propObj.setPrivateProperty(100);
    let updatedPrivateValue = propObj.getPrivateProperty();
    console.log(`Updated private property: ${updatedPrivateValue}`);
    
    // Test protected property access through methods
    let protectedValue = propObj.getProtectedProperty();
    console.log(`Protected property: ${protectedValue}`);
    propObj.setProtectedProperty(false);
    let updatedProtectedValue = propObj.getProtectedProperty();
    console.log(`Updated protected property: ${updatedProtectedValue}`);
    
    // Test property access with inheritance
    let derivedPropObj = new PropertyDerivedClass("derived public", 200, false, "derived");
    
    console.log(`Derived public property: ${derivedPropObj.publicProperty}`);
    console.log(`Derived property: ${derivedPropObj.derivedProperty}`);
    
    let derivedPrivateValue = derivedPropObj.getPrivateProperty();
    console.log(`Derived private property: ${derivedPrivateValue}`);
    
    let derivedProtectedValue = derivedPropObj.getProtectedFromDerived();
    console.log(`Derived protected property: ${derivedProtectedValue}`);
}

// =============================================================================
// Test 4: Index Access
// =============================================================================

// Test array index access
function testArrayIndexAccess(): void {
    let numbers: number[] = [1, 2, 3, 4, 5];
    let strings: string[] = ["a", "b", "c", "d", "e"];
    let mixed: (string | number)[] = ["a", 1, "b", 2, "c"];
    
    // Test basic array indexing
    console.log(`Numbers[0]: ${numbers[0]}`);
    console.log(`Numbers[2]: ${numbers[2]}`);
    console.log(`Numbers[4]: ${numbers[4]}`);
    
    console.log(`Strings[0]: ${strings[0]}`);
    console.log(`Strings[2]: ${strings[2]}`);
    console.log(`Strings[4]: ${strings[4]}`);
    
    console.log(`Mixed[0]: ${mixed[0]}`);
    console.log(`Mixed[1]: ${mixed[1]}`);
    console.log(`Mixed[2]: ${mixed[2]}`);
    
    // Test array index assignment
    numbers[0] = 10;
    strings[0] = "z";
    mixed[0] = "x";
    
    console.log(`Updated numbers[0]: ${numbers[0]}`);
    console.log(`Updated strings[0]: ${strings[0]}`);
    console.log(`Updated mixed[0]: ${mixed[0]}`);
    
    // Test array index with variables
    let index = 2;
    console.log(`Numbers[${index}]: ${numbers[index]}`);
    console.log(`Strings[${index}]: ${strings[index]}`);
    console.log(`Mixed[${index}]: ${mixed[index]}`);
}

// Test object index access
function testObjectIndexAccess(): void {
    let obj: { [key: string]: any } = {
        "name": "test",
        "age": 30,
        "active": true
    };
    
    // Test string index access
    console.log(`Obj["name"]: ${obj["name"]}`);
    console.log(`Obj["age"]: ${obj["age"]}`);
    console.log(`Obj["active"]: ${obj["active"]}`);
    
    // Test string index assignment
    obj["name"] = "updated";
    obj["age"] = 35;
    obj["active"] = false;
    
    console.log(`Updated obj["name"]: ${obj["name"]}`);
    console.log(`Updated obj["age"]: ${obj["age"]}`);
    console.log(`Updated obj["active"]: ${obj["active"]}`);
    
    // Test index access with variables
    let key = "name";
    console.log(`Obj[${key}]: ${obj[key]}`);
    
    key = "age";
    console.log(`Obj[${key}]: ${obj[key]}`);
    
    // Test numeric index access
    let numericObj: { [key: number]: string } = {
        0: "zero",
        1: "one",
        2: "two"
    };
    
    console.log(`NumericObj[0]: ${numericObj[0]}`);
    console.log(`NumericObj[1]: ${numericObj[1]}`);
    console.log(`NumericObj[2]: ${numericObj[2]}`);
}

// Test tuple index access
function testTupleIndexAccess(): void {
    let tuple: [string, number, boolean] = ["hello", 42, true];
    
    // Test tuple indexing
    console.log(`Tuple[0]: ${tuple[0]}`);
    console.log(`Tuple[1]: ${tuple[1]}`);
    console.log(`Tuple[2]: ${tuple[2]}`);
    
    // Test tuple index assignment
    tuple[0] = "world";
    tuple[1] = 100;
    tuple[2] = false;
    
    console.log(`Updated tuple[0]: ${tuple[0]}`);
    console.log(`Updated tuple[1]: ${tuple[1]}`);
    console.log(`Updated tuple[2]: ${tuple[2]}`);
    
    // Test tuple index with variables
    let index = 1;
    console.log(`Tuple[${index}]: ${tuple[index]}`);
}

// =============================================================================
// Test 5: Generic Monomorphization
// =============================================================================

// Test basic generic function
function genericIdentity<T>(value: T): T {
    return value;
}

// Test generic function with constraints
function genericWithConstraint<T extends string | number>(value: T): T {
    return value;
}

// Test generic function with multiple parameters
function genericMultiple<T, U>(first: T, second: U): [T, U] {
    return [first, second];
}

// Test generic class
class GenericClass<T> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
    
    setValue(newValue: T): void {
        this.value = newValue;
    }
    
    // Test generic method
    process<U>(fn: (value: T) => U): U {
        return fn(this.value);
    }
}

// Test generic class with constraints
class ConstrainedGenericClass<T extends { id: number; name: string }> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    getById(id: number): T | undefined {
        return this.items.find(item => item.id === id);
    }
    
    getAll(): T[] {
        return this.items;
    }
}

function testGenericMonomorphization(): void {
    // Test generic function monomorphization
    let stringResult = genericIdentity<string>("hello");
    let numberResult = genericIdentity<number>(42);
    let booleanResult = genericIdentity<boolean>(true);
    
    console.log(`Generic identity string: ${stringResult}`);
    console.log(`Generic identity number: ${numberResult}`);
    console.log(`Generic identity boolean: ${booleanResult}`);
    
    // Test generic function with constraints
    let constraintStringResult = genericWithConstraint<string>("test");
    let constraintNumberResult = genericWithConstraint<number>(123);
    
    console.log(`Generic constraint string: ${constraintStringResult}`);
    console.log(`Generic constraint number: ${constraintNumberResult}`);
    
    // Test generic function with multiple parameters
    let multiResult = genericMultiple<string, number>("hello", 42);
    console.log(`Generic multiple result: ${JSON.stringify(multiResult)}`);
    
    // Test generic class monomorphization
    let stringClass = new GenericClass<string>("test");
    let numberClass = new GenericClass<number>(100);
    let booleanClass = new GenericClass<boolean>(false);
    
    console.log(`Generic class string: ${stringClass.getValue()}`);
    console.log(`Generic class number: ${numberClass.getValue()}`);
    console.log(`Generic class boolean: ${booleanClass.getValue()}`);
    
    // Test generic method
    let stringProcessResult = stringClass.process<number>(s => s.length);
    let numberProcessResult = numberClass.process<string>(n => n.toString());
    let booleanProcessResult = booleanClass.process<string>(b => b ? "true" : "false");
    
    console.log(`Generic method string process: ${stringProcessResult}`);
    console.log(`Generic method number process: ${numberProcessResult}`);
    console.log(`Generic method boolean process: ${booleanProcessResult}`);
    
    // Test generic class with constraints
    let constrainedClass = new ConstrainedGenericClass<{ id: number; name: string; extra: string }>();
    
    constrainedClass.add({ id: 1, name: "item1", extra: "data1" });
    constrainedClass.add({ id: 2, name: "item2", extra: "data2" });
    
    let foundItem = constrainedClass.getById(1);
    let allItems = constrainedClass.getAll();
    
    console.log(`Constrained generic found item: ${JSON.stringify(foundItem)}`);
    console.log(`Constrained generic all items: ${JSON.stringify(allItems)}`);
}

// =============================================================================
// Test 6: Complex Integration Scenarios
// =============================================================================

// Test function pointer with method calls
type MethodFunctionPointer = (obj: TestClass) => string;

function testFunctionPointerWithMethods(): void {
    let methodFuncPtr: MethodFunctionPointer = (obj: TestClass) => {
        return `Object: ${obj.getName()} (value: ${obj.getValue()})`;
    };
    
    let testObj = new TestClass(42, "test object");
    let result = methodFuncPtr(testObj);
    console.log(`Function pointer with method result: ${result}`);
}

// Test property access with index access
class IndexedPropertyClass {
    private data: { [key: string]: any } = {};
    
    setProperty(key: string, value: any): void {
        this.data[key] = value;
    }
    
    getProperty(key: string): any {
        return this.data[key];
    }
    
    // Test index access method
    getData(): { [key: string]: any } {
        return this.data;
    }
}

function testPropertyAccessWithIndexAccess(): void {
    let indexedObj = new IndexedPropertyClass();
    
    indexedObj.setProperty("name", "test");
    indexedObj.setProperty("age", 30);
    indexedObj.setProperty("active", true);
    
    let name = indexedObj.getProperty("name");
    let age = indexedObj.getProperty("age");
    let active = indexedObj.getProperty("active");
    
    console.log(`Indexed property name: ${name}`);
    console.log(`Indexed property age: ${age}`);
    console.log(`Indexed property active: ${active}`);
    
    let data = indexedObj.getData();
    console.log(`Indexed property data: ${JSON.stringify(data)}`);
}

// Test generic with function pointers
function testGenericWithFunctionPointers<T>(value: T, processor: (value: T) => string): string {
    return processor(value);
}

function testGenericWithFunctionPointersUsage(): void {
    let stringProcessor = (value: string) => `String: ${value}`;
    let numberProcessor = (value: number) => `Number: ${value}`;
    let booleanProcessor = (value: boolean) => `Boolean: ${value}`;
    
    let stringResult = testGenericWithFunctionPointers<string>("hello", stringProcessor);
    let numberResult = testGenericWithFunctionPointers<number>(42, numberProcessor);
    let booleanResult = testGenericWithFunctionPointers<boolean>(true, booleanProcessor);
    
    console.log(`Generic with function pointer string: ${stringResult}`);
    console.log(`Generic with function pointer number: ${numberResult}`);
    console.log(`Generic with function pointer boolean: ${booleanResult}`);
}

// =============================================================================
// Test 7: Error Handling and Edge Cases
// =============================================================================

function testErrorHandling(): void {
    try {
        // Test function pointer with null
        let funcPtr: (() => void) | null = null;
        if (funcPtr !== null) {
            funcPtr();
        }
        
        // Test method call on null object
        let obj: TestClass | null = null;
        if (obj !== null) {
            obj.getValue();
        }
        
        // Test property access on undefined
        let propObj: PropertyTestClass | undefined = undefined;
        if (propObj !== undefined) {
            console.log(propObj.publicProperty);
        }
        
        // Test index access out of bounds
        let array: number[] = [1, 2, 3];
        let index = 10;
        if (index < array.length) {
            console.log(array[index]);
        }
        
        // Test generic with invalid type
        let genericResult = genericIdentity<any>("test");
        console.log(`Generic with any type: ${genericResult}`);
        
    } catch (error) {
        console.log(`Error caught: ${error}`);
    }
}

// =============================================================================
// Test 8: Performance Testing
// =============================================================================

function testPerformance(): void {
    const iterations = 100000;
    
    // Test function pointer performance
    let startTime = Date.now();
    let funcPtr: (x: number) => number = (x: number) => x * 2;
    let sum = 0;
    for (let i = 0; i < iterations; i++) {
        sum += funcPtr(i);
    }
    let funcPtrTime = Date.now() - startTime;
    console.log(`Function pointer operations (${iterations} iterations): ${funcPtrTime}ms`);
    
    // Test method call performance
    startTime = Date.now();
    let testObj = new TestClass(42, "test");
    let methodSum = 0;
    for (let i = 0; i < iterations; i++) {
        methodSum += testObj.getValue();
    }
    let methodTime = Date.now() - startTime;
    console.log(`Method call operations (${iterations} iterations): ${methodTime}ms`);
    
    // Test property access performance
    startTime = Date.now();
    let propSum = 0;
    for (let i = 0; i < iterations; i++) {
        propSum += testObj.getValue();
    }
    let propTime = Date.now() - startTime;
    console.log(`Property access operations (${iterations} iterations): ${propTime}ms`);
    
    // Test index access performance
    startTime = Date.now();
    let array: number[] = Array.from({ length: 1000 }, (_, i) => i);
    let indexSum = 0;
    for (let i = 0; i < iterations; i++) {
        indexSum += array[i % array.length];
    }
    let indexTime = Date.now() - startTime;
    console.log(`Index access operations (${iterations} iterations): ${indexTime}ms`);
    
    // Test generic monomorphization performance
    startTime = Date.now();
    let genericSum = 0;
    for (let i = 0; i < iterations; i++) {
        genericSum += genericIdentity<number>(i);
    }
    let genericTime = Date.now() - startTime;
    console.log(`Generic monomorphization operations (${iterations} iterations): ${genericTime}ms`);
}

// =============================================================================
// Main Test Runner
// =============================================================================

function runCodeGenTests(): void {
    console.log("🧪 Running LLVMCodeGen Tests");
    console.log("=============================");
    
    console.log("\n📊 Test 1: Function Pointers");
    console.log("─".repeat(50));
    testBasicFunctionPointers();
    testFunctionPointerArrays();
    
    console.log("\n📊 Test 2: Method Calls");
    console.log("─".repeat(50));
    testMethodCalls();
    
    console.log("\n📊 Test 3: Property Access");
    console.log("─".repeat(50));
    testPropertyAccess();
    
    console.log("\n📊 Test 4: Index Access");
    console.log("─".repeat(50));
    testArrayIndexAccess();
    testObjectIndexAccess();
    testTupleIndexAccess();
    
    console.log("\n📊 Test 5: Generic Monomorphization");
    console.log("─".repeat(50));
    testGenericMonomorphization();
    
    console.log("\n📊 Test 6: Complex Integration Scenarios");
    console.log("─".repeat(50));
    testFunctionPointerWithMethods();
    testPropertyAccessWithIndexAccess();
    testGenericWithFunctionPointersUsage();
    
    console.log("\n📊 Test 7: Error Handling and Edge Cases");
    console.log("─".repeat(50));
    testErrorHandling();
    
    console.log("\n📊 Test 8: Performance Testing");
    console.log("─".repeat(50));
    testPerformance();
    
    console.log("\n✅ LLVMCodeGen Tests Completed Successfully!");
}

// Export for use in other test files
export { runCodeGenTests };

// Run if this is the main module
if (require.main === module) {
    runCodeGenTests();
}
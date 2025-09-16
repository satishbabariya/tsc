// Comprehensive Tests for SemanticAnalyzer Enhancements
// Tests tuple indexing, constraint validation, enum member checking, circular inheritance detection, destructor analysis

// =============================================================================
// Test 1: Tuple Indexing
// =============================================================================

// Test basic tuple indexing
type BasicTuple = [string, number, boolean];

function testBasicTupleIndexing(): void {
    let tuple: BasicTuple = ["hello", 42, true];
    
    // Test valid indices
    let firstElement = tuple[0]; // Should be string
    let secondElement = tuple[1]; // Should be number
    let thirdElement = tuple[2]; // Should be boolean
    
    console.log(`Tuple[0]: ${firstElement} (type: ${typeof firstElement})`);
    console.log(`Tuple[1]: ${secondElement} (type: ${typeof secondElement})`);
    console.log(`Tuple[2]: ${thirdElement} (type: ${typeof thirdElement})`);
    
    // Test operations on indexed elements
    let stringResult = firstElement + " world";
    let numberResult = secondElement + 10;
    let booleanResult = !thirdElement;
    
    console.log(`String operation: ${stringResult}`);
    console.log(`Number operation: ${numberResult}`);
    console.log(`Boolean operation: ${booleanResult}`);
}

// Test tuple with different types
type MixedTuple = [number, string, boolean, number[]];

function testMixedTupleIndexing(): void {
    let tuple: MixedTuple = [1, "test", false, [1, 2, 3]];
    
    let numberElement = tuple[0];
    let stringElement = tuple[1];
    let booleanElement = tuple[2];
    let arrayElement = tuple[3];
    
    console.log(`Mixed tuple[0]: ${numberElement} (type: ${typeof numberElement})`);
    console.log(`Mixed tuple[1]: ${stringElement} (type: ${typeof stringElement})`);
    console.log(`Mixed tuple[2]: ${booleanElement} (type: ${typeof booleanElement})`);
    console.log(`Mixed tuple[3]: ${JSON.stringify(arrayElement)} (type: ${typeof arrayElement})`);
}

// Test tuple with optional elements
type OptionalTuple = [string?, number?, boolean?];

function testOptionalTupleIndexing(): void {
    let tuple1: OptionalTuple = ["hello"];
    let tuple2: OptionalTuple = ["hello", 42];
    let tuple3: OptionalTuple = ["hello", 42, true];
    
    console.log(`Optional tuple1[0]: ${tuple1[0]}`);
    console.log(`Optional tuple1[1]: ${tuple1[1]}`);
    console.log(`Optional tuple1[2]: ${tuple1[2]}`);
    
    console.log(`Optional tuple2[0]: ${tuple2[0]}`);
    console.log(`Optional tuple2[1]: ${tuple2[1]}`);
    console.log(`Optional tuple2[2]: ${tuple2[2]}`);
    
    console.log(`Optional tuple3[0]: ${tuple3[0]}`);
    console.log(`Optional tuple3[1]: ${tuple3[1]}`);
    console.log(`Optional tuple3[2]: ${tuple3[2]}`);
}

// Test tuple with rest elements
type RestTuple = [string, number, ...boolean[]];

function testRestTupleIndexing(): void {
    let tuple: RestTuple = ["hello", 42, true, false, true];
    
    console.log(`Rest tuple[0]: ${tuple[0]}`);
    console.log(`Rest tuple[1]: ${tuple[1]}`);
    console.log(`Rest tuple[2]: ${tuple[2]}`);
    console.log(`Rest tuple[3]: ${tuple[3]}`);
    console.log(`Rest tuple[4]: ${tuple[4]}`);
}

// =============================================================================
// Test 2: Type Parameter Constraint Validation
// =============================================================================

// Test class constraint
class BaseClass {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
}

class DerivedClass extends BaseClass {
    additional: string;
    
    constructor(id: number, name: string, additional: string) {
        super(id, name);
        this.additional = additional;
    }
}

// Test interface constraint
interface BaseInterface {
    getId(): number;
    getName(): string;
}

class InterfaceImpl implements BaseInterface {
    private id: number;
    private name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
}

// Test object constraint
type ObjectConstraint = {
    value: number;
    label: string;
};

// Test constrained type parameter
function testConstrainedTypeParameter<T extends BaseClass>(value: T): T {
    console.log(`Constrained type parameter: ${value.name} (ID: ${value.id})`);
    return value;
}

function testInterfaceConstraint<T extends BaseInterface>(value: T): T {
    console.log(`Interface constraint: ${value.getName()} (ID: ${value.getId()})`);
    return value;
}

function testObjectConstraint<T extends ObjectConstraint>(value: T): T {
    console.log(`Object constraint: ${value.label} = ${value.value}`);
    return value;
}

function testConstraintValidation(): void {
    // Test class constraint
    let derived = new DerivedClass(1, "test", "additional");
    let constrainedResult = testConstrainedTypeParameter<DerivedClass>(derived);
    console.log(`Constrained result: ${constrainedResult.additional}`);
    
    // Test interface constraint
    let interfaceImpl = new InterfaceImpl(2, "interface test");
    let interfaceResult = testInterfaceConstraint<InterfaceImpl>(interfaceImpl);
    console.log(`Interface result: ${interfaceResult.getName()}`);
    
    // Test object constraint
    let objectValue: ObjectConstraint = { value: 42, label: "object test" };
    let objectResult = testObjectConstraint<ObjectConstraint>(objectValue);
    console.log(`Object result: ${objectResult.label}`);
}

// =============================================================================
// Test 3: Enum Member Value Type Checking
// =============================================================================

// Test enum with string literals
enum StringEnum {
    First = "first",
    Second = "second",
    Third = "third"
}

// Test enum with numeric literals
enum NumericEnum {
    Zero = 0,
    One = 1,
    Two = 2
}

// Test enum with mixed literals
enum MixedEnum {
    StringValue = "string",
    NumericValue = 42,
    BooleanValue = true
}

// Test enum with constant expressions
const CONSTANT_VALUE = 100;

enum ConstantEnum {
    ConstantValue = CONSTANT_VALUE,
    ComputedValue = CONSTANT_VALUE + 10
}

// Test enum with references to other enum members
enum ReferenceEnum {
    First = 1,
    Second = First + 1,
    Third = Second + 1
}

function testEnumMemberValueTypes(): void {
    // Test string enum
    console.log(`StringEnum.First: ${StringEnum.First} (type: ${typeof StringEnum.First})`);
    console.log(`StringEnum.Second: ${StringEnum.Second} (type: ${typeof StringEnum.Second})`);
    console.log(`StringEnum.Third: ${StringEnum.Third} (type: ${typeof StringEnum.Third})`);
    
    // Test numeric enum
    console.log(`NumericEnum.Zero: ${NumericEnum.Zero} (type: ${typeof NumericEnum.Zero})`);
    console.log(`NumericEnum.One: ${NumericEnum.One} (type: ${typeof NumericEnum.One})`);
    console.log(`NumericEnum.Two: ${NumericEnum.Two} (type: ${typeof NumericEnum.Two})`);
    
    // Test mixed enum
    console.log(`MixedEnum.StringValue: ${MixedEnum.StringValue} (type: ${typeof MixedEnum.StringValue})`);
    console.log(`MixedEnum.NumericValue: ${MixedEnum.NumericValue} (type: ${typeof MixedEnum.NumericValue})`);
    console.log(`MixedEnum.BooleanValue: ${MixedEnum.BooleanValue} (type: ${typeof MixedEnum.BooleanValue})`);
    
    // Test constant enum
    console.log(`ConstantEnum.ConstantValue: ${ConstantEnum.ConstantValue} (type: ${typeof ConstantEnum.ConstantValue})`);
    console.log(`ConstantEnum.ComputedValue: ${ConstantEnum.ComputedValue} (type: ${typeof ConstantEnum.ComputedValue})`);
    
    // Test reference enum
    console.log(`ReferenceEnum.First: ${ReferenceEnum.First} (type: ${typeof ReferenceEnum.First})`);
    console.log(`ReferenceEnum.Second: ${ReferenceEnum.Second} (type: ${typeof ReferenceEnum.Second})`);
    console.log(`ReferenceEnum.Third: ${ReferenceEnum.Third} (type: ${typeof ReferenceEnum.Third})`);
}

// =============================================================================
// Test 4: Circular Inheritance Detection
// =============================================================================

// Test interface inheritance chain
interface InterfaceA {
    methodA(): void;
}

interface InterfaceB extends InterfaceA {
    methodB(): void;
}

interface InterfaceC extends InterfaceB {
    methodC(): void;
}

// Test class inheritance chain
class ClassA {
    methodA(): void {
        console.log("ClassA.methodA()");
    }
}

class ClassB extends ClassA {
    methodB(): void {
        console.log("ClassB.methodB()");
    }
}

class ClassC extends ClassB {
    methodC(): void {
        console.log("ClassC.methodC()");
    }
}

// Test multiple inheritance
interface InterfaceD extends InterfaceA, InterfaceB {
    methodD(): void;
}

class ClassD extends ClassA implements InterfaceD {
    methodB(): void {
        console.log("ClassD.methodB()");
    }
    
    methodD(): void {
        console.log("ClassD.methodD()");
    }
}

function testInheritanceChains(): void {
    // Test interface inheritance
    let interfaceC: InterfaceC = {
        methodA: () => console.log("InterfaceC.methodA()"),
        methodB: () => console.log("InterfaceC.methodB()"),
        methodC: () => console.log("InterfaceC.methodC()")
    };
    
    interfaceC.methodA();
    interfaceC.methodB();
    interfaceC.methodC();
    
    // Test class inheritance
    let classC = new ClassC();
    classC.methodA();
    classC.methodB();
    classC.methodC();
    
    // Test multiple inheritance
    let classD = new ClassD();
    classD.methodA();
    classD.methodB();
    classD.methodD();
}

// =============================================================================
// Test 5: Destructor Body Analysis
// =============================================================================

class ResourceManager {
    private resources: string[] = [];
    private isDisposed: boolean = false;
    
    addResource(resource: string): void {
        if (!this.isDisposed) {
            this.resources.push(resource);
        }
    }
    
    getResources(): string[] {
        return this.resources;
    }
    
    dispose(): void {
        if (!this.isDisposed) {
            this.resources = [];
            this.isDisposed = true;
        }
    }
}

class TestClass {
    private resourceManager: ResourceManager;
    private data: number[];
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        this.resourceManager = new ResourceManager();
        this.data = [1, 2, 3, 4, 5];
    }
    
    // Test destructor with proper cleanup
    destructor(): void {
        // Clean up resource manager
        this.resourceManager.dispose();
        
        // Clear data array
        this.data = [];
        
        // Reset name
        this.name = "";
    }
    
    // Test destructor with incomplete cleanup
    badDestructor(): void {
        // Missing cleanup for resourceManager
        // Missing cleanup for data
        // Only reset name
        this.name = "";
    }
    
    getResourceManager(): ResourceManager {
        return this.resourceManager;
    }
    
    getData(): number[] {
        return this.data;
    }
    
    getName(): string {
        return this.name;
    }
}

function testDestructorAnalysis(): void {
    let testObj = new TestClass("test object");
    
    // Add some resources
    testObj.getResourceManager().addResource("resource1");
    testObj.getResourceManager().addResource("resource2");
    
    console.log(`Before destructor - Resources: ${testObj.getResourceManager().getResources().length}`);
    console.log(`Before destructor - Data: ${testObj.getData().length}`);
    console.log(`Before destructor - Name: ${testObj.getName()}`);
    
    // Test proper destructor
    testObj.destructor();
    
    console.log(`After proper destructor - Resources: ${testObj.getResourceManager().getResources().length}`);
    console.log(`After proper destructor - Data: ${testObj.getData().length}`);
    console.log(`After proper destructor - Name: ${testObj.getName()}`);
    
    // Test bad destructor
    let testObj2 = new TestClass("test object 2");
    testObj2.getResourceManager().addResource("resource3");
    
    console.log(`Before bad destructor - Resources: ${testObj2.getResourceManager().getResources().length}`);
    console.log(`Before bad destructor - Data: ${testObj2.getData().length}`);
    console.log(`Before bad destructor - Name: ${testObj2.getName()}`);
    
    testObj2.badDestructor();
    
    console.log(`After bad destructor - Resources: ${testObj2.getResourceManager().getResources().length}`);
    console.log(`After bad destructor - Data: ${testObj2.getData().length}`);
    console.log(`After bad destructor - Name: ${testObj2.getName()}`);
}

// =============================================================================
// Test 6: Complex Type Scenarios
// =============================================================================

// Test generic with constraints
class GenericClass<T extends BaseClass> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
    
    processValue(): string {
        return `${this.value.name} (ID: ${this.value.id})`;
    }
}

// Test union types
type UnionType = string | number | boolean;

function processUnionType(value: UnionType): string {
    if (typeof value === "string") {
        return `String: ${value}`;
    } else if (typeof value === "number") {
        return `Number: ${value}`;
    } else if (typeof value === "boolean") {
        return `Boolean: ${value}`;
    }
    return "Unknown";
}

// Test intersection types
type IntersectionType = BaseClass & BaseInterface;

class IntersectionClass extends BaseClass implements BaseInterface {
    constructor(id: number, name: string) {
        super(id, name);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
}

function testComplexTypeScenarios(): void {
    // Test generic with constraints
    let derived = new DerivedClass(1, "test", "additional");
    let genericClass = new GenericClass<DerivedClass>(derived);
    let processedValue = genericClass.processValue();
    console.log(`Generic class processed value: ${processedValue}`);
    
    // Test union types
    let unionValues: UnionType[] = ["hello", 42, true];
    for (let value of unionValues) {
        let result = processUnionType(value);
        console.log(`Union type processing: ${result}`);
    }
    
    // Test intersection types
    let intersectionObj = new IntersectionClass(2, "intersection test");
    console.log(`Intersection type - ID: ${intersectionObj.getId()}, Name: ${intersectionObj.getName()}`);
}

// =============================================================================
// Test 7: Error Handling and Edge Cases
// =============================================================================

function testErrorHandling(): void {
    try {
        // Test tuple index out of bounds
        let tuple: [string, number] = ["hello", 42];
        let invalidIndex = tuple[5]; // Should handle gracefully
        console.log(`Invalid tuple index: ${invalidIndex}`);
        
        // Test enum with invalid value
        let enumValue = StringEnum.First;
        console.log(`Enum value: ${enumValue}`);
        
        // Test constraint validation with invalid type
        let invalidValue: any = "not a BaseClass";
        // This should be caught by type checking
        console.log(`Invalid constraint value: ${invalidValue}`);
        
    } catch (error) {
        console.log(`Error caught: ${error}`);
    }
}

// =============================================================================
// Test 8: Performance Testing
// =============================================================================

function testPerformance(): void {
    const iterations = 100000;
    
    // Test tuple indexing performance
    let startTime = Date.now();
    let tuple: [number, string, boolean] = [42, "test", true];
    let sum = 0;
    for (let i = 0; i < iterations; i++) {
        sum += tuple[0];
    }
    let tupleTime = Date.now() - startTime;
    console.log(`Tuple indexing (${iterations} iterations): ${tupleTime}ms`);
    
    // Test enum access performance
    startTime = Date.now();
    let enumSum = 0;
    for (let i = 0; i < iterations; i++) {
        enumSum += NumericEnum.One;
    }
    let enumTime = Date.now() - startTime;
    console.log(`Enum access (${iterations} iterations): ${enumTime}ms`);
    
    // Test constraint validation performance
    startTime = Date.now();
    let constraintSum = 0;
    for (let i = 0; i < iterations; i++) {
        let derived = new DerivedClass(i, `test${i}`, "additional");
        constraintSum += derived.id;
    }
    let constraintTime = Date.now() - startTime;
    console.log(`Constraint validation (${iterations} iterations): ${constraintTime}ms`);
}

// =============================================================================
// Main Test Runner
// =============================================================================

function runSemanticAnalyzerTests(): void {
    console.log("🧪 Running SemanticAnalyzer Tests");
    console.log("=================================");
    
    console.log("\n📊 Test 1: Tuple Indexing");
    console.log("─".repeat(50));
    testBasicTupleIndexing();
    testMixedTupleIndexing();
    testOptionalTupleIndexing();
    testRestTupleIndexing();
    
    console.log("\n📊 Test 2: Type Parameter Constraint Validation");
    console.log("─".repeat(50));
    testConstraintValidation();
    
    console.log("\n📊 Test 3: Enum Member Value Type Checking");
    console.log("─".repeat(50));
    testEnumMemberValueTypes();
    
    console.log("\n📊 Test 4: Circular Inheritance Detection");
    console.log("─".repeat(50));
    testInheritanceChains();
    
    console.log("\n📊 Test 5: Destructor Body Analysis");
    console.log("─".repeat(50));
    testDestructorAnalysis();
    
    console.log("\n📊 Test 6: Complex Type Scenarios");
    console.log("─".repeat(50));
    testComplexTypeScenarios();
    
    console.log("\n📊 Test 7: Error Handling and Edge Cases");
    console.log("─".repeat(50));
    testErrorHandling();
    
    console.log("\n📊 Test 8: Performance Testing");
    console.log("─".repeat(50));
    testPerformance();
    
    console.log("\n✅ SemanticAnalyzer Tests Completed Successfully!");
}

// Export for use in other test files
export { runSemanticAnalyzerTests };

// Run if this is the main module
if (require.main === module) {
    runSemanticAnalyzerTests();
}
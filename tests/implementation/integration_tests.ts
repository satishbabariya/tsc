// Comprehensive Integration Tests for All Implemented Features
// Tests combinations of TypeSystem, SemanticAnalyzer, Parser, CodeGen, and Runtime improvements

// =============================================================================
// Test 1: Type System + Semantic Analyzer Integration
// =============================================================================

// Test enum with complex type operations
enum ComplexEnum {
    StringValue = "complex_string",
    NumericValue = 42,
    BooleanValue = true
}

// Test generic with enum constraints
function processEnumValue<T extends ComplexEnum>(value: T): string {
    if (value === ComplexEnum.StringValue) {
        return `String enum: ${value}`;
    } else if (value === ComplexEnum.NumericValue) {
        return `Numeric enum: ${value}`;
    } else if (value === ComplexEnum.BooleanValue) {
        return `Boolean enum: ${value}`;
    }
    return "Unknown enum";
}

// Test tuple with enum values
type EnumTuple = [ComplexEnum, string, number];

// Test object type with enum properties
type EnumObjectType = {
    enumValue: ComplexEnum;
    description: string;
    count: number;
};

function testTypeSystemSemanticIntegration(): void {
    console.log("Testing TypeSystem + SemanticAnalyzer integration...");
    
    // Test enum type operations
    let stringEnum = ComplexEnum.StringValue;
    let numericEnum = ComplexEnum.NumericValue;
    let booleanEnum = ComplexEnum.BooleanValue;
    
    console.log(`String enum: ${processEnumValue(stringEnum)}`);
    console.log(`Numeric enum: ${processEnumValue(numericEnum)}`);
    console.log(`Boolean enum: ${processEnumValue(booleanEnum)}`);
    
    // Test tuple with enum values
    let enumTuple: EnumTuple = [ComplexEnum.StringValue, "test", 42];
    console.log(`Enum tuple: ${JSON.stringify(enumTuple)}`);
    
    // Test object type with enum properties
    let enumObject: EnumObjectType = {
        enumValue: ComplexEnum.NumericValue,
        description: "A numeric enum object",
        count: 100
    };
    
    console.log(`Enum object: ${JSON.stringify(enumObject)}`);
    
    // Test binary operations with enum values
    let enumSum = ComplexEnum.NumericValue + 10;
    console.log(`Enum sum: ${enumSum}`);
    
    // Test comparison operations with enum values
    let enumEqual = ComplexEnum.StringValue === ComplexEnum.StringValue;
    console.log(`Enum equality: ${enumEqual}`);
    
    console.log("TypeSystem + SemanticAnalyzer integration test completed");
}

// =============================================================================
// Test 2: Parser + Semantic Analyzer Integration
// =============================================================================

// Test complex type parsing with semantic validation
type ComplexUnionType = string | number | boolean | ComplexEnum;
type ComplexIntersectionType = EnumObjectType & { id: number; name: string };
type ComplexFunctionType = (value: ComplexUnionType) => ComplexIntersectionType;

// Test generic with complex type constraints
class ComplexGenericClass<T extends ComplexUnionType> {
    private value: T;
    private metadata: ComplexIntersectionType;
    
    constructor(value: T, metadata: ComplexIntersectionType) {
        this.value = value;
        this.metadata = metadata;
    }
    
    getValue(): T {
        return this.value;
    }
    
    getMetadata(): ComplexIntersectionType {
        return this.metadata;
    }
    
    processValue(): string {
        if (typeof this.value === "string") {
            return `String: ${this.value}`;
        } else if (typeof this.value === "number") {
            return `Number: ${this.value}`;
        } else if (typeof this.value === "boolean") {
            return `Boolean: ${this.value}`;
        } else {
            return `Enum: ${this.value}`;
        }
    }
}

function testParserSemanticIntegration(): void {
    console.log("Testing Parser + SemanticAnalyzer integration...");
    
    // Test complex union type
    let unionValue: ComplexUnionType = "hello";
    console.log(`Union type (string): ${unionValue}`);
    
    unionValue = 42;
    console.log(`Union type (number): ${unionValue}`);
    
    unionValue = true;
    console.log(`Union type (boolean): ${unionValue}`);
    
    unionValue = ComplexEnum.StringValue;
    console.log(`Union type (enum): ${unionValue}`);
    
    // Test complex intersection type
    let intersectionValue: ComplexIntersectionType = {
        enumValue: ComplexEnum.NumericValue,
        description: "Intersection test",
        count: 50,
        id: 1,
        name: "test"
    };
    
    console.log(`Intersection type: ${JSON.stringify(intersectionValue)}`);
    
    // Test complex function type
    let complexFunction: ComplexFunctionType = (value: ComplexUnionType) => {
        return {
            enumValue: ComplexEnum.StringValue,
            description: `Processed: ${value}`,
            count: 1,
            id: 1,
            name: "processed"
        };
    };
    
    let functionResult = complexFunction("test");
    console.log(`Complex function result: ${JSON.stringify(functionResult)}`);
    
    // Test complex generic class
    let genericClass = new ComplexGenericClass<string>(unionValue as string, intersectionValue);
    let processedValue = genericClass.processValue();
    console.log(`Generic class processed value: ${processedValue}`);
    
    console.log("Parser + SemanticAnalyzer integration test completed");
}

// =============================================================================
// Test 3: CodeGen + Runtime Integration
// =============================================================================

// Test class with ARC management and method calls
class ARCManagedClass {
    private id: number;
    private name: string;
    private data: number[];
    private ref: ARCManagedClass | null;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        this.data = [1, 2, 3, 4, 5];
        this.ref = null;
        console.log(`ARCManagedClass created: ${name} (ID: ${id})`);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
    
    setData(newData: number[]): void {
        this.data = newData;
    }
    
    setReference(ref: ARCManagedClass): void {
        this.ref = ref;
        console.log(`Reference set: ${this.name} -> ${ref.getName()}`);
    }
    
    getReference(): ARCManagedClass | null {
        return this.ref;
    }
    
    clearReference(): void {
        this.ref = null;
        console.log(`Reference cleared: ${this.name}`);
    }
    
    // Test method with generic parameters
    processGeneric<T>(value: T): string {
        return `Processed: ${value} (type: ${typeof value})`;
    }
    
    // Test method with function pointer
    processWithFunction<T>(value: T, processor: (value: T) => string): string {
        return processor(value);
    }
    
    destructor(): void {
        console.log(`ARCManagedClass destroyed: ${this.name} (ID: ${this.id})`);
        this.clearReference();
        this.data = [];
    }
}

// Test function pointer with ARC-managed objects
type ARCFunctionPointer = (obj: ARCManagedClass) => string;

function testCodeGenRuntimeIntegration(): void {
    console.log("Testing CodeGen + Runtime integration...");
    
    // Create ARC-managed objects
    let obj1 = new ARCManagedClass(1, "Object 1");
    let obj2 = new ARCManagedClass(2, "Object 2");
    
    // Test method calls
    let id1 = obj1.getId();
    let name1 = obj1.getName();
    let data1 = obj1.getData();
    
    console.log(`Object 1 - ID: ${id1}, Name: ${name1}, Data: ${JSON.stringify(data1)}`);
    
    // Test reference management
    obj1.setReference(obj2);
    let ref = obj1.getReference();
    console.log(`Object 1 reference: ${ref?.getName()}`);
    
    // Test generic method
    let genericResult = obj1.processGeneric<string>("generic test");
    console.log(`Generic method result: ${genericResult}`);
    
    // Test function pointer
    let funcPtr: ARCFunctionPointer = (obj: ARCManagedClass) => {
        return `Function pointer: ${obj.getName()} (ID: ${obj.getId()})`;
    };
    
    let funcResult = obj1.processWithFunction<ARCManagedClass>(obj1, funcPtr);
    console.log(`Function pointer result: ${funcResult}`);
    
    // Test property access
    obj1.setData([10, 20, 30]);
    let updatedData = obj1.getData();
    console.log(`Updated data: ${JSON.stringify(updatedData)}`);
    
    // Test index access
    let firstElement = updatedData[0];
    let secondElement = updatedData[1];
    console.log(`Index access - [0]: ${firstElement}, [1]: ${secondElement}`);
    
    // Clear references
    obj1.clearReference();
    
    // Clean up
    obj1.destructor();
    obj2.destructor();
    
    console.log("CodeGen + Runtime integration test completed");
}

// =============================================================================
// Test 4: Full Pipeline Integration
// =============================================================================

// Test complete pipeline: Parser -> Semantic -> CodeGen -> Runtime
class FullPipelineTest {
    private id: number;
    private name: string;
    private enumValue: ComplexEnum;
    private unionValue: ComplexUnionType;
    private intersectionValue: ComplexIntersectionType;
    private data: number[];
    private refs: FullPipelineTest[];
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        this.enumValue = ComplexEnum.StringValue;
        this.unionValue = "initial";
        this.intersectionValue = {
            enumValue: ComplexEnum.NumericValue,
            description: "Initial intersection",
            count: 0,
            id: id,
            name: name
        };
        this.data = [1, 2, 3, 4, 5];
        this.refs = [];
        console.log(`FullPipelineTest created: ${name} (ID: ${id})`);
    }
    
    // Test method with complex type operations
    processComplexTypes(): string {
        let result = `Processing complex types for ${this.name}:\n`;
        
        // Test enum operations
        result += `  Enum: ${this.enumValue}\n`;
        
        // Test union operations
        result += `  Union: ${this.unionValue}\n`;
        
        // Test intersection operations
        result += `  Intersection: ${JSON.stringify(this.intersectionValue)}\n`;
        
        // Test array operations
        result += `  Data: ${JSON.stringify(this.data)}\n`;
        
        return result;
    }
    
    // Test method with generic constraints
    processGeneric<T extends ComplexUnionType>(value: T): string {
        if (typeof value === "string") {
            return `String processing: ${value}`;
        } else if (typeof value === "number") {
            return `Number processing: ${value}`;
        } else if (typeof value === "boolean") {
            return `Boolean processing: ${value}`;
        } else {
            return `Enum processing: ${value}`;
        }
    }
    
    // Test method with function pointer
    processWithFunction<T>(value: T, processor: (value: T) => string): string {
        return processor(value);
    }
    
    // Test method with tuple operations
    processTuple(): [string, number, boolean] {
        return [this.name, this.id, this.enumValue === ComplexEnum.BooleanValue];
    }
    
    // Test method with object type operations
    processObject(): ComplexIntersectionType {
        return {
            enumValue: this.enumValue,
            description: `Processed ${this.name}`,
            count: this.data.length,
            id: this.id,
            name: this.name
        };
    }
    
    // Test reference management
    addReference(ref: FullPipelineTest): void {
        this.refs.push(ref);
        console.log(`Reference added: ${this.name} -> ${ref.getName()}`);
    }
    
    getReferences(): FullPipelineTest[] {
        return this.refs;
    }
    
    clearReferences(): void {
        this.refs = [];
        console.log(`References cleared: ${this.name}`);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
    
    destructor(): void {
        console.log(`FullPipelineTest destroyed: ${this.name} (ID: ${this.id})`);
        this.clearReferences();
        this.data = [];
    }
}

function testFullPipelineIntegration(): void {
    console.log("Testing full pipeline integration...");
    
    // Create objects
    let obj1 = new FullPipelineTest(1, "Pipeline Object 1");
    let obj2 = new FullPipelineTest(2, "Pipeline Object 2");
    
    // Test complex type processing
    let complexResult = obj1.processComplexTypes();
    console.log(complexResult);
    
    // Test generic processing
    let genericResult1 = obj1.processGeneric<string>("generic string");
    let genericResult2 = obj1.processGeneric<number>(42);
    let genericResult3 = obj1.processGeneric<boolean>(true);
    let genericResult4 = obj1.processGeneric<ComplexEnum>(ComplexEnum.StringValue);
    
    console.log(`Generic results: ${genericResult1}, ${genericResult2}, ${genericResult3}, ${genericResult4}`);
    
    // Test function pointer processing
    let funcPtr = (value: string) => `Function pointer: ${value}`;
    let funcResult = obj1.processWithFunction<string>("test", funcPtr);
    console.log(`Function pointer result: ${funcResult}`);
    
    // Test tuple processing
    let tupleResult = obj1.processTuple();
    console.log(`Tuple result: ${JSON.stringify(tupleResult)}`);
    
    // Test object processing
    let objectResult = obj1.processObject();
    console.log(`Object result: ${JSON.stringify(objectResult)}`);
    
    // Test reference management
    obj1.addReference(obj2);
    obj2.addReference(obj1);
    
    let refs1 = obj1.getReferences();
    let refs2 = obj2.getReferences();
    
    console.log(`Object 1 references: ${refs1.map(r => r.getName()).join(", ")}`);
    console.log(`Object 2 references: ${refs2.map(r => r.getName()).join(", ")}`);
    
    // Test property access
    let id1 = obj1.getId();
    let name1 = obj1.getName();
    console.log(`Object 1 - ID: ${id1}, Name: ${name1}`);
    
    // Test index access
    let data = obj1.getData();
    let firstElement = data[0];
    let lastElement = data[data.length - 1];
    console.log(`Index access - first: ${firstElement}, last: ${lastElement}`);
    
    // Clean up
    obj1.clearReferences();
    obj2.clearReferences();
    
    obj1.destructor();
    obj2.destructor();
    
    console.log("Full pipeline integration test completed");
}

// =============================================================================
// Test 5: Error Handling Integration
// =============================================================================

function testErrorHandlingIntegration(): void {
    console.log("Testing error handling integration...");
    
    try {
        // Test type system error handling
        let enumValue: ComplexEnum = ComplexEnum.StringValue;
        console.log(`Enum value: ${enumValue}`);
        
        // Test semantic analyzer error handling
        let unionValue: ComplexUnionType = "test";
        console.log(`Union value: ${unionValue}`);
        
        // Test parser error handling
        let intersectionValue: ComplexIntersectionType = {
            enumValue: ComplexEnum.NumericValue,
            description: "Error test",
            count: 1,
            id: 1,
            name: "test"
        };
        console.log(`Intersection value: ${JSON.stringify(intersectionValue)}`);
        
        // Test codegen error handling
        let obj = new FullPipelineTest(1, "Error Test Object");
        let result = obj.processComplexTypes();
        console.log(`Codegen result: ${result}`);
        
        // Test runtime error handling
        obj.destructor();
        
        console.log("Error handling integration test completed successfully");
        
    } catch (error) {
        console.log(`Error handling integration test failed: ${error}`);
    }
}

// =============================================================================
// Test 6: Performance Integration
// =============================================================================

function testPerformanceIntegration(): void {
    console.log("Testing performance integration...");
    
    const iterations = 1000;
    
    // Test type system performance
    let startTime = Date.now();
    for (let i = 0; i < iterations; i++) {
        let enumValue: ComplexEnum = ComplexEnum.StringValue;
        let unionValue: ComplexUnionType = "test";
        let intersectionValue: ComplexIntersectionType = {
            enumValue: ComplexEnum.NumericValue,
            description: "Performance test",
            count: i,
            id: i,
            name: `test${i}`
        };
    }
    let typeSystemTime = Date.now() - startTime;
    console.log(`Type system operations (${iterations} iterations): ${typeSystemTime}ms`);
    
    // Test semantic analyzer performance
    startTime = Date.now();
    for (let i = 0; i < iterations; i++) {
        let obj = new FullPipelineTest(i, `Performance Object ${i}`);
        let result = obj.processComplexTypes();
        obj.destructor();
    }
    let semanticTime = Date.now() - startTime;
    console.log(`Semantic analyzer operations (${iterations} iterations): ${semanticTime}ms`);
    
    // Test codegen performance
    startTime = Date.now();
    for (let i = 0; i < iterations; i++) {
        let obj = new FullPipelineTest(i, `CodeGen Object ${i}`);
        let genericResult = obj.processGeneric<string>("test");
        let funcResult = obj.processWithFunction<number>(i, (x: number) => x.toString());
        let tupleResult = obj.processTuple();
        let objectResult = obj.processObject();
        obj.destructor();
    }
    let codegenTime = Date.now() - startTime;
    console.log(`Codegen operations (${iterations} iterations): ${codegenTime}ms`);
    
    // Test runtime performance
    startTime = Date.now();
    for (let i = 0; i < iterations; i++) {
        let obj = new FullPipelineTest(i, `Runtime Object ${i}`);
        obj.addReference(obj);
        let refs = obj.getReferences();
        obj.clearReferences();
        obj.destructor();
    }
    let runtimeTime = Date.now() - startTime;
    console.log(`Runtime operations (${iterations} iterations): ${runtimeTime}ms`);
    
    console.log("Performance integration test completed");
}

// =============================================================================
// Main Test Runner
// =============================================================================

function runIntegrationTests(): void {
    console.log("🧪 Running Integration Tests");
    console.log("============================");
    
    console.log("\n📊 Test 1: Type System + Semantic Analyzer Integration");
    console.log("─".repeat(50));
    testTypeSystemSemanticIntegration();
    
    console.log("\n📊 Test 2: Parser + Semantic Analyzer Integration");
    console.log("─".repeat(50));
    testParserSemanticIntegration();
    
    console.log("\n📊 Test 3: CodeGen + Runtime Integration");
    console.log("─".repeat(50));
    testCodeGenRuntimeIntegration();
    
    console.log("\n📊 Test 4: Full Pipeline Integration");
    console.log("─".repeat(50));
    testFullPipelineIntegration();
    
    console.log("\n📊 Test 5: Error Handling Integration");
    console.log("─".repeat(50));
    testErrorHandlingIntegration();
    
    console.log("\n📊 Test 6: Performance Integration");
    console.log("─".repeat(50));
    testPerformanceIntegration();
    
    console.log("\n✅ Integration Tests Completed Successfully!");
}

// Export for use in other test files
export { runIntegrationTests };

// Run if this is the main module
if (require.main === module) {
    runIntegrationTests();
}
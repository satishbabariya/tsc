// Language Feature Interaction System Tests
// Tests how generic constraints interact with other TypeScript language features

// Test 1: Generic constraints with union types
function processUnion<T extends string | number>(value: T): T {
    return value;
}

// Test 2: Generic constraints with intersection types
interface A {
    propA: string;
}

interface B {
    propB: number;
}

function processIntersection<T extends A & B>(item: T): string {
    return `${item.propA}: ${item.propB}`;
}

// Test 3: Generic constraints with conditional types (if supported)
type ConditionalType<T> = T extends string ? number : string;

function processConditional<T extends string>(value: T): ConditionalType<T> {
    return value.length as ConditionalType<T>;
}

// Test 4: Generic constraints with mapped types (if supported)
type MappedType<T> = {
    [K in keyof T]: T[K];
};

function processMapped<T extends { id: number; name: string }>(item: T): MappedType<T> {
    return item;
}

// Test 5: Generic constraints with template literal types (if supported)
type TemplateLiteral<T extends string> = `prefix_${T}`;

function processTemplate<T extends string>(value: T): TemplateLiteral<T> {
    return `prefix_${value}` as TemplateLiteral<T>;
}

// Test 6: Generic constraints with function overloads
function processOverload<T extends string>(value: T): T;
function processOverload<T extends number>(value: T): T;
function processOverload<T extends string | number>(value: T): T {
    return value;
}

// Test 7: Generic constraints with async functions
async function processAsync<T extends string>(value: T): Promise<T> {
    return Promise.resolve(value);
}

// Test 8: Generic constraints with generator functions
function* processGenerator<T extends number>(value: T): Generator<T, void, unknown> {
    yield value;
    yield value * 2;
    yield value * 3;
}

// Test 9: Generic constraints with decorators (if supported)
function logMethod(target: any, propertyKey: string, descriptor: PropertyDescriptor) {
    const originalMethod = descriptor.value;
    descriptor.value = function (...args: any[]) {
        console.log(`Calling ${propertyKey} with args:`, args);
        return originalMethod.apply(this, args);
    };
}

class DecoratedClass {
    @logMethod
    process<T extends string>(value: T): T {
        return value;
    }
}

// Test 10: Generic constraints with modules and namespaces
namespace GenericNamespace {
    export function process<T extends string>(value: T): T {
        return value;
    }
    
    export class GenericClass<T extends number> {
        constructor(private value: T) {}
        
        getValue(): T {
            return this.value;
        }
    }
}

// Test 11: Generic constraints with enums
enum Status {
    ACTIVE = "active",
    INACTIVE = "inactive",
    PENDING = "pending"
}

function processEnum<T extends Status>(status: T): T {
    return status;
}

// Test 12: Generic constraints with readonly properties
interface ReadonlyConstraint {
    readonly id: number;
    readonly name: string;
}

function processReadonly<T extends ReadonlyConstraint>(item: T): T {
    return item;
}

// Test data
const unionValue: string | number = "test";
const intersectionValue: A & B = {
    propA: "hello",
    propB: 42
};

const mappedValue = {
    id: 1,
    name: "test"
};

const readonlyValue: ReadonlyConstraint = {
    id: 123,
    name: "readonly test"
};

// Test functions
function testLanguageInteractions(): void {
    console.log("=== Language Feature Interaction Tests ===");
    
    // Union type constraints
    let unionResult = processUnion<string | number>(unionValue);
    console.log(`Union result: ${unionResult}`);
    
    // Intersection type constraints
    let intersectionResult = processIntersection<A & B>(intersectionValue);
    console.log(`Intersection result: ${intersectionResult}`);
    
    // Conditional type constraints
    let conditionalResult = processConditional<string>("hello");
    console.log(`Conditional result: ${conditionalResult}`);
    
    // Mapped type constraints
    let mappedResult = processMapped<{ id: number; name: string }>(mappedValue);
    console.log(`Mapped result: ${mappedResult.id}, ${mappedResult.name}`);
    
    // Template literal constraints
    let templateResult = processTemplate<string>("test");
    console.log(`Template result: ${templateResult}`);
    
    // Function overload constraints
    let overloadStrResult = processOverload<string>("overload");
    let overloadNumResult = processOverload<number>(42);
    console.log(`Overload string result: ${overloadStrResult}`);
    console.log(`Overload number result: ${overloadNumResult}`);
    
    // Async function constraints
    processAsync<string>("async").then(result => {
        console.log(`Async result: ${result}`);
    });
    
    // Generator function constraints
    let generator = processGenerator<number>(5);
    let generatorResults: number[] = [];
    for (const value of generator) {
        generatorResults.push(value);
    }
    console.log(`Generator results: ${generatorResults.join(", ")}`);
    
    // Decorated class constraints
    let decorated = new DecoratedClass();
    let decoratedResult = decorated.process<string>("decorated");
    console.log(`Decorated result: ${decoratedResult}`);
    
    // Namespace constraints
    let namespaceResult = GenericNamespace.process<string>("namespace");
    console.log(`Namespace result: ${namespaceResult}`);
    
    let namespaceClass = new GenericNamespace.GenericClass<number>(99);
    let namespaceClassResult = namespaceClass.getValue();
    console.log(`Namespace class result: ${namespaceClassResult}`);
    
    // Enum constraints
    let enumResult = processEnum<Status>(Status.ACTIVE);
    console.log(`Enum result: ${enumResult}`);
    
    // Readonly constraints
    let readonlyResult = processReadonly<ReadonlyConstraint>(readonlyValue);
    console.log(`Readonly result: ${readonlyResult.id}, ${readonlyResult.name}`);
}

// Test constraint violations (should fail compilation)
function testLanguageInteractionViolations(): void {
    console.log("=== Language Feature Interaction Violation Tests (Expected Failures) ===");
    
    // These should cause compilation errors:
    // let invalidUnion = processUnion<boolean>(true);                    // boolean not assignable to string | number
    // let invalidIntersection = processIntersection<A>({ propA: "test" }); // A doesn't extend A & B
    // let invalidConditional = processConditional<number>(42);          // number doesn't extend string
    // let invalidMapped = processMapped<{ id: string }>({ id: "test" }); // wrong property type
    // let invalidTemplate = processTemplate<number>(123);                // number doesn't extend string
    // let invalidOverload = processOverload<boolean>(true);             // boolean not assignable to string | number
    // let invalidAsync = processAsync<number>(42);                      // number doesn't extend string
    // let invalidGenerator = processGenerator<string>("test");          // string doesn't extend number
    // let invalidNamespace = GenericNamespace.process<number>(123);      // number doesn't extend string
    // let invalidNamespaceClass = new GenericNamespace.GenericClass<string>("test"); // string doesn't extend number
    // let invalidEnum = processEnum<string>("invalid");                 // string not assignable to Status
    // let invalidReadonly = processReadonly<string>("test");            // string doesn't extend ReadonlyConstraint
    
    console.log("Language feature interaction violation tests are commented out as they should fail compilation");
}

function main(): void {
    testLanguageInteractions();
    testLanguageInteractionViolations();
}

main();
// Integration Tests for Monomorphization with Type Checking
// Tests the interaction between monomorphization and semantic analysis

// =============================================================================
// Test 1: Type Information Propagation
// =============================================================================

interface TypeInfo<T> {
    type: string;
    value: T;
}

class TypePropagationTest<T> {
    private typeInfo: TypeInfo<T>;
    
    constructor(value: T) {
        this.typeInfo = {
            type: typeof value,
            value: value
        };
    }
    
    getTypeInfo(): TypeInfo<T> {
        return this.typeInfo;
    }
    
    // Method that preserves type information
    transform<U>(fn: (value: T) => U): TypeInfo<U> {
        const transformedValue = fn(this.typeInfo.value);
        return {
            type: typeof transformedValue,
            value: transformedValue
        };
    }
}

// Test type information propagation
let numTypeTest = new TypePropagationTest<number>(42);
let strTypeTest = new TypePropagationTest<string>("hello");

let numTypeInfo = numTypeTest.getTypeInfo();
let strTypeInfo = strTypeTest.getTypeInfo();

let transformedNumInfo = numTypeTest.transform<string>(x => x.toString());
let transformedStrInfo = strTypeTest.transform<number>(x => x.length);

// =============================================================================
// Test 2: Constraint Validation Integration
// =============================================================================

interface ConstraintValidator<T extends string | number> {
    validate(value: T): boolean;
    process(value: T): T;
}

class StringValidator implements ConstraintValidator<string> {
    validate(value: string): boolean {
        return value.length > 0 && /^[a-zA-Z]+$/.test(value);
    }
    
    process(value: string): string {
        return value.toLowerCase();
    }
}

class NumberValidator implements ConstraintValidator<number> {
    validate(value: number): boolean {
        return value > 0 && Number.isInteger(value);
    }
    
    process(value: number): number {
        return value * 2;
    }
}

// Test constraint validation integration
let strValidator = new StringValidator();
let numValidator = new NumberValidator();

let validStr = strValidator.validate("Hello");
let validNum = numValidator.validate(42);

let processedStr = strValidator.process("HELLO");
let processedNum = numValidator.process(21);

// =============================================================================
// Test 3: Generic Method Overloading
// =============================================================================

class OverloadTest {
    // Generic method with different signatures
    static process<T>(value: T): string {
        return `Generic: ${value}`;
    }
    
    static process(value: number): string {
        return `Number: ${value}`;
    }
    
    static process(value: string): string {
        return `String: ${value}`;
    }
    
    static process(value: boolean): string {
        return `Boolean: ${value}`;
    }
    
    // Generic method with constraints
    static constrained<T extends { length: number }>(value: T): number {
        return value.length;
    }
    
    // Generic method with multiple parameters
    static multiParam<T, U>(first: T, second: U): [T, U] {
        return [first, second];
    }
}

// Test method overloading with generics
let genericOverload = OverloadTest.process<boolean>(true);
let numberOverload = OverloadTest.process(42);
let stringOverload = OverloadTest.process("test");
let booleanOverload = OverloadTest.process(true);

let constrainedOverload = OverloadTest.constrained<string>("hello");
let multiParamOverload = OverloadTest.multiParam<number, string>(1, "a");

// =============================================================================
// Test 4: Generic Inheritance with Type Checking
// =============================================================================

abstract class AbstractGeneric<T> {
    protected value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    abstract process(): T;
    
    getValue(): T {
        return this.value;
    }
    
    // Generic method in abstract class
    transform<U>(fn: (value: T) => U): U {
        return fn(this.value);
    }
}

class ConcreteGeneric<T> extends AbstractGeneric<T> {
    constructor(value: T) {
        super(value);
    }
    
    process(): T {
        return this.value;
    }
    
    // Additional method
    duplicate(): [T, T] {
        return [this.value, this.value];
    }
}

// Test generic inheritance
let concreteNum = new ConcreteGeneric<number>(42);
let concreteStr = new ConcreteGeneric<string>("hello");

let processedConcreteNum = concreteNum.process();
let processedConcreteStr = concreteStr.process();

let duplicatedNum = concreteNum.duplicate();
let duplicatedStr = concreteStr.duplicate();

let transformedConcreteNum = concreteNum.transform<string>(x => x.toString());
let transformedConcreteStr = concreteStr.transform<number>(x => x.length);

// =============================================================================
// Test 5: Generic Type Aliases Integration
// =============================================================================

type GenericResult<T, E = string> = 
    | { success: true; data: T }
    | { success: false; error: E };

type GenericOptional<T> = T | null | undefined;

class ResultProcessor<T, E = string> {
    static success<T, E = string>(data: T): GenericResult<T, E> {
        return { success: true, data };
    }
    
    static error<T, E = string>(error: E): GenericResult<T, E> {
        return { success: false, error };
    }
    
    static process<T, E = string>(
        value: GenericOptional<T>,
        errorMessage: E
    ): GenericResult<T, E> {
        if (value === null || value === undefined) {
            return { success: false, error: errorMessage };
        }
        return { success: true, data: value };
    }
}

// Test generic type aliases
let successResult = ResultProcessor.success<number>(42);
let errorResult = ResultProcessor.error<string>("Something went wrong");

let processedResult = ResultProcessor.process<number, string>(42, "No value");
let nullResult = ResultProcessor.process<number, string>(null, "No value");

// =============================================================================
// Test 6: Generic Union Types
// =============================================================================

type GenericUnion<T, U> = T | U;

class UnionProcessor<T, U> {
    static process<T, U>(value: GenericUnion<T, U>): string {
        if (typeof value === "string") {
            return `String: ${value}`;
        } else if (typeof value === "number") {
            return `Number: ${value}`;
        } else if (typeof value === "boolean") {
            return `Boolean: ${value}`;
        } else {
            return `Object: ${JSON.stringify(value)}`;
        }
    }
    
    static isFirstType<T, U>(value: GenericUnion<T, U>): value is T {
        return typeof value === "string";
    }
}

// Test generic union types
let unionStr: GenericUnion<string, number> = "hello";
let unionNum: GenericUnion<string, number> = 42;

let processedUnionStr = UnionProcessor.process<string, number>(unionStr);
let processedUnionNum = UnionProcessor.process<string, number>(unionNum);

let isString = UnionProcessor.isFirstType<string, number>(unionStr);
let isStringNum = UnionProcessor.isFirstType<string, number>(unionNum);

// =============================================================================
// Test 7: Generic Intersection Types
// =============================================================================

interface Nameable {
    name: string;
}

interface Countable {
    count: number;
}

type GenericIntersection<T, U> = T & U;

class IntersectionProcessor<T, U> {
    static process<T extends Nameable, U extends Countable>(
        value: GenericIntersection<T, U>
    ): string {
        return `${value.name}: ${value.count}`;
    }
    
    static create<T, U>(t: T, u: U): GenericIntersection<T, U> {
        return { ...t, ...u } as GenericIntersection<T, U>;
    }
}

// Test generic intersection types
let intersection = IntersectionProcessor.create<Nameable, Countable>(
    { name: "test" },
    { count: 42 }
);

let processedIntersection = IntersectionProcessor.process<Nameable, Countable>(intersection);

// =============================================================================
// Test 8: Generic Recursive Types
// =============================================================================

interface GenericNode<T> {
    value: T;
    children: GenericNode<T>[];
}

class TreeProcessor<T> {
    static createNode<T>(value: T): GenericNode<T> {
        return {
            value,
            children: []
        };
    }
    
    static addChild<T>(node: GenericNode<T>, child: GenericNode<T>): void {
        node.children.push(child);
    }
    
    static findValue<T>(node: GenericNode<T>, target: T): GenericNode<T> | null {
        if (node.value === target) {
            return node;
        }
        
        for (const child of node.children) {
            const found = TreeProcessor.findValue(child, target);
            if (found) {
                return found;
            }
        }
        
        return null;
    }
    
    static map<T, U>(node: GenericNode<T>, fn: (value: T) => U): GenericNode<U> {
        return {
            value: fn(node.value),
            children: node.children.map(child => TreeProcessor.map(child, fn))
        };
    }
}

// Test generic recursive types
let rootNode = TreeProcessor.createNode<number>(1);
let childNode1 = TreeProcessor.createNode<number>(2);
let childNode2 = TreeProcessor.createNode<number>(3);
let grandchildNode = TreeProcessor.createNode<number>(4);

TreeProcessor.addChild(rootNode, childNode1);
TreeProcessor.addChild(rootNode, childNode2);
TreeProcessor.addChild(childNode1, grandchildNode);

let foundNode = TreeProcessor.findValue(rootNode, 4);
let mappedTree = TreeProcessor.map(rootNode, (x: number) => x.toString());

// =============================================================================
// Test 9: Generic Error Handling
// =============================================================================

type GenericError<T, E = Error> = {
    success: false;
    error: E;
    context?: T;
};

type GenericSuccess<T> = {
    success: true;
    data: T;
};

type GenericResult2<T, E = Error> = GenericSuccess<T> | GenericError<T, E>;

class ErrorProcessor<T, E = Error> {
    static success<T>(data: T): GenericSuccess<T> {
        return { success: true, data };
    }
    
    static error<T, E = Error>(error: E, context?: T): GenericError<T, E> {
        return { success: false, error, context };
    }
    
    static tryProcess<T, E = Error>(
        fn: () => T,
        errorHandler: (error: E) => GenericError<T, E>
    ): GenericResult2<T, E> {
        try {
            const result = fn();
            return { success: true, data: result };
        } catch (error) {
            return errorHandler(error as E);
        }
    }
}

// Test generic error handling
let successResult2 = ErrorProcessor.success<number>(42);
let errorResult2 = ErrorProcessor.error<number, string>("Something went wrong", 42);

let tryResult = ErrorProcessor.tryProcess<number, string>(
    () => 42,
    (error) => ErrorProcessor.error<number, string>(error)
);

// =============================================================================
// Test 10: Generic Performance Testing
// =============================================================================

class PerformanceTest<T> {
    private data: T[];
    
    constructor(data: T[]) {
        this.data = data;
    }
    
    // Generic method that should be optimized by monomorphization
    map<U>(fn: (value: T) => U): U[] {
        const result: U[] = [];
        for (const item of this.data) {
            result.push(fn(item));
        }
        return result;
    }
    
    // Generic method with filtering
    filter(predicate: (value: T) => boolean): T[] {
        const result: T[] = [];
        for (const item of this.data) {
            if (predicate(item)) {
                result.push(item);
            }
        }
        return result;
    }
    
    // Generic method with reduction
    reduce<U>(fn: (acc: U, value: T) => U, initial: U): U {
        let result = initial;
        for (const item of this.data) {
            result = fn(result, item);
        }
        return result;
    }
}

// Test performance with different types
let numPerformanceTest = new PerformanceTest<number>([1, 2, 3, 4, 5]);
let strPerformanceTest = new PerformanceTest<string>(["a", "b", "c", "d", "e"]);

let mappedNums = numPerformanceTest.map<string>(x => x.toString());
let mappedStrs = strPerformanceTest.map<number>(x => x.length);

let filteredNums = numPerformanceTest.filter(x => x > 2);
let filteredStrs = strPerformanceTest.filter(x => x.length > 1);

let reducedNums = numPerformanceTest.reduce<number>((acc, x) => acc + x, 0);
let reducedStrs = strPerformanceTest.reduce<string>((acc, x) => acc + x, "");

// =============================================================================
// Main execution
// =============================================================================

function main(): void {
    console.log("Monomorphization integration tests");
    console.log("All integration tests completed successfully!");
}

main();
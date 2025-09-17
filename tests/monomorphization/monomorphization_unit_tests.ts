// Unit Tests for Monomorphization Implementation
// Tests specific aspects of template generation and specialization

// =============================================================================
// Test 1: Template Generation
// =============================================================================

class TemplateTest<T> {
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
    
    // Generic method within generic class
    transform<U>(fn: (value: T) => U): U {
        return fn(this.value);
    }
}

// Test template generation for different types
let templateNum = new TemplateTest<number>(42);
let templateStr = new TemplateTest<string>("hello");
let templateBool = new TemplateTest<boolean>(true);

// Verify template methods work correctly
let numValue = templateNum.getValue();
let strValue = templateStr.getValue();
let boolValue = templateBool.getValue();

let transformedNum = templateNum.transform<string>(x => x.toString());
let transformedStr = templateStr.transform<number>(x => x.length);

// =============================================================================
// Test 2: Specialization Logic
// =============================================================================

interface SpecializedInterface<T> {
    process(input: T): T;
    validate(input: T): boolean;
}

class NumberProcessor implements SpecializedInterface<number> {
    process(input: number): number {
        return input * 2;
    }
    
    validate(input: number): boolean {
        return input > 0;
    }
}

class StringProcessor implements SpecializedInterface<string> {
    process(input: string): string {
        return input.toUpperCase();
    }
    
    validate(input: string): boolean {
        return input.length > 0;
    }
}

// Test specialization for different types
let numProcessor = new NumberProcessor();
let strProcessor = new StringProcessor();

let processedNum = numProcessor.process(21);
let processedStr = strProcessor.process("test");

let validNum = numProcessor.validate(5);
let validStr = strProcessor.validate("hello");

// =============================================================================
// Test 3: Type Parameter Substitution
// =============================================================================

class TypeSubstitutionTest<T, U> {
    private first: T;
    private second: U;
    
    constructor(first: T, second: U) {
        this.first = first;
        this.second = second;
    }
    
    getFirst(): T {
        return this.first;
    }
    
    getSecond(): U {
        return this.second;
    }
    
    swap(): TypeSubstitutionTest<U, T> {
        return new TypeSubstitutionTest<U, T>(this.second, this.first);
    }
    
    // Nested generic method
    combine<V>(fn: (t: T, u: U) => V): V {
        return fn(this.first, this.second);
    }
}

// Test type parameter substitution
let substitutionTest = new TypeSubstitutionTest<number, string>(42, "hello");
let swapped = substitutionTest.swap();
let combined = substitutionTest.combine<string>((n, s) => `${n}:${s}`);

// =============================================================================
// Test 4: Generic Method Resolution
// =============================================================================

class MethodResolutionTest {
    // Overloaded generic methods
    static process<T>(value: T): string {
        return `Generic: ${value}`;
    }
    
    static process(value: number): string {
        return `Number: ${value}`;
    }
    
    static process(value: string): string {
        return `String: ${value}`;
    }
    
    // Generic method with constraints
    static constrained<T extends { length: number }>(value: T): number {
        return value.length;
    }
    
    // Generic method with multiple type parameters
    static multiParam<T, U>(first: T, second: U): [T, U] {
        return [first, second];
    }
}

// Test method resolution
let genericResult = MethodResolutionTest.process<boolean>(true);
let numberResult = MethodResolutionTest.process(42);
let stringResult = MethodResolutionTest.process("test");

let constrainedResult = MethodResolutionTest.constrained<string>("hello");
let multiParamResult = MethodResolutionTest.multiParam<number, string>(1, "a");

// =============================================================================
// Test 5: Generic Inheritance
// =============================================================================

class BaseGeneric<T> {
    protected value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
    
    // Virtual method to be overridden
    process(): T {
        return this.value;
    }
}

class DerivedGeneric<T> extends BaseGeneric<T> {
    constructor(value: T) {
        super(value);
    }
    
    // Override with specialization
    process(): T {
        return this.value;
    }
    
    // Additional method
    transform<U>(fn: (value: T) => U): U {
        return fn(this.value);
    }
}

// Test generic inheritance
let baseNum = new BaseGeneric<number>(10);
let derivedNum = new DerivedGeneric<number>(20);
let derivedStr = new DerivedGeneric<string>("test");

let baseValue = baseNum.getValue();
let derivedValue = derivedNum.getValue();
let derivedStrValue = derivedStr.getValue();

let processedBase = baseNum.process();
let processedDerived = derivedNum.process();
let processedDerivedStr = derivedStr.process();

let transformed = derivedNum.transform<string>(x => x.toString());

// =============================================================================
// Test 6: Generic Constraints and Bounds
// =============================================================================

interface ConstraintTest<T extends number | string> {
    value: T;
    process(): T;
}

class NumberConstraint implements ConstraintTest<number> {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    process(): number {
        return this.value * 2;
    }
}

class StringConstraint implements ConstraintTest<string> {
    value: string;
    
    constructor(value: string) {
        this.value = value;
    }
    
    process(): string {
        return this.value.toUpperCase();
    }
}

// Test constraints
let numConstraint = new NumberConstraint(5);
let strConstraint = new StringConstraint("hello");

let processedNumConstraint = numConstraint.process();
let processedStrConstraint = strConstraint.process();

// =============================================================================
// Test 7: Generic Default Parameters
// =============================================================================

class DefaultParamTest<T = string> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
    
    // Method with default type parameter
    createArray<U = T>(size: number): U[] {
        const result: U[] = [];
        for (let i = 0; i < size; i++) {
            // This is a simplified version - in reality we'd need a way to create U
            result.push(this.value as any);
        }
        return result;
    }
}

// Test default parameters
let defaultTest = new DefaultParamTest("default");
let explicitTest = new DefaultParamTest<number>(42);

let defaultValue = defaultTest.getValue();
let explicitValue = explicitTest.getValue();

let defaultArray = defaultTest.createArray<string>(3);
let explicitArray = explicitTest.createArray<number>(3);

// =============================================================================
// Test 8: Generic Variance
// =============================================================================

interface Covariant<T> {
    get(): T;
}

interface Contravariant<T> {
    set(value: T): void;
}

interface Invariant<T> {
    get(): T;
    set(value: T): void;
}

class CovariantImpl<T> implements Covariant<T> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    get(): T {
        return this.value;
    }
}

class ContravariantImpl<T> implements Contravariant<T> {
    private value: T | undefined;
    
    set(value: T): void {
        this.value = value;
    }
}

class InvariantImpl<T> implements Invariant<T> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    get(): T {
        return this.value;
    }
    
    set(value: T): void {
        this.value = value;
    }
}

// Test variance
let covariantNum = new CovariantImpl<number>(42);
let contravariantNum = new ContravariantImpl<number>();
let invariantNum = new InvariantImpl<number>(42);

let covariantValue = covariantNum.get();
contravariantNum.set(100);
let invariantValue = invariantNum.get();
invariantNum.set(200);

// =============================================================================
// Test 9: Generic Recursion
// =============================================================================

class RecursiveGeneric<T> {
    private value: T;
    private children: RecursiveGeneric<T>[] = [];
    
    constructor(value: T) {
        this.value = value;
    }
    
    addChild(child: RecursiveGeneric<T>): void {
        this.children.push(child);
    }
    
    getValue(): T {
        return this.value;
    }
    
    getChildren(): RecursiveGeneric<T>[] {
        return this.children;
    }
    
    // Recursive method
    findValue(target: T): RecursiveGeneric<T> | null {
        if (this.value === target) {
            return this;
        }
        
        for (const child of this.children) {
            const found = child.findValue(target);
            if (found) {
                return found;
            }
        }
        
        return null;
    }
}

// Test recursive generics
let root = new RecursiveGeneric<number>(1);
let child1 = new RecursiveGeneric<number>(2);
let child2 = new RecursiveGeneric<number>(3);
let grandchild = new RecursiveGeneric<number>(4);

root.addChild(child1);
root.addChild(child2);
child1.addChild(grandchild);

let found = root.findValue(4);
let notFound = root.findValue(5);

// =============================================================================
// Test 10: Generic Error Handling
// =============================================================================

class GenericErrorTest<T> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // Method that might throw
    riskyOperation<U>(fn: (value: T) => U): U | null {
        try {
            return fn(this.value);
        } catch (error) {
            return null;
        }
    }
    
    // Method with error handling
    safeTransform<U>(fn: (value: T) => U, defaultValue: U): U {
        try {
            return fn(this.value);
        } catch (error) {
            return defaultValue;
        }
    }
}

// Test error handling in generics
let errorTest = new GenericErrorTest<number>(42);

let riskyResult = errorTest.riskyOperation<string>(x => x.toString());
let safeResult = errorTest.safeTransform<string>(x => x.toString(), "error");

// =============================================================================
// Main execution
// =============================================================================

function main(): void {
    console.log("Monomorphization unit tests");
    console.log("All unit tests completed successfully!");
}

main();
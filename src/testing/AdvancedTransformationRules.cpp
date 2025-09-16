#include "tsc/testing/CodeTransformationFramework.h"
#include <random>
#include <sstream>
#include <algorithm>
#include <regex>

namespace tsc {
namespace testing {

// =============================================================================
// ADVANCED TRANSFORMATION RULES
// =============================================================================

// Nested objects with spread operations and conflict resolution
class NestedObjectSpreadRule : public TransformationRule {
public:
    String getName() const override { return "NestedObjectSpread"; }
    TransformationType getType() const override { return TransformationType::ComplexExpressions; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add nested objects
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate complex nested objects with spread operations
        String nestedObject = R"(
// Complex nested objects with spread operations
function createComplexObject(): any {
    let baseObject = {
        id: 1,
        name: "base",
        properties: {
            color: "red",
            size: "large",
            dimensions: {
                width: 100,
                height: 200,
                depth: 50
            }
        },
        metadata: {
            created: "2024-01-01",
            updated: "2024-01-02",
            tags: ["important", "urgent", "critical"]
        }
    };
    
    let extendedObject = {
        ...baseObject,
        ...baseObject.properties,
        ...baseObject.properties.dimensions,
        ...baseObject.metadata,
        additional: {
            ...baseObject.properties,
            ...baseObject.metadata,
            nested: {
                ...baseObject.properties.dimensions,
                ...baseObject.metadata.tags,
                conflict: "resolved"
            }
        },
        conflicts: {
            ...baseObject.properties,
            ...baseObject.metadata,
            name: "conflict", // This should conflict with baseObject.name
            color: "blue",    // This should conflict with baseObject.properties.color
            width: 999        // This should conflict with baseObject.properties.dimensions.width
        }
    };
    
    return extendedObject;
}

// Deep nested object with multiple spread levels
function createDeepNestedObject(): any {
    let level1 = {
        a: 1,
        b: { c: 2, d: 3 },
        e: { f: { g: 4, h: 5 }, i: 6 }
    };
    
    let level2 = {
        ...level1,
        ...level1.b,
        ...level1.e,
        ...level1.e.f,
        nested: {
            ...level1,
            ...level1.b,
            ...level1.e,
            ...level1.e.f,
            deeper: {
                ...level1,
                ...level1.b,
                ...level1.e,
                ...level1.e.f,
                deepest: {
                    ...level1,
                    ...level1.b,
                    ...level1.e,
                    ...level1.e.f
                }
            }
        }
    };
    
    return level2;
}
)";
        
        result.insert(pos, nestedObject);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates complex nested objects with spread operations and conflict resolution"; 
    }
};

// Functions with nested control statements
class NestedControlFunctionRule : public TransformationRule {
public:
    String getName() const override { return "NestedControlFunction"; }
    TransformationType getType() const override { return TransformationType::DeepNesting; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add nested control functions
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate functions with complex nested control statements
        String nestedControlFunction = R"(
// Complex function with nested control statements
function complexNestedFunction(x: number, y: string, z: boolean): string {
    let result = " + ";
    
    // Level 1: if-else
    if (x > 0) {
        // Level 2: switch
        switch (y.length) {
            case 1:
                // Level 3: for loop
                for (let i = 0; i < x; i++) {
                    // Level 4: while loop
                    while (i < 10) {
                        // Level 5: if-else
                        if (z) {
                            // Level 6: do-while
                            do {
                                // Level 7: nested switch
                                switch (i % 3) {
                                    case 0:
                                        // Level 8: for loop
                                        for (let j = 0; j < 5; j++) {
                                            // Level 9: if-else
                                            if (j % 2 === 0) {
                                                // Level 10: try-catch
                                                try {
                                                    result += `Level10: ${i}-${j}`;
                                                } catch (error) {
                                                    result += "Error";
                                                }
                                            } else {
                                                result += `Odd: ${j}`;
                                            }
                                        }
                                        break;
                                    case 1:
                                        result += "One";
                                        break;
                                    case 2:
                                        result += "Two";
                                        break;
                                }
                                i++;
                            } while (i < 5);
                        } else {
                            result += "False";
                        }
                    }
                }
                break;
            case 2:
                // Level 2: while loop
                while (x > 0) {
                    // Level 3: for loop
                    for (let k = 0; k < y.length; k++) {
                        // Level 4: if-else
                        if (k % 2 === 0) {
                            // Level 5: switch
                            switch (k) {
                                case 0:
                                    result += "Zero";
                                    break;
                                case 2:
                                    result += "Two";
                                    break;
                                default:
                                    result += "Default";
                            }
                        }
                    }
                    x--;
                }
                break;
            default:
                // Level 2: do-while
                do {
                    // Level 3: for loop
                    for (let m = 0; m < 3; m++) {
                        // Level 4: while loop
                        while (m < 2) {
                            // Level 5: if-else
                            if (m === 0) {
                                result += "Start";
                            } else {
                                result += "Middle";
                            }
                            m++;
                        }
                    }
                    x++;
                } while (x < 10);
        }
    } else {
        // Level 1: else - switch
        switch (y) {
            case "negative":
                // Level 2: for loop
                for (let n = 0; n < Math.abs(x); n++) {
                    // Level 3: while loop
                    while (n < 5) {
                        // Level 4: if-else
                        if (n % 2 === 0) {
                            result += "Even";
                        } else {
                            result += "Odd";
                        }
                        n++;
                    }
                }
                break;
            default:
                // Level 2: while loop
                while (x < 0) {
                    // Level 3: for loop
                    for (let p = 0; p < 3; p++) {
                        result += "Negative";
                    }
                    x++;
                }
        }
    }
    
    return result;
}

// Function with complex control flow and error handling
function complexControlFlowWithErrors(): void {
    let data = [1, 2, 3, 4, 5];
    let processedData = [];
    
    try {
        // Complex nested loops with error conditions
        for (let i = 0; i < data.length; i++) {
            if (data[i] > 0) {
                while (data[i] > 0) {
                    try {
                        if (data[i] % 2 === 0) {
                            for (let j = 0; j < data[i]; j++) {
                                if (j === 0) {
                                    throw new Error("Zero division");
                                }
                                processedData.push(data[i] / j);
                            }
                        } else {
                            switch (data[i]) {
                                case 1:
                                    processedData.push(1);
                                    break;
                                case 3:
                                    processedData.push(9);
                                    break;
                                case 5:
                                    processedData.push(25);
                                    break;
                                default:
                                    throw new Error("Unexpected value");
                            }
                        }
                        data[i]--;
                    } catch (innerError) {
                        processedData.push(-1);
                    }
                }
            }
        }
    } catch (outerError) {
        processedData = [-1];
    }
    
    console.log("Processed data:", processedData);
}
)";
        
        result.insert(pos, nestedControlFunction);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates functions with complex nested control statements"; 
    }
};

// Type parameter transformations with incorrect types
class TypeParameterTransformationRule : public TransformationRule {
public:
    String getName() const override { return "TypeParameterTransformation"; }
    TransformationType getType() const override { return TransformationType::AddTypeMismatch; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add type parameter transformations
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate functions with incorrect type parameters
        String typeParameterTransformation = R"(
// Functions with incorrect type parameters
function processNumbers(numbers: number[]): number[] {
    return numbers.map(n => n * 2);
}

function processStrings(strings: string[]): string[] {
    return strings.map(s => s.toUpperCase());
}

function processBooleans(booleans: boolean[]): boolean[] {
    return booleans.map(b => !b);
}

// Type mismatches in function calls
function testTypeMismatches(): void {
    // Incorrect parameter types
    let numbers = [1, 2, 3, 4, 5];
    let strings = ["hello", "world", "test"];
    let booleans = [true, false, true];
    
    // Type mismatches
    let result1 = processNumbers(strings); // string[] passed to number[] parameter
    let result2 = processStrings(numbers); // number[] passed to string[] parameter
    let result3 = processBooleans(strings); // string[] passed to boolean[] parameter
    
    // Mixed type arrays
    let mixedArray = [1, "hello", true, 42, "world"];
    let result4 = processNumbers(mixedArray); // mixed array passed to number[] parameter
    let result5 = processStrings(mixedArray); // mixed array passed to string[] parameter
    let result6 = processBooleans(mixedArray); // mixed array passed to boolean[] parameter
}

// Generic functions with type mismatches
function genericProcess<T>(items: T[], processor: (item: T) => T): T[] {
    return items.map(processor);
}

function testGenericTypeMismatches(): void {
    let numbers = [1, 2, 3];
    let strings = ["a", "b", "c"];
    
    // Type mismatches in generic functions
    let result1 = genericProcess(numbers, (n: string) => n.toUpperCase()); // number[] with string processor
    let result2 = genericProcess(strings, (s: number) => s * 2); // string[] with number processor
    let result3 = genericProcess(numbers, (n: boolean) => !n); // number[] with boolean processor
}

// Complex type mismatches with nested structures
interface User {
    id: number;
    name: string;
    email: string;
    active: boolean;
}

interface Product {
    id: number;
    name: string;
    price: number;
    inStock: boolean;
}

function processUsers(users: User[]): User[] {
    return users.filter(user => user.active);
}

function processProducts(products: Product[]): Product[] {
    return products.filter(product => product.inStock);
}

function testComplexTypeMismatches(): void {
    let users: User[] = [
        { id: 1, name: "John", email: "john@example.com", active: true },
        { id: 2, name: "Jane", email: "jane@example.com", active: false }
    ];
    
    let products: Product[] = [
        { id: 1, name: "Laptop", price: 999, inStock: true },
        { id: 2, name: "Mouse", price: 25, inStock: false }
    ];
    
    // Type mismatches with complex objects
    let result1 = processUsers(products); // Product[] passed to User[] parameter
    let result2 = processProducts(users); // User[] passed to Product[] parameter
    
    // Mixed arrays
    let mixedArray = [...users, ...products];
    let result3 = processUsers(mixedArray); // mixed array passed to User[] parameter
    let result4 = processProducts(mixedArray); // mixed array passed to Product[] parameter
}

// Function overloads with type mismatches
function overloadedFunction(x: number): number;
function overloadedFunction(x: string): string;
function overloadedFunction(x: boolean): boolean;
function overloadedFunction(x: number | string | boolean): number | string | boolean {
    if (typeof x === "number") {
        return x * 2;
    } else if (typeof x === "string") {
        return x.toUpperCase();
    } else {
        return !x;
    }
}

function testOverloadTypeMismatches(): void {
    // Correct usage
    let result1 = overloadedFunction(42);
    let result2 = overloadedFunction("hello");
    let result3 = overloadedFunction(true);
    
    // Type mismatches
    let result4 = overloadedFunction([1, 2, 3]); // array passed to overloaded function
    let result5 = overloadedFunction({ name: "test" }); // object passed to overloaded function
    let result6 = overloadedFunction(null); // null passed to overloaded function
}
)";
        
        result.insert(pos, typeParameterTransformation);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates functions with incorrect type parameters and type mismatches"; 
    }
};

// Advanced memory stress transformations
class AdvancedMemoryStressRule : public TransformationRule {
public:
    String getName() const override { return "AdvancedMemoryStress"; }
    TransformationType getType() const override { return TransformationType::MemoryStress; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add memory stress
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate advanced memory stress scenarios
        String memoryStress = R"(
// Advanced memory stress scenarios
function advancedMemoryStress(): void {
    // Large nested object structures
    let largeObject = {};
    for (let i = 0; i < 1000; i++) {
        largeObject[`level1_${i}`] = {};
        for (let j = 0; j < 100; j++) {
            largeObject[`level1_${i}`][`level2_${j}`] = {};
            for (let k = 0; k < 10; k++) {
                largeObject[`level1_${i}`][`level2_${j}`][`level3_${k}`] = {
                    data: new Array(1000).fill(i * j * k),
                    metadata: {
                        created: new Date(),
                        id: `${i}-${j}-${k}`,
                        nested: {
                            deep: {
                                deeper: {
                                    deepest: new Array(100).fill("deep data")
                                }
                            }
                        }
                    }
                };
            }
        }
    }
    
    // Large array operations
    let largeArrays = [];
    for (let i = 0; i < 100; i++) {
        let array = new Array(10000);
        for (let j = 0; j < 10000; j++) {
            array[j] = {
                index: j,
                value: Math.random() * 1000,
                nested: {
                    data: new Array(100).fill(j),
                    metadata: new Array(50).fill(`item_${j}`)
                }
            };
        }
        largeArrays.push(array);
    }
    
    // Complex function closures
    let closures = [];
    for (let i = 0; i < 1000; i++) {
        let closure = (function(index) {
            let data = new Array(1000).fill(index);
            return function() {
                return data.map(x => x * Math.random());
            };
        })(i);
        closures.push(closure);
    }
    
    // Execute closures to create memory pressure
    closures.forEach(closure => {
        try {
            closure();
        } catch (error) {
            // Handle potential memory errors
        }
    });
}

// Memory leak simulation
function memoryLeakSimulation(): void {
    let leakedObjects = [];
    
    // Simulate memory leaks
    for (let i = 0; i < 10000; i++) {
        let leakedObject = {
            id: i,
            data: new Array(1000).fill(i),
            circular: null as any
        };
        
        // Create circular reference
        leakedObject.circular = leakedObject;
        
        leakedObjects.push(leakedObject);
    }
    
    // Don't clean up - simulate memory leak
    console.log("Memory leak simulation complete");
}

// Buffer overflow simulation
function bufferOverflowSimulation(): void {
    let buffers = [];
    
    for (let i = 0; i < 1000; i++) {
        let buffer = new Array(10000);
        
        // Fill buffer beyond capacity
        for (let j = 0; j < 20000; j++) {
            try {
                buffer[j] = j;
            } catch (error) {
                // Handle buffer overflow
                break;
            }
        }
        
        buffers.push(buffer);
    }
}

// Stack overflow simulation
function stackOverflowSimulation(depth: number = 0): void {
    if (depth > 10000) {
        return;
    }
    
    let localData = new Array(100).fill(depth);
    stackOverflowSimulation(depth + 1);
}
)";
        
        result.insert(pos, memoryStress);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates advanced memory stress scenarios with complex data structures"; 
    }
};

// Performance stress with complex computations
class AdvancedPerformanceStressRule : public TransformationRule {
public:
    String getName() const override { return "AdvancedPerformanceStress"; }
    TransformationType getType() const override { return TransformationType::PerformanceStress; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add performance stress
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate advanced performance stress scenarios
        String performanceStress = R"(
// Advanced performance stress scenarios
function advancedPerformanceStress(): number {
    let result = 0;
    
    // Complex nested loops with heavy computations
    for (let i = 0; i < 1000; i++) {
        for (let j = 0; j < 1000; j++) {
            for (let k = 0; k < 1000; k++) {
                // Heavy mathematical computations
                let value = Math.sqrt(i * j * k) + 
                           Math.sin(i) * Math.cos(j) + 
                           Math.tan(k) * Math.log(i + 1) +
                           Math.pow(i, 2) + Math.pow(j, 2) + Math.pow(k, 2);
                
                // Complex conditional logic
                if (value > 0) {
                    if (i % 2 === 0) {
                        if (j % 3 === 0) {
                            if (k % 5 === 0) {
                                result += Math.sqrt(value) * Math.sin(value) * Math.cos(value);
                            } else {
                                result += Math.tan(value) * Math.log(value + 1);
                            }
                        } else {
                            result += Math.pow(value, 1.5) * Math.exp(value / 1000);
                        }
                    } else {
                        result += Math.abs(value) * Math.floor(value);
                    }
                } else {
                    result += Math.abs(value) * Math.ceil(value);
                }
            }
        }
    }
    
    return result;
}

// Recursive performance stress
function recursivePerformanceStress(n: number, depth: number = 0): number {
    if (depth > 1000 || n <= 0) {
        return 1;
    }
    
    let result = 0;
    
    // Heavy computation at each recursive level
    for (let i = 0; i < n; i++) {
        result += Math.sqrt(i) * Math.sin(i) * Math.cos(i) * Math.tan(i);
    }
    
    // Recursive calls
    let recursiveResult1 = recursivePerformanceStress(n - 1, depth + 1);
    let recursiveResult2 = recursivePerformanceStress(n - 2, depth + 1);
    let recursiveResult3 = recursivePerformanceStress(n - 3, depth + 1);
    
    return result + recursiveResult1 + recursiveResult2 + recursiveResult3;
}

// Complex data structure operations
function complexDataStructureOperations(): void {
    let complexData = [];
    
    // Create complex nested data structures
    for (let i = 0; i < 1000; i++) {
        let item = {
            id: i,
            data: new Array(1000).fill(0).map((_, index) => ({
                value: Math.random() * 1000,
                computed: Math.sqrt(index) * Math.sin(index),
                nested: {
                    level1: {
                        level2: {
                            level3: new Array(100).fill(0).map((_, j) => ({
                                deepValue: Math.random() * 100,
                                deepComputed: Math.pow(j, 2) + Math.sqrt(j)
                            }))
                        }
                    }
                }
            }))
        };
        
        complexData.push(item);
    }
    
    // Perform complex operations on the data
    let processedData = complexData.map(item => {
        return {
            id: item.id,
            sum: item.data.reduce((sum, d) => sum + d.value, 0),
            avg: item.data.reduce((sum, d) => sum + d.computed, 0) / item.data.length,
            max: Math.max(...item.data.map(d => d.value)),
            min: Math.min(...item.data.map(d => d.value)),
            nestedSum: item.data.reduce((sum, d) => {
                return sum + d.nested.level1.level2.level3.reduce((nestedSum, n) => {
                    return nestedSum + n.deepValue + n.deepComputed;
                }, 0);
            }, 0)
        };
    });
    
    // Sort and filter operations
    let sortedData = processedData.sort((a, b) => b.sum - a.sum);
    let filteredData = sortedData.filter(item => item.sum > 500000);
    
    console.log("Processed data count:", processedData.length);
    console.log("Filtered data count:", filteredData.length);
}

// String processing performance stress
function stringProcessingStress(): string {
    let result = " + ";
    
    // Generate large strings with complex operations
    for (let i = 0; i < 10000; i++) {
        let baseString = `Item_${i}_${Math.random()}_${Date.now()}`;
        
        // Complex string operations
        let processedString = baseString
            .toUpperCase()
            .toLowerCase()
            .split('_')
            .map(part => part.split('').reverse().join(''))
            .join('')
            .replace(/[0-9]/g, 'X')
            .replace(/[a-z]/g, 'Y')
            .replace(/[A-Z]/g, 'Z');
        
        result += processedString + "|";
    }
    
    // Complex string analysis
    let analysis = {
        length: result.length,
        charCounts: {},
        wordCount: result.split('|').length,
        uniqueChars: new Set(result.split('')).size
    };
    
    // Count character frequencies
    for (let char of result) {
        analysis.charCounts[char] = (analysis.charCounts[char] || 0) + 1;
    }
    
    return result;
}
)";
        
        result.insert(pos, performanceStress);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates advanced performance stress scenarios with complex computations"; 
    }
};

// Edge case generation with boundary conditions
class AdvancedEdgeCaseRule : public TransformationRule {
public:
    String getName() const override { return "AdvancedEdgeCase"; }
    TransformationType getType() const override { return TransformationType::EdgeCaseGeneration; }
    bool canApply(const String& code, const SourceLocation& location) const override {
        return true; // Can always add edge cases
    }
    
    String apply(const String& code, const SourceLocation& location) const override {
        String result = code;
        size_t pos = location.getColumn() - 1;
        
        // Generate advanced edge cases
        String edgeCases = R"(
// Advanced edge cases and boundary conditions
function advancedEdgeCases(): void {
    // Numeric edge cases
    let numericEdgeCases = {
        zero: 0,
        negativeZero: -0,
        infinity: Infinity,
        negativeInfinity: -Infinity,
        nan: NaN,
        maxSafeInteger: Number.MAX_SAFE_INTEGER,
        minSafeInteger: Number.MIN_SAFE_INTEGER,
        maxValue: Number.MAX_VALUE,
        minValue: Number.MIN_VALUE,
        epsilon: Number.EPSILON,
        veryLarge: 1e308,
        verySmall: 1e-308,
        precision: 0.1 + 0.2, // Floating point precision issue
        overflow: Number.MAX_VALUE * 2,
        underflow: Number.MIN_VALUE / 2
    };
    
    // String edge cases
    let stringEdgeCases = {
        empty: " + ",
        singleChar: "a",
        unicode: "🚀🌟💫⭐✨🔥💥⚡🌈🎉🎊🎈🎁🎀🎂🍰🎪🎭🎨🎬🎵🎶🎸🎹🎺🎻🎼🎽🎾🎿🏀🏁🏂🏃🏄🏅🏆🏇🏈🏉🏊🏋🏌🏍🏎🏏🏐🏑🏒🏓🏔🏕🏖🏗🏘🏙🏚🏛🏜🏝🏞🏟🏠🏡🏢🏣🏤🏥🏦🏧🏨🏩🏪🏫🏬🏭🏮🏯🏰🏱🏲🏳🏴🏵🏶🏷🏸🏹🏺🏻🏼🏽🏾🏿",
        specialChars: "!@#$%^&*()_+-=[]{}|;':\",./<>?`~",
        newlines: "line1\nline2\rline3\r\nline4",
        tabs: "col1\tcol2\tcol3",
        quotes: 'He said "Hello" and \'Goodbye\'',
        backslashes: "C:\\Users\\Name\\Documents",
        veryLong: "a".repeat(1000000),
        nullChars: "hello\0world\0test"
    };
    
    // Array edge cases
    let arrayEdgeCases = {
        empty: [],
        singleElement: [42],
        twoElements: [1, 2],
        largeArray: new Array(1000000).fill(0),
        sparseArray: [1, , , 4, , 6],
        mixedTypes: [1, "hello", true, null, undefined, {}, []],
        nestedArrays: [[1, 2], [3, 4], [5, 6]],
        deepNested: [[[[[[[[[[42]]]]]]]]]],
        circular: [] as any[]
    };
    
    // Create circular reference
    arrayEdgeCases.circular.push(arrayEdgeCases.circular);
    
    // Object edge cases
    let objectEdgeCases = {
        empty: {},
        singleProperty: { name: "test" },
        manyProperties: (() => {
            let obj = {};
            for (let i = 0; i < 10000; i++) {
                obj[`prop${i}`] = i;
            }
            return obj;
        })(),
        nestedObjects: {
            level1: {
                level2: {
                    level3: {
                        level4: {
                            level5: {
                                value: "deep"
                            }
                        }
                    }
                }
            }
        },
        circular: {} as any,
        prototype: Object.create(null),
        frozen: Object.freeze({ name: "frozen" }),
        sealed: Object.seal({ name: "sealed" }),
        nonExtensible: Object.preventExtensions({ name: "nonExtensible" })
    };
    
    // Create circular reference
    objectEdgeCases.circular.self = objectEdgeCases.circular;
    
    // Function edge cases
    let functionEdgeCases = {
        noParams: () => "no params",
        manyParams: (a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) => a + b + c,
        defaultParams: (x = 42, y = "hello", z = true) => x + y + z,
        restParams: (...args) => args.length,
        async: async () => "async",
        generator: function* () { yield 1; yield 2; yield 3; },
        arrow: () => "arrow",
        method: {
            method() { return "method"; }
        },
        bound: (function() { return this; }).bind({ name: "bound" }),
        constructor: function TestConstructor() { this.name = "constructor"; }
    };
    
    // Test edge cases
    console.log("Numeric edge cases:", numericEdgeCases);
    console.log("String edge cases:", stringEdgeCases);
    console.log("Array edge cases:", arrayEdgeCases);
    console.log("Object edge cases:", objectEdgeCases);
    console.log("Function edge cases:", functionEdgeCases);
}

// Boundary condition testing
function boundaryConditionTesting(): void {
    // Array boundary conditions
    let arr = [1, 2, 3, 4, 5];
    
    // Test array access at boundaries
    try {
        console.log("arr[-1]:", arr[-1]); // Negative index
        console.log("arr[0]:", arr[0]);   // First element
        console.log("arr[4]:", arr[4]);   // Last element
        console.log("arr[5]:", arr[5]);   // Out of bounds
        console.log("arr[100]:", arr[100]); // Far out of bounds
    } catch (error) {
        console.log("Array access error:", error);
    }
    
    // String boundary conditions
    let str = "hello";
    
    try {
        console.log("str[-1]:", str[-1]); // Negative index
        console.log("str[0]:", str[0]);   // First character
        console.log("str[4]:", str[4]);   // Last character
        console.log("str[5]:", str[5]);   // Out of bounds
        console.log("str[100]:", str[100]); // Far out of bounds
    } catch (error) {
        console.log("String access error:", error);
    }
    
    // Object boundary conditions
    let obj = { a: 1, b: 2, c: 3 };
    
    try {
        console.log("obj.undefined:", obj.undefined); // Undefined property
        console.log("obj.null:", obj.null);           // Null property
        console.log("obj['']:", obj['']);             // Empty string property
        console.log("obj[Symbol()]:", obj[Symbol()]); // Symbol property
    } catch (error) {
        console.log("Object access error:", error);
    }
}
)";
        
        result.insert(pos, edgeCases);
        return result;
    }
    
    String getDescription() const override { 
        return "Creates advanced edge cases and boundary conditions"; 
    }
};

// =============================================================================
// ADVANCED CODE TRANSFORMER WITH UPDATED PROBABILITIES
// =============================================================================

class AdvancedCodeTransformer : public CodeTransformer {
public:
    AdvancedCodeTransformer() {
        // Register advanced transformation rules
        registerAdvancedRules();
        
        // Update probability distribution for better error targeting
        updateProbabilityDistribution();
    }
    
private:
    void registerAdvancedRules() {
        // Register new advanced rules
        registerRule(make_unique<NestedObjectSpreadRule>());
        registerRule(make_unique<NestedControlFunctionRule>());
        registerRule(make_unique<TypeParameterTransformationRule>());
        registerRule(make_unique<AdvancedMemoryStressRule>());
        registerRule(make_unique<AdvancedPerformanceStressRule>());
        registerRule(make_unique<AdvancedEdgeCaseRule>());
    }
    
    void updateProbabilityDistribution() {
        // Update probabilities to better target error categories
        for (auto& rule : rules_) {
            TransformationType type = rule->getType();
            TransformationConfig config = rule->getConfig();
            
            switch (type) {
                case TransformationType::RemoveSemicolon:
                case TransformationType::RemoveBrace:
                case TransformationType::RemoveParenthesis:
                case TransformationType::RemoveBracket:
                    config.probability = 0.9; // High probability for syntax errors
                    break;
                    
                case TransformationType::ChangeTypeAnnotation:
                case TransformationType::AddTypeMismatch:
                case TransformationType::RemoveTypeAnnotation:
                    config.probability = 0.8; // High probability for type errors
                    break;
                    
                case TransformationType::RenameVariable:
                case TransformationType::RemoveDeclaration:
                case TransformationType::AddDuplicateIdentifier:
                    config.probability = 0.7; // Medium-high probability for semantic errors
                    break;
                    
                case TransformationType::DeepNesting:
                case TransformationType::LongExpressions:
                case TransformationType::ComplexExpressions:
                    config.probability = 0.6; // Medium probability for structural errors
                    break;
                    
                case TransformationType::MemoryStress:
                case TransformationType::PerformanceStress:
                    config.probability = 0.5; // Medium probability for stress tests
                    break;
                    
                case TransformationType::EdgeCaseGeneration:
                case TransformationType::BoundaryTesting:
                    config.probability = 0.4; // Lower probability for edge cases
                    break;
                    
                default:
                    config.probability = 0.5; // Default probability
                    break;
            }
            
            rule->setConfig(config);
        }
    }
};

// Factory function for advanced transformer
unique_ptr<CodeTransformer> createAdvancedCodeTransformer() {
    return make_unique<AdvancedCodeTransformer>();
}

} // namespace testing
} // namespace tsc
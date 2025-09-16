// Circular Dependencies Test
// Test circular import handling

import type {TypeB} from './moduleB';
// Module A imports from Module B
import {classB, ClassB, CONSTANT_B, EnumB, functionB, FunctionB, InterfaceB, NamespaceB} from './moduleB';

// Circular dependency with types
import type {TypeA} from './moduleA';
// Module B imports from Module A
// Circular dependency with interfaces
// Circular dependency with classes
// Circular dependency with functions
// Circular dependency with constants
// Circular dependency with enums
// Circular dependency with namespaces
import {classA, ClassA, CONSTANT_A, EnumA, functionA, FunctionA, InterfaceA, NamespaceA} from './moduleA';

// Test circular dependency resolution
console.log("Function B:", functionB);
console.log("Class B:", classB);
console.log("Function A:", functionA);
console.log("Class A:", classA);

// Test type usage
const typeA: TypeA = {value: 'A'};
const typeB: TypeB = {value: 'B'};

// Test interface usage
const interfaceA: InterfaceA = {name: 'InterfaceA'};
const interfaceB: InterfaceB = {name: 'InterfaceB'};

// Test class usage
const instanceA = new ClassA();
const instanceB = new ClassB();

// Test function usage
const resultA = FunctionA();
const resultB = FunctionB();

// Test constant usage
console.log("Constant A:", CONSTANT_A);
console.log("Constant B:", CONSTANT_B);

// Test enum usage
console.log("Enum A:", EnumA.VALUE1);
console.log("Enum B:", EnumB.VALUE1);

// Test namespace usage
console.log("Namespace A:", NamespaceA);
console.log("Namespace B:", NamespaceB);

console.log("Circular Dependencies test completed successfully!");

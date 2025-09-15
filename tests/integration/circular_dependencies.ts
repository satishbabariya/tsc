
// Circular Dependencies Test
// Test circular import handling

// Module A imports from Module B
import { functionB } from './moduleB';
import { classB } from './moduleB';

// Module B imports from Module A
import { functionA } from './moduleA';
import { classA } from './moduleA';

// Circular dependency with types
import type { TypeA } from './moduleA';
import type { TypeB } from './moduleB';

// Circular dependency with interfaces
import { InterfaceA } from './moduleA';
import { InterfaceB } from './moduleB';

// Circular dependency with classes
import { ClassA } from './moduleA';
import { ClassB } from './moduleB';

// Circular dependency with functions
import { FunctionA } from './moduleA';
import { FunctionB } from './moduleB';

// Circular dependency with constants
import { CONSTANT_A } from './moduleA';
import { CONSTANT_B } from './moduleB';

// Circular dependency with enums
import { EnumA } from './moduleA';
import { EnumB } from './moduleB';

// Circular dependency with namespaces
import { NamespaceA } from './moduleA';
import { NamespaceB } from './moduleB';

// Test circular dependency resolution
console.log("Function B:", functionB);
console.log("Class B:", classB);
console.log("Function A:", functionA);
console.log("Class A:", classA);

// Test type usage
const typeA: TypeA = { value: 'A' };
const typeB: TypeB = { value: 'B' };

// Test interface usage
const interfaceA: InterfaceA = { name: 'InterfaceA' };
const interfaceB: InterfaceB = { name: 'InterfaceB' };

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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Module System Testing Suite Generator
class ModuleSystemTestingSuite {
public:
    void runModuleSystemTests() {
        std::cout << "=== Module System Testing Suite ===" << std::endl;
        
        // Test 1: ES6 Module Syntax
        testES6ModuleSyntax();
        
        // Test 2: Namespace Support
        testNamespaceSupport();
        
        // Test 3: Module Resolution
        testModuleResolution();
        
        // Test 4: Circular Dependencies
        testCircularDependencies();
        
        // Test 5: Union Types
        testUnionTypes();
        
        // Test 6: Intersection Types
        testIntersectionTypes();
        
        // Test 7: Conditional Types
        testConditionalTypes();
        
        // Test 8: Mapped Types
        testMappedTypes();
        
        std::cout << "=== Module System Testing Complete ===" << std::endl;
    }
    
private:
    void testES6ModuleSyntax() {
        std::cout << "\n--- Testing ES6 Module Syntax ---" << std::endl;
        
        std::string es6ModuleTest = R"(
// ES6 Module Syntax Test
// Test import/export functionality

// Named exports
export const PI = 3.14159;
export const E = 2.71828;
export function add(a: number, b: number): number {
    return a + b;
}
export class Calculator {
    multiply(a: number, b: number): number {
        return a * b;
    }
}

// Default export
export default class MathUtils {
    static square(x: number): number {
        return x * x;
    }
}

// Re-export
export { PI as PI_CONSTANT, add as addFunction } from './math';
export * from './geometry';

// Type-only exports
export type { Point, Line } from './geometry';
export type { Calculator as CalculatorType } from './math';

// Import tests
import { PI, E, add, Calculator } from './math';
import MathUtils from './math';
import * as MathNamespace from './math';
import { PI as PI_ALIAS } from './math';
import type { Point, Line } from './geometry';

// Dynamic import
async function loadModule() {
    const module = await import('./dynamic-module');
    return module.default;
}

// Test module usage
console.log("PI:", PI);
console.log("E:", E);
console.log("Add result:", add(5, 3));

const calc = new Calculator();
console.log("Multiply result:", calc.multiply(4, 6));

const mathUtils = new MathUtils();
console.log("Square result:", MathUtils.square(7));

console.log("Math namespace:", MathNamespace);

// Test dynamic import
loadModule().then(module => {
    console.log("Dynamic module loaded:", module);
});

console.log("ES6 Module Syntax test completed successfully!");
)";
        
        saveTestFile("es6_module_syntax.ts", es6ModuleTest);
        std::cout << "Generated ES6 module syntax test" << std::endl;
    }
    
    void testNamespaceSupport() {
        std::cout << "\n--- Testing Namespace Support ---" << std::endl;
        
        std::string namespaceTest = R"(
// Namespace Support Test
// Test namespace functionality

// Basic namespace
namespace MyNamespace {
    export const VERSION = "1.0.0";
    export function greet(name: string): string {
        return `Hello, ${name}!`;
    }
    export class User {
        constructor(public name: string) {}
        getName(): string {
            return this.name;
        }
    }
}

// Nested namespace
namespace MyNamespace.SubNamespace {
    export const SUB_VERSION = "1.0.1";
    export function subGreet(name: string): string {
        return `Sub hello, ${name}!`;
    }
}

// Namespace with interface
namespace DataTypes {
    export interface Person {
        id: number;
        name: string;
        email: string;
    }
    
    export interface Address {
        street: string;
        city: string;
        country: string;
    }
    
    export class PersonManager {
        private people: Person[] = [];
        
        addPerson(person: Person): void {
            this.people.push(person);
        }
        
        getPerson(id: number): Person | undefined {
            return this.people.find(p => p.id === id);
        }
    }
}

// Global namespace
declare global {
    namespace NodeJS {
        interface Process {
            customProperty: string;
        }
    }
}

// Namespace merging
namespace Utils {
    export function formatDate(date: Date): string {
        return date.toISOString();
    }
}

namespace Utils {
    export function formatNumber(num: number): string {
        return num.toFixed(2);
    }
}

// Test namespace usage
console.log("Version:", MyNamespace.VERSION);
console.log("Greet:", MyNamespace.greet("Alice"));

const user = new MyNamespace.User("Bob");
console.log("User name:", user.getName());

console.log("Sub version:", MyNamespace.SubNamespace.SUB_VERSION);
console.log("Sub greet:", MyNamespace.SubNamespace.subGreet("Charlie"));

const personManager = new DataTypes.PersonManager();
personManager.addPerson({
    id: 1,
    name: "David",
    email: "david@example.com"
});

const person = personManager.getPerson(1);
console.log("Person:", person);

console.log("Formatted date:", Utils.formatDate(new Date()));
console.log("Formatted number:", Utils.formatNumber(3.14159));

console.log("Namespace Support test completed successfully!");
)";
        
        saveTestFile("namespace_support.ts", namespaceTest);
        std::cout << "Generated namespace support test" << std::endl;
    }
    
    void testModuleResolution() {
        std::cout << "\n--- Testing Module Resolution ---" << std::endl;
        
        std::string moduleResolutionTest = R"(
// Module Resolution Test
// Test path-based module loading

// Relative imports
import { helper } from './helpers/helper';
import { utils } from '../utils/utils';
import { config } from '../../config/config';

// Absolute imports
import { api } from '/src/api/api';
import { database } from '/src/database/database';

// Package imports
import React from 'react';
import { useState, useEffect } from 'react';
import * as lodash from 'lodash';
import { debounce } from 'lodash/debounce';

// Scoped package imports
import { Button } from '@mui/material';
import { ThemeProvider } from '@mui/material/styles';

// Type-only imports
import type { ComponentProps } from 'react';
import type { User } from './types/user';

// Dynamic imports with paths
async function loadUserModule() {
    const userModule = await import('./modules/user');
    return userModule;
}

async function loadAdminModule() {
    const adminModule = await import('./modules/admin');
    return adminModule;
}

// Conditional imports
async function loadModule(moduleName: string) {
    switch (moduleName) {
        case 'user':
            return await import('./modules/user');
        case 'admin':
            return await import('./modules/admin');
        case 'guest':
            return await import('./modules/guest');
        default:
            throw new Error(`Unknown module: ${moduleName}`);
    }
}

// Test module resolution
console.log("Helper:", helper);
console.log("Utils:", utils);
console.log("Config:", config);
console.log("API:", api);
console.log("Database:", database);

// Test React imports
const [state, setState] = useState(0);
useEffect(() => {
    console.log("Effect triggered");
}, []);

// Test lodash imports
const debouncedFunction = debounce(() => {
    console.log("Debounced function called");
}, 300);

// Test scoped package imports
const button = <Button>Click me</Button>;
const theme = <ThemeProvider theme={{}}>App</ThemeProvider>;

// Test dynamic imports
loadUserModule().then(module => {
    console.log("User module loaded:", module);
});

loadAdminModule().then(module => {
    console.log("Admin module loaded:", module);
});

// Test conditional imports
loadModule('user').then(module => {
    console.log("User module loaded conditionally:", module);
});

console.log("Module Resolution test completed successfully!");
)";
        
        saveTestFile("module_resolution.ts", moduleResolutionTest);
        std::cout << "Generated module resolution test" << std::endl;
    }
    
    void testCircularDependencies() {
        std::cout << "\n--- Testing Circular Dependencies ---" << std::endl;
        
        std::string circularDependencyTest = R"(
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
)";
        
        saveTestFile("circular_dependencies.ts", circularDependencyTest);
        std::cout << "Generated circular dependencies test" << std::endl;
    }
    
    void testUnionTypes() {
        std::cout << "\n--- Testing Union Types ---" << std::endl;
        
        std::string unionTypesTest = R"(
// Union Types Test
// Test union type functionality

// Basic union types
type StringOrNumber = string | number;
type StringOrNull = string | null;
type StringOrUndefined = string | undefined;
type StringOrNullOrUndefined = string | null | undefined;

// Complex union types
type Primitive = string | number | boolean | null | undefined;
type ArrayOrString = string[] | string;
type ObjectOrNull = object | null;
type FunctionOrString = Function | string;

// Union types with literals
type Status = 'pending' | 'approved' | 'rejected';
type Direction = 'up' | 'down' | 'left' | 'right';
type Color = 'red' | 'green' | 'blue' | 'yellow';

// Union types with interfaces
interface User {
    id: number;
    name: string;
}

interface Admin {
    id: number;
    name: string;
    permissions: string[];
}

type UserOrAdmin = User | Admin;

// Union types with classes
class Car {
    drive(): void {
        console.log("Driving car");
    }
}

class Bike {
    ride(): void {
        console.log("Riding bike");
    }
}

type Vehicle = Car | Bike;

// Union types with functions
type StringProcessor = (str: string) => string | ((str: string) => string);

// Union types with arrays
type StringOrNumberArray = string[] | number[];
type MixedArray = (string | number)[];

// Union types with objects
type StringOrNumberObject = { [key: string]: string } | { [key: string]: number };

// Test union type usage
function processValue(value: StringOrNumber): string {
    if (typeof value === 'string') {
        return value.toUpperCase();
    } else {
        return value.toString();
    }
}

function handleStatus(status: Status): string {
    switch (status) {
        case 'pending':
            return 'Please wait';
        case 'approved':
            return 'Approved';
        case 'rejected':
            return 'Rejected';
        default:
            return 'Unknown status';
    }
}

function processUser(user: UserOrAdmin): string {
    if ('permissions' in user) {
        return `Admin: ${user.name} with ${user.permissions.length} permissions`;
    } else {
        return `User: ${user.name}`;
    }
}

function useVehicle(vehicle: Vehicle): void {
    if (vehicle instanceof Car) {
        vehicle.drive();
    } else if (vehicle instanceof Bike) {
        vehicle.ride();
    }
}

// Test union type functions
console.log("Process string:", processValue("hello"));
console.log("Process number:", processValue(42));

console.log("Handle pending:", handleStatus('pending'));
console.log("Handle approved:", handleStatus('approved'));
console.log("Handle rejected:", handleStatus('rejected'));

const user: User = { id: 1, name: "Alice" };
const admin: Admin = { id: 2, name: "Bob", permissions: ["read", "write"] };

console.log("Process user:", processUser(user));
console.log("Process admin:", processUser(admin));

const car = new Car();
const bike = new Bike();

useVehicle(car);
useVehicle(bike);

// Test union type arrays
const stringArray: StringOrNumberArray = ["hello", "world"];
const numberArray: StringOrNumberArray = [1, 2, 3];
const mixedArray: MixedArray = ["hello", 42, "world", 3.14];

console.log("String array:", stringArray);
console.log("Number array:", numberArray);
console.log("Mixed array:", mixedArray);

// Test union type objects
const stringObject: StringOrNumberObject = { name: "Alice", age: "30" };
const numberObject: StringOrNumberObject = { count: 42, price: 99.99 };

console.log("String object:", stringObject);
console.log("Number object:", numberObject);

console.log("Union Types test completed successfully!");
)";
        
        saveTestFile("union_types.ts", unionTypesTest);
        std::cout << "Generated union types test" << std::endl;
    }
    
    void testIntersectionTypes() {
        std::cout << "\n--- Testing Intersection Types ---" << std::endl;
        
        std::string intersectionTypesTest = R"(
// Intersection Types Test
// Test intersection type functionality

// Basic intersection types
interface Person {
    name: string;
    age: number;
}

interface Employee {
    id: number;
    department: string;
}

type PersonAndEmployee = Person & Employee;

// Intersection with optional properties
interface OptionalPerson {
    name?: string;
    age?: number;
}

interface OptionalEmployee {
    id?: number;
    department?: string;
}

type OptionalPersonAndEmployee = OptionalPerson & OptionalEmployee;

// Intersection with readonly properties
interface ReadonlyPerson {
    readonly name: string;
    readonly age: number;
}

interface ReadonlyEmployee {
    readonly id: number;
    readonly department: string;
}

type ReadonlyPersonAndEmployee = ReadonlyPerson & ReadonlyEmployee;

// Intersection with methods
interface PersonWithMethods {
    getName(): string;
    getAge(): number;
}

interface EmployeeWithMethods {
    getId(): number;
    getDepartment(): string;
}

type PersonAndEmployeeWithMethods = PersonWithMethods & EmployeeWithMethods;

// Intersection with classes
class PersonClass {
    constructor(public name: string, public age: number) {}
    
    getName(): string {
        return this.name;
    }
    
    getAge(): number {
        return this.age;
    }
}

class EmployeeClass {
    constructor(public id: number, public department: string) {}
    
    getId(): number {
        return this.id;
    }
    
    getDepartment(): string {
        return this.department;
    }
}

// Intersection with generics
interface GenericPerson<T> {
    data: T;
    name: string;
}

interface GenericEmployee<T> {
    data: T;
    id: number;
}

type GenericPersonAndEmployee<T> = GenericPerson<T> & GenericEmployee<T>;

// Intersection with union types
type StringOrNumber = string | number;
type BooleanOrString = boolean | string;

type StringOrNumberAndBooleanOrString = StringOrNumber & BooleanOrString;

// Intersection with function types
type StringFunction = (str: string) => string;
type NumberFunction = (num: number) => number;

type StringAndNumberFunction = StringFunction & NumberFunction;

// Test intersection type usage
function createPersonAndEmployee(): PersonAndEmployee {
    return {
        name: "Alice",
        age: 30,
        id: 1,
        department: "Engineering"
    };
}

function processPersonAndEmployee(person: PersonAndEmployee): string {
    return `${person.name} (${person.age}) - ID: ${person.id}, Dept: ${person.department}`;
}

function createOptionalPersonAndEmployee(): OptionalPersonAndEmployee {
    return {
        name: "Bob",
        age: 25,
        id: 2,
        department: "Marketing"
    };
}

function processOptionalPersonAndEmployee(person: OptionalPersonAndEmployee): string {
    const name = person.name || "Unknown";
    const age = person.age || 0;
    const id = person.id || 0;
    const department = person.department || "Unknown";
    
    return `${name} (${age}) - ID: ${id}, Dept: ${department}`;
}

function createReadonlyPersonAndEmployee(): ReadonlyPersonAndEmployee {
    return {
        name: "Charlie",
        age: 35,
        id: 3,
        department: "Sales"
    };
}

function processReadonlyPersonAndEmployee(person: ReadonlyPersonAndEmployee): string {
    return `${person.name} (${person.age}) - ID: ${person.id}, Dept: ${person.department}`;
}

// Test intersection type functions
const personAndEmployee = createPersonAndEmployee();
console.log("Person and Employee:", processPersonAndEmployee(personAndEmployee));

const optionalPersonAndEmployee = createOptionalPersonAndEmployee();
console.log("Optional Person and Employee:", processOptionalPersonAndEmployee(optionalPersonAndEmployee));

const readonlyPersonAndEmployee = createReadonlyPersonAndEmployee();
console.log("Readonly Person and Employee:", processReadonlyPersonAndEmployee(readonlyPersonAndEmployee));

// Test intersection with generics
const genericPersonAndEmployee: GenericPersonAndEmployee<string> = {
    data: "test data",
    name: "David",
    id: 4
};

console.log("Generic Person and Employee:", genericPersonAndEmployee);

// Test intersection with union types
const stringValue: StringOrNumberAndBooleanOrString = "hello";
const numberValue: StringOrNumberAndBooleanOrString = 42;

console.log("String value:", stringValue);
console.log("Number value:", numberValue);

// Test intersection with function types
const stringAndNumberFunction: StringAndNumberFunction = ((input: string | number) => {
    if (typeof input === 'string') {
        return input.toUpperCase();
    } else {
        return input * 2;
    }
}) as StringAndNumberFunction;

console.log("String function result:", stringAndNumberFunction("hello"));
console.log("Number function result:", stringAndNumberFunction(21));

console.log("Intersection Types test completed successfully!");
)";
        
        saveTestFile("intersection_types.ts", intersectionTypesTest);
        std::cout << "Generated intersection types test" << std::endl;
    }
    
    void testConditionalTypes() {
        std::cout << "\n--- Testing Conditional Types ---" << std::endl;
        
        std::string conditionalTypesTest = R"(
// Conditional Types Test
// Test conditional type functionality

// Basic conditional types
type IsString<T> = T extends string ? true : false;
type IsNumber<T> = T extends number ? true : false;
type IsBoolean<T> = T extends boolean ? true : false;

// Conditional types with multiple conditions
type IsPrimitive<T> = T extends string | number | boolean ? true : false;
type IsArray<T> = T extends any[] ? true : false;
type IsObject<T> = T extends object ? true : false;

// Conditional types with function types
type IsFunction<T> = T extends Function ? true : false;
type IsAsyncFunction<T> = T extends (...args: any[]) => Promise<any> ? true : false;

// Conditional types with class types
type IsClass<T> = T extends new (...args: any[]) => any ? true : false;

// Conditional types with interface types
type HasName<T> = T extends { name: string } ? true : false;
type HasId<T> = T extends { id: number } ? true : false;

// Conditional types with union types
type IsStringOrNumber<T> = T extends string | number ? true : false;
type IsStringOrNumberOrBoolean<T> = T extends string | number | boolean ? true : false;

// Conditional types with intersection types
type IsPersonAndEmployee<T> = T extends { name: string } & { id: number } ? true : false;

// Conditional types with generic constraints
type IsExtending<T, U> = T extends U ? true : false;
type IsAssignable<T, U> = T extends U ? true : false;

// Conditional types with mapped types
type Optional<T> = T extends undefined ? T : T | undefined;
type Required<T> = T extends undefined ? never : T;

// Conditional types with recursive types
type DeepReadonly<T> = T extends object ? { readonly [K in keyof T]: DeepReadonly<T[K]> } : T;
type DeepPartial<T> = T extends object ? { [K in keyof T]?: DeepPartial<T[K]> } : T;

// Conditional types with utility types
type NonNullable<T> = T extends null | undefined ? never : T;
type Extract<T, U> = T extends U ? T : never;
type Exclude<T, U> = T extends U ? never : T;

// Test conditional type usage
type StringCheck = IsString<string>; // true
type NumberCheck = IsNumber<number>; // true
type BooleanCheck = IsBoolean<boolean>; // true
type StringNumberCheck = IsString<number>; // false

type PrimitiveCheck = IsPrimitive<string>; // true
type ArrayCheck = IsArray<string[]>; // true
type ObjectCheck = IsObject<object>; // true

type FunctionCheck = IsFunction<Function>; // true
type AsyncFunctionCheck = IsAsyncFunction<() => Promise<void>>; // true

type ClassCheck = IsClass<new () => any>; // true

type NameCheck = HasName<{ name: string }>; // true
type IdCheck = HasId<{ id: number }>; // true

type StringOrNumberCheck = IsStringOrNumber<string>; // true
type StringOrNumberOrBooleanCheck = IsStringOrNumberOrBoolean<boolean>; // true

type PersonAndEmployeeCheck = IsPersonAndEmployee<{ name: string; id: number }>; // true

type ExtendingCheck = IsExtending<string, string | number>; // true
type AssignableCheck = IsAssignable<string, string | number>; // true

type OptionalCheck = Optional<string>; // string | undefined
type RequiredCheck = Required<string | undefined>; // string

type DeepReadonlyCheck = DeepReadonly<{ name: string; age: number }>; // { readonly name: string; readonly age: number }
type DeepPartialCheck = DeepPartial<{ name: string; age: number }>; // { name?: string; age?: number }

type NonNullableCheck = NonNullable<string | null | undefined>; // string
type ExtractCheck = Extract<string | number, string>; // string
type ExcludeCheck = Exclude<string | number, string>; // number

// Test conditional type functions
function testConditionalTypes(): void {
    console.log("String check:", StringCheck);
    console.log("Number check:", NumberCheck);
    console.log("Boolean check:", BooleanCheck);
    console.log("String number check:", StringNumberCheck);
    
    console.log("Primitive check:", PrimitiveCheck);
    console.log("Array check:", ArrayCheck);
    console.log("Object check:", ObjectCheck);
    
    console.log("Function check:", FunctionCheck);
    console.log("Async function check:", AsyncFunctionCheck);
    
    console.log("Class check:", ClassCheck);
    
    console.log("Name check:", NameCheck);
    console.log("Id check:", IdCheck);
    
    console.log("String or number check:", StringOrNumberCheck);
    console.log("String or number or boolean check:", StringOrNumberOrBooleanCheck);
    
    console.log("Person and employee check:", PersonAndEmployeeCheck);
    
    console.log("Extending check:", ExtendingCheck);
    console.log("Assignable check:", AssignableCheck);
    
    console.log("Optional check:", OptionalCheck);
    console.log("Required check:", RequiredCheck);
    
    console.log("Deep readonly check:", DeepReadonlyCheck);
    console.log("Deep partial check:", DeepPartialCheck);
    
    console.log("Non nullable check:", NonNullableCheck);
    console.log("Extract check:", ExtractCheck);
    console.log("Exclude check:", ExcludeCheck);
}

// Run conditional type tests
testConditionalTypes();

console.log("Conditional Types test completed successfully!");
)";
        
        saveTestFile("conditional_types.ts", conditionalTypesTest);
        std::cout << "Generated conditional types test" << std::endl;
    }
    
    void testMappedTypes() {
        std::cout << "\n--- Testing Mapped Types ---" << std::endl;
        
        std::string mappedTypesTest = R"(
// Mapped Types Test
// Test mapped type functionality

// Basic mapped types
type Partial<T> = {
    [K in keyof T]?: T[K];
};

type Required<T> = {
    [K in keyof T]-?: T[K];
};

type Readonly<T> = {
    readonly [K in keyof T]: T[K];
};

type Mutable<T> = {
    -readonly [K in keyof T]: T[K];
};

// Mapped types with conditional types
type NonNullable<T> = {
    [K in keyof T]: T[K] extends null | undefined ? never : T[K];
};

type Pick<T, K extends keyof T> = {
    [P in K]: T[P];
};

type Omit<T, K extends keyof T> = {
    [P in Exclude<keyof T, K>]: T[P];
};

// Mapped types with string manipulation
type Uppercase<S extends string> = S extends `${infer F}${infer R}` ? `${Uppercase<F>}${Uppercase<R>}` : S;
type Lowercase<S extends string> = S extends `${infer F}${infer R}` ? `${Lowercase<F>}${Lowercase<R>}` : S;
type Capitalize<S extends string> = S extends `${infer F}${infer R}` ? `${Capitalize<F>}${R}` : S;
type Uncapitalize<S extends string> = S extends `${infer F}${infer R}` ? `${Uncapitalize<F>}${R}` : S;

// Mapped types with template literals
type EventName<T extends string> = `on${Capitalize<T>}`;
type PropertyName<T extends string> = `get${Capitalize<T>}`;
type SetterName<T extends string> = `set${Capitalize<T>}`;

// Mapped types with keyof
type Keys<T> = keyof T;
type Values<T> = T[keyof T];
type Entries<T> = {
    [K in keyof T]: [K, T[K]];
};

// Mapped types with recursive types
type DeepPartial<T> = {
    [K in keyof T]?: T[K] extends object ? DeepPartial<T[K]> : T[K];
};

type DeepReadonly<T> = {
    readonly [K in keyof T]: T[K] extends object ? DeepReadonly<T[K]> : T[K];
};

type DeepRequired<T> = {
    [K in keyof T]-?: T[K] extends object ? DeepRequired<T[K]> : T[K];
};

// Mapped types with utility types
type Record<K extends keyof any, T> = {
    [P in K]: T;
};

type RecordFromKeys<T, K extends keyof T> = {
    [P in K]: T[P];
};

// Test mapped type usage
interface User {
    id: number;
    name: string;
    email: string;
    age: number;
    isActive: boolean;
}

interface Product {
    id: number;
    name: string;
    price: number;
    description: string;
    inStock: boolean;
}

// Test basic mapped types
type PartialUser = Partial<User>;
type RequiredUser = Required<PartialUser>;
type ReadonlyUser = Readonly<User>;
type MutableUser = Mutable<ReadonlyUser>;

// Test conditional mapped types
type NonNullableUser = NonNullable<User>;
type UserKeys = Pick<User, 'id' | 'name'>;
type UserWithoutId = Omit<User, 'id'>;

// Test string manipulation mapped types
type UppercaseName = Uppercase<'hello'>;
type LowercaseName = Lowercase<'HELLO'>;
type CapitalizedName = Capitalize<'hello'>;
type UncapitalizedName = Uncapitalize<'Hello'>;

// Test template literal mapped types
type ClickEvent = EventName<'click'>;
type ChangeEvent = EventName<'change'>;
type SubmitEvent = EventName<'submit'>;

type GetName = PropertyName<'name'>;
type GetAge = PropertyName<'age'>;
type GetEmail = PropertyName<'email'>;

type SetName = SetterName<'name'>;
type SetAge = SetterName<'age'>;
type SetEmail = SetterName<'email'>;

// Test keyof mapped types
type UserKeysType = Keys<User>;
type UserValuesType = Values<User>;
type UserEntriesType = Entries<User>;

// Test recursive mapped types
type DeepPartialUser = DeepPartial<User>;
type DeepReadonlyUser = DeepReadonly<User>;
type DeepRequiredUser = DeepRequired<PartialUser>;

// Test utility mapped types
type UserRecord = Record<'id' | 'name' | 'email', string>;
type UserRecordFromKeys = RecordFromKeys<User, 'id' | 'name'>;

// Test mapped type functions
function testMappedTypes(): void {
    console.log("Partial user:", {} as PartialUser);
    console.log("Required user:", {} as RequiredUser);
    console.log("Readonly user:", {} as ReadonlyUser);
    console.log("Mutable user:", {} as MutableUser);
    
    console.log("Non nullable user:", {} as NonNullableUser);
    console.log("User keys:", {} as UserKeys);
    console.log("User without id:", {} as UserWithoutId);
    
    console.log("Uppercase name:", UppercaseName);
    console.log("Lowercase name:", LowercaseName);
    console.log("Capitalized name:", CapitalizedName);
    console.log("Uncapitalized name:", UncapitalizedName);
    
    console.log("Click event:", ClickEvent);
    console.log("Change event:", ChangeEvent);
    console.log("Submit event:", SubmitEvent);
    
    console.log("Get name:", GetName);
    console.log("Get age:", GetAge);
    console.log("Get email:", GetEmail);
    
    console.log("Set name:", SetName);
    console.log("Set age:", SetAge);
    console.log("Set email:", SetEmail);
    
    console.log("User keys type:", UserKeysType);
    console.log("User values type:", UserValuesType);
    console.log("User entries type:", UserEntriesType);
    
    console.log("Deep partial user:", {} as DeepPartialUser);
    console.log("Deep readonly user:", {} as DeepReadonlyUser);
    console.log("Deep required user:", {} as DeepRequiredUser);
    
    console.log("User record:", {} as UserRecord);
    console.log("User record from keys:", {} as UserRecordFromKeys);
}

// Run mapped type tests
testMappedTypes();

console.log("Mapped Types test completed successfully!");
)";
        
        saveTestFile("mapped_types.ts", mappedTypesTest);
        std::cout << "Generated mapped types test" << std::endl;
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }
};

int main() {
    try {
        ModuleSystemTestingSuite suite;
        suite.runModuleSystemTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
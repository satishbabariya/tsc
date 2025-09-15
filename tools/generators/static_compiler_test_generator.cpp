#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "=== Static Compiler Test Generator ===" << std::endl;
    
    // Generate static module system test
    std::string staticModuleTest = R"(
// Static Module System Test
// Test static imports/exports (no dynamic imports)

// Static named exports
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

// Static default export
export default class MathUtils {
    static square(x: number): number {
        return x * x;
    }
}

// Static re-export
export { PI as PI_CONSTANT, add as addFunction } from './math';
export * from './geometry';

// Type-only exports (compile-time only)
export type { Point, Line } from './geometry';
export type { Calculator as CalculatorType } from './math';

// Static imports (compile-time resolution)
import { PI, E, add, Calculator } from './math';
import MathUtils from './math';
import * as MathNamespace from './math';
import { PI as PI_ALIAS } from './math';
import type { Point, Line } from './geometry';

// Static module usage
console.log("PI:", PI);
console.log("E:", E);
console.log("Add result:", add(5, 3));

const calc = new Calculator();
console.log("Multiply result:", calc.multiply(4, 6));

const mathUtils = new MathUtils();
console.log("Square result:", MathUtils.square(7));

console.log("Math namespace:", MathNamespace);

console.log("Static Module System test completed successfully!");
)";
    
    std::ofstream staticFile("static_module_system.ts");
    if (staticFile.is_open()) {
        staticFile << staticModuleTest;
        staticFile.close();
        std::cout << "Generated: static_module_system.ts" << std::endl;
    }
    
    // Generate static namespace test
    std::string staticNamespaceTest = R"(
// Static Namespace System Test
// Test static namespace declarations

// Static namespace
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

// Static nested namespace
namespace MyNamespace.SubNamespace {
    export const SUB_VERSION = "1.0.1";
    export function subGreet(name: string): string {
        return `Sub hello, ${name}!`;
    }
}

// Static namespace with interface
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

// Static global namespace
declare global {
    namespace NodeJS {
        interface Process {
            customProperty: string;
        }
    }
}

// Static namespace merging
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

// Test static namespace usage
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

console.log("Static Namespace System test completed successfully!");
)";
    
    std::ofstream namespaceFile("static_namespace_system.ts");
    if (namespaceFile.is_open()) {
        namespaceFile << staticNamespaceTest;
        namespaceFile.close();
        std::cout << "Generated: static_namespace_system.ts" << std::endl;
    }
    
    // Generate static type system test
    std::string staticTypeTest = R"(
// Static Type System Test
// Test static type resolution and compilation

// Static union types
type StringOrNumber = string | number;
type Status = 'pending' | 'approved' | 'rejected';

// Static intersection types
interface Person {
    name: string;
    age: number;
}

interface Employee {
    id: number;
    department: string;
}

type PersonAndEmployee = Person & Employee;

// Static conditional types
type IsString<T> = T extends string ? true : false;
type NonNullable<T> = T extends null | undefined ? never : T;

// Static mapped types
type Partial<T> = {
    [K in keyof T]?: T[K];
};

type Required<T> = {
    [K in keyof T]-?: T[K];
};

// Static recursive types
type DeepReadonly<T> = T extends object ? { readonly [K in keyof T]: DeepReadonly<T[K]> } : T;
type DeepPartial<T> = T extends object ? { [K in keyof T]?: DeepPartial<T[K]> } : T;

// Static utility types
type Extract<T, U> = T extends U ? T : never;
type Exclude<T, U> = T extends U ? never : T;
type Pick<T, K extends keyof T> = { [P in K]: T[P] };
type Omit<T, K extends keyof T> = { [P in Exclude<keyof T, K>]: T[P] };

// Test static type usage
function processValue(value: StringOrNumber): string {
    if (typeof value === 'string') {
        return value.toUpperCase();
    } else {
        return value.toString();
    }
}

function handleStatus(status: Status): string {
    switch (status) {
        case 'pending': return 'Please wait';
        case 'approved': return 'Approved';
        case 'rejected': return 'Rejected';
        default: return 'Unknown status';
    }
}

function createPersonAndEmployee(): PersonAndEmployee {
    return {
        name: "Alice",
        age: 30,
        id: 1,
        department: "Engineering"
    };
}

// Test static type functions
console.log("Process string:", processValue("hello"));
console.log("Process number:", processValue(42));

console.log("Handle pending:", handleStatus('pending'));
console.log("Handle approved:", handleStatus('approved'));
console.log("Handle rejected:", handleStatus('rejected'));

const personAndEmployee = createPersonAndEmployee();
console.log("Person and Employee:", personAndEmployee);

// Test static type transformations
type User = {
    id: number;
    name: string;
    email: string;
    age: number;
};

type PartialUser = Partial<User>;
type RequiredUser = Required<PartialUser>;
type ReadonlyUser = DeepReadonly<User>;

const partialUser: PartialUser = { name: "Bob" };
const requiredUser: RequiredUser = { id: 1, name: "Alice", email: "alice@example.com", age: 30 };
const readonlyUser: ReadonlyUser = { id: 2, name: "Charlie", email: "charlie@example.com", age: 25 };

console.log("Partial user:", partialUser);
console.log("Required user:", requiredUser);
console.log("Readonly user:", readonlyUser);

console.log("Static Type System test completed successfully!");
)";
    
    std::ofstream typeFile("static_type_system.ts");
    if (typeFile.is_open()) {
        typeFile << staticTypeTest;
        typeFile.close();
        std::cout << "Generated: static_type_system.ts" << std::endl;
    }
    
    // Generate static compilation test
    std::string staticCompilationTest = R"(
// Static Compilation Test
// Test static compilation features

// Static class with generics
class StaticContainer<T> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    size(): number {
        return this.items.length;
    }
    
    map<U>(fn: (item: T) => U): StaticContainer<U> {
        const newContainer = new StaticContainer<U>();
        for (const item of this.items) {
            newContainer.add(fn(item));
        }
        return newContainer;
    }
    
    filter(predicate: (item: T) => boolean): StaticContainer<T> {
        const newContainer = new StaticContainer<T>();
        for (const item of this.items) {
            if (predicate(item)) {
                newContainer.add(item);
            }
        }
        return newContainer;
    }
}

// Static function with generics
function staticProcess<T, U>(input: T, processor: (value: T) => U): U {
    return processor(input);
}

// Static interface with generics
interface StaticProcessor<T, U> {
    process(input: T): U;
    validate(input: T): boolean;
}

// Static implementation
class StringProcessor implements StaticProcessor<string, number> {
    process(input: string): number {
        return input.length;
    }
    
    validate(input: string): boolean {
        return typeof input === 'string' && input.length > 0;
    }
}

// Test static compilation
const stringContainer = new StaticContainer<string>();
stringContainer.add("hello");
stringContainer.add("world");
stringContainer.add("typescript");

console.log("Container size:", stringContainer.size());
console.log("First item:", stringContainer.get(0));

const lengthContainer = stringContainer.map(str => str.length);
console.log("Length container:", lengthContainer);

const filteredContainer = stringContainer.filter(str => str.length > 5);
console.log("Filtered container:", filteredContainer);

const processor = new StringProcessor();
const result = staticProcess("hello world", processor.process);
console.log("Processed result:", result);

const isValid = processor.validate("hello");
console.log("Is valid:", isValid);

console.log("Static Compilation test completed successfully!");
)";
    
    std::ofstream compilationFile("static_compilation.ts");
    if (compilationFile.is_open()) {
        compilationFile << staticCompilationTest;
        compilationFile.close();
        std::cout << "Generated: static_compilation.ts" << std::endl;
    }
    
    std::cout << "=== Static Compiler Test Generation Complete ===" << std::endl;
    return 0;
}
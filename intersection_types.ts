
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


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

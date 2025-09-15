
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


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


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

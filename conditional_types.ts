
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

// Negative tests - these should fail compilation due to type errors

// Test constraint violations
class ConstraintViolationTest<T extends string> {
    private items: T[];
    
    // This should fail - trying to push number to string array
    addNumber(num: number): void {
        this.items.push(num);  // ERROR: Expected
    }
    
    // This should fail - trying to push boolean to string array  
    addBoolean(flag: boolean): void {
        this.items.push(flag);  // ERROR: Expected
    }
    
    // This should fail - trying to push any to constrained array
    addAny(value: any): void {
        this.items.push(value);  // ERROR: Expected
    }
}

// Test union constraint violations
class UnionConstraintViolation<T extends string | number> {
    private data: T[];
    
    // This should fail - boolean not in union constraint
    addBoolean(flag: boolean): void {
        this.data.push(flag);  // ERROR: Expected
    }
    
    // This should fail - null not in union constraint
    addNull(): void {
        this.data.push(null);  // ERROR: Expected
    }
}

// Test generic type mismatches
class TypeMismatchTest<T, U> {
    private tArray: T[];
    private uArray: U[];
    
    // This should fail - mixing T and U types
    mixTypes(tItem: T, uItem: U): void {
        this.tArray.push(uItem);  // ERROR: Expected
        this.uArray.push(tItem);  // ERROR: Expected
    }
    
    // This should fail - wrong array assignment
    wrongAssignment(): void {
        this.tArray = this.uArray;  // ERROR: Expected
    }
}

// Test array method type errors
class ArrayMethodTypeErrors<T> {
    private items: T[];
    
    // This should fail - wrong argument type to push
    wrongPushType(num: number): void {
        this.items.push(num);  // ERROR: Expected (if T is not number)
    }
    
    // This should fail - wrong argument type to unshift
    wrongUnshiftType(str: string): void {
        this.items.unshift(str);  // ERROR: Expected (if T is not string)
    }
}

// Test generic function type errors
class GenericFunctionErrors<T> {
    private storage: T[];
    
    // This should fail - function expects T but gets wrong type
    callWithWrongType(): void {
        this.storage.push(42);  // ERROR: Expected (if T is not number)
        this.storage.push("hello");  // ERROR: Expected (if T is not string)
        this.storage.push(true);  // ERROR: Expected (if T is not boolean)
    }
}

// Test interface constraint violations
interface StringConstraint {
    toString(): string;
}

class InterfaceConstraintViolation<T extends StringConstraint> {
    private items: T[];
    
    // This should fail - number doesn't satisfy StringConstraint
    addNumber(num: number): void {
        this.items.push(num);  // ERROR: Expected
    }
    
    // This should fail - boolean doesn't satisfy StringConstraint
    addBoolean(flag: boolean): void {
        this.items.push(flag);  // ERROR: Expected
    }
}
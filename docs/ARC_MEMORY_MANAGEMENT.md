# ARC Memory Management in TSC

## Overview

TSC implements Automatic Reference Counting (ARC) as its primary memory management strategy. ARC provides automatic memory management without the overhead of garbage collection, making it ideal for performance-oriented native code generation.

## Key Features

- **Automatic Memory Management**: Objects are automatically deallocated when no longer referenced
- **Deterministic Performance**: No GC pauses or unpredictable memory management overhead
- **Smart Pointer Types**: `unique_ptr<T>`, `shared_ptr<T>`, and `weak_ptr<T>` for different ownership patterns
- **Move Semantics**: Efficient transfer of ownership using `std::move()`
- **Memory Management Annotations**: `@manual_memory`, `@stack_allocated`, `@immutable` for fine-grained control
- **Cycle Detection**: Automatic detection and breaking of reference cycles
- **LLVM Optimization**: ARC-specific optimizations to minimize runtime overhead

## Smart Pointer Types

### unique_ptr<T>

Exclusive ownership with automatic cleanup when the pointer goes out of scope.

```typescript
let ptr: unique_ptr<number> = new unique_ptr<number>(42);
let value = ptr.get(); // Access the value
// ptr is automatically cleaned up when it goes out of scope
```

### shared_ptr<T>

Shared ownership with reference counting. Multiple pointers can reference the same object.

```typescript
let ptr1: shared_ptr<string> = new shared_ptr<string>("Hello");
let ptr2: shared_ptr<string> = ptr1; // Both pointers reference the same object
console.log(ptr1.get()); // "Hello"
console.log(ptr2.get()); // "Hello"
// Object is deallocated when both pointers go out of scope
```

### weak_ptr<T>

Non-owning reference that doesn't prevent object deallocation.

```typescript
let strong: shared_ptr<number> = new shared_ptr<number>(100);
let weak: weak_ptr<number> = new weak_ptr<number>(strong);

let value = weak.get(); // Returns 100 if object still exists, null otherwise

strong = null; // Object is deallocated
let value2 = weak.get(); // Returns null
```

## Move Semantics

Move semantics allow efficient transfer of ownership without copying.

```typescript
let source: unique_ptr<number> = new unique_ptr<number>(42);
let destination: unique_ptr<number> = std::move(source);

console.log(source.get()); // null (ownership transferred)
console.log(destination.get()); // 42
```

## Memory Management Annotations

### @manual_memory

Indicates that the class requires manual memory management.

```typescript
@manual_memory
class ManualClass {
    private data: number[];
    
    constructor(size: number) {
        this.data = new Array<number>(size);
    }
    
    // Manual cleanup required
    destroy(): void {
        this.data = null;
    }
}
```

### @stack_allocated

Hints that the class should be allocated on the stack when possible.

```typescript
@stack_allocated
class StackClass {
    private value: number;
    
    constructor(value: number) {
        this.value = value;
    }
}
```

### @immutable

Indicates that the class instances are immutable.

```typescript
@immutable
class ImmutableClass {
    private readonly value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    getValue(): number {
        return this.value;
    }
}
```

## Best Practices

### 1. Use Appropriate Smart Pointer Types

- Use `unique_ptr<T>` for exclusive ownership
- Use `shared_ptr<T>` for shared ownership
- Use `weak_ptr<T>` to break reference cycles

### 2. Prefer Move Semantics

```typescript
// Good: Use move semantics
let ptr1: unique_ptr<number> = new unique_ptr<number>(42);
let ptr2: unique_ptr<number> = std::move(ptr1);

// Avoid: Copying unique_ptr
let ptr3: unique_ptr<number> = ptr1; // Error: unique_ptr cannot be copied
```

### 3. Break Reference Cycles

```typescript
class Node {
    private value: number;
    private next: weak_ptr<Node>; // Use weak_ptr to break cycles
    
    constructor(value: number) {
        this.value = value;
    }
    
    setNext(node: shared_ptr<Node>): void {
        this.next = new weak_ptr<Node>(node);
    }
}
```

### 4. Use Memory Annotations Appropriately

- Use `@manual_memory` only when necessary
- Use `@stack_allocated` for small, short-lived objects
- Use `@immutable` for objects that don't change after construction

## Performance Considerations

### ARC Overhead

ARC adds minimal overhead to your programs:

- **Reference Counting**: ~10-20 cycles per retain/release operation
- **Object Headers**: 16-32 bytes per ARC-managed object
- **Optimization**: LLVM can eliminate 50-80% of ARC calls

### Optimization Tips

1. **Use Move Semantics**: Avoid unnecessary reference counting
2. **Prefer Stack Allocation**: Use `@stack_allocated` for small objects
3. **Minimize Shared Ownership**: Use `unique_ptr<T>` when possible
4. **Break Cycles Early**: Use `weak_ptr<T>` to prevent reference cycles

## Error Handling

### Common Errors

1. **Use After Move**: Accessing a moved object
```typescript
let ptr1: unique_ptr<number> = new unique_ptr<number>(42);
let ptr2: unique_ptr<number> = std::move(ptr1);
let value = ptr1.get(); // Error: ptr1 has been moved
```

2. **Dangling Weak References**: Accessing weak references after object deallocation
```typescript
let strong: shared_ptr<number> = new shared_ptr<number>(42);
let weak: weak_ptr<number> = new weak_ptr<number>(strong);
strong = null;
let value = weak.get(); // Returns null, not an error
```

3. **Reference Cycles**: Creating circular references without weak pointers
```typescript
class A {
    private b: shared_ptr<B>;
}

class B {
    private a: shared_ptr<A>; // Should be weak_ptr<A>
}
```

## Debugging

### Memory Statistics

Use the ARC runtime functions to debug memory issues:

```typescript
// Print memory statistics
__tsc_memory_stats();

// Print reference counts for an object
__tsc_print_ref_counts(obj);

// Dump object information
__tsc_dump_object_info(obj);
```

### Common Debugging Scenarios

1. **Memory Leaks**: Check for unreleased objects
2. **Premature Deallocation**: Check reference counting
3. **Reference Cycles**: Use cycle detection tools
4. **Performance Issues**: Profile ARC operations

## Migration Guide

### From Manual Memory Management

1. Replace `malloc`/`free` with smart pointers
2. Use `shared_ptr<T>` for shared ownership
3. Use `unique_ptr<T>` for exclusive ownership
4. Add `@manual_memory` annotation where needed

### From Garbage Collection

1. Replace automatic GC with ARC
2. Use `weak_ptr<T>` to break cycles
3. Add explicit ownership annotations
4. Use move semantics for performance

## Examples

### Simple Example

```typescript
function createAndUseObject(): number {
    let obj: shared_ptr<number> = new shared_ptr<number>(42);
    let value = obj.get();
    return value * 2;
    // obj is automatically cleaned up
}
```

### Complex Example

```typescript
class Database {
    private connection: unique_ptr<Connection>;
    
    constructor() {
        this.connection = new unique_ptr<Connection>(new Connection());
    }
    
    query(sql: string): shared_ptr<Result[]> {
        let results: shared_ptr<Result>[] = [];
        // ... perform query ...
        return new shared_ptr<Result[]>(results);
    }
    
    close(): void {
        this.connection = null; // Automatic cleanup
    }
}
```

## Conclusion

ARC memory management in TSC provides a powerful, efficient, and safe way to manage memory in native applications. By following best practices and using the appropriate smart pointer types, you can write high-performance code without the complexity of manual memory management or the overhead of garbage collection.
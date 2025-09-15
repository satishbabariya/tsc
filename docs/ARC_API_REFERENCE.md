# ARC API Reference

## Runtime Functions

### Memory Management

#### `__tsc_alloc(size, destructor, type_info)`

Allocates memory with ARC header.

**Parameters:**
- `size` (size_t): Size of the object to allocate
- `destructor` (void*): Destructor function pointer (can be null)
- `type_info` (void*): Type information pointer (can be null)

**Returns:** `void*` - Pointer to the allocated object

**Example:**
```c
void* obj = __tsc_alloc(sizeof(int), NULL, NULL);
```

#### `__tsc_dealloc(obj)`

Deallocates ARC-managed memory.

**Parameters:**
- `obj` (void*): Pointer to the object to deallocate

**Returns:** `void`

**Example:**
```c
__tsc_dealloc(obj);
```

### Reference Counting

#### `__tsc_retain(obj)`

Increments the reference count of an ARC object.

**Parameters:**
- `obj` (void*): Pointer to the ARC object

**Returns:** `void*` - The same pointer (for chaining)

**Example:**
```c
void* retained = __tsc_retain(obj);
```

#### `__tsc_release(obj)`

Decrements the reference count of an ARC object.

**Parameters:**
- `obj` (void*): Pointer to the ARC object

**Returns:** `void`

**Example:**
```c
__tsc_release(obj);
```

### Weak References

#### `__tsc_weak_load(weak_ref)`

Loads a value from a weak reference if the object is still alive.

**Parameters:**
- `weak_ref` (void*): Weak reference pointer

**Returns:** `void*` - Object pointer if alive, null otherwise

**Example:**
```c
void* obj = __tsc_weak_load(weak_ref);
if (obj) {
    // Object is still alive
    __tsc_retain(obj);
}
```

#### `__tsc_weak_store(weak_ref, obj)`

Stores a value in a weak reference.

**Parameters:**
- `weak_ref` (void*): Weak reference pointer
- `obj` (void*): Object pointer to store

**Returns:** `void`

**Example:**
```c
__tsc_weak_store(weak_ref, obj);
```

#### `__tsc_weak_release(weak_ref)`

Releases a weak reference.

**Parameters:**
- `weak_ref` (void*): Weak reference pointer

**Returns:** `void`

**Example:**
```c
__tsc_weak_release(weak_ref);
```

### Cycle Detection

#### `__tsc_has_cycles(obj)`

Checks if an object is part of a reference cycle.

**Parameters:**
- `obj` (void*): Object to check

**Returns:** `bool` - true if cycles detected, false otherwise

**Example:**
```c
if (__tsc_has_cycles(obj)) {
    printf("Cycle detected!\n");
}
```

#### `__tsc_break_cycles(obj)`

Breaks reference cycles by converting strong references to weak references.

**Parameters:**
- `obj` (void*): Object to process

**Returns:** `void`

**Example:**
```c
__tsc_break_cycles(obj);
```

### Debug Functions

#### `__tsc_print_ref_counts(obj)`

Prints reference count information for an object.

**Parameters:**
- `obj` (void*): Object to inspect

**Returns:** `void`

**Example:**
```c
__tsc_print_ref_counts(obj);
```

#### `__tsc_memory_stats()`

Prints memory statistics.

**Parameters:** None

**Returns:** `void`

**Example:**
```c
__tsc_memory_stats();
```

#### `__tsc_dump_object_info(obj)`

Dumps detailed information about an object.

**Parameters:**
- `obj` (void*): Object to inspect

**Returns:** `void`

**Example:**
```c
__tsc_dump_object_info(obj);
```

### Utility Functions

#### `__tsc_get_header(obj)`

Gets the ARC header for an object.

**Parameters:**
- `obj` (void*): Object pointer

**Returns:** `ARC_ObjectHeader*` - Pointer to the ARC header

**Example:**
```c
ARC_ObjectHeader* header = __tsc_get_header(obj);
```

#### `__tsc_is_arc_object(obj)`

Checks if a pointer points to an ARC-managed object.

**Parameters:**
- `obj` (void*): Pointer to check

**Returns:** `bool` - true if ARC object, false otherwise

**Example:**
```c
if (__tsc_is_arc_object(obj)) {
    printf("This is an ARC object\n");
}
```

#### `__tsc_get_ref_count(obj)`

Gets the reference count of an object.

**Parameters:**
- `obj` (void*): Object pointer

**Returns:** `size_t` - Reference count

**Example:**
```c
size_t count = __tsc_get_ref_count(obj);
printf("Reference count: %zu\n", count);
```

#### `__tsc_get_weak_count(obj)`

Gets the weak reference count of an object.

**Parameters:**
- `obj` (void*): Object pointer

**Returns:** `size_t` - Weak reference count

**Example:**
```c
size_t weak_count = __tsc_get_weak_count(obj);
printf("Weak reference count: %zu\n", weak_count);
```

### Performance Monitoring

#### `__tsc_get_memory_stats()`

Gets memory statistics structure.

**Parameters:** None

**Returns:** `ARC_MemoryStats` - Memory statistics

**Example:**
```c
ARC_MemoryStats stats = __tsc_get_memory_stats();
printf("Total allocations: %zu\n", stats.total_allocations);
printf("Total deallocations: %zu\n", stats.total_deallocations);
printf("Total retains: %zu\n", stats.total_retains);
printf("Total releases: %zu\n", stats.total_releases);
```

#### `__tsc_reset_memory_stats()`

Resets memory statistics.

**Parameters:** None

**Returns:** `void`

**Example:**
```c
__tsc_reset_memory_stats();
```

## Data Structures

### ARC_ObjectHeader

Structure containing ARC metadata for each object.

```c
typedef struct {
    uint32_t ref_count;        // Reference count (atomic)
    uint32_t weak_count;       // Weak reference count (atomic)
    void (*destructor)(void*); // Destructor function
    void* type_info;          // Type information for debugging
} ARC_ObjectHeader;
```

### ARC_MemoryStats

Structure containing memory statistics.

```c
typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t total_retains;
    size_t total_releases;
    size_t peak_memory_usage;
    double average_lifetime;
} ARC_MemoryStats;
```

## TypeScript Syntax

### Smart Pointer Types

#### unique_ptr<T>

Exclusive ownership smart pointer.

```typescript
let ptr: unique_ptr<number> = new unique_ptr<number>(42);
let value = ptr.get();
```

#### shared_ptr<T>

Shared ownership smart pointer.

```typescript
let ptr: shared_ptr<string> = new shared_ptr<string>("Hello");
let value = ptr.get();
```

#### weak_ptr<T>

Weak reference smart pointer.

```typescript
let strong: shared_ptr<number> = new shared_ptr<number>(100);
let weak: weak_ptr<number> = new weak_ptr<number>(strong);
let value = weak.get();
```

### Move Semantics

#### std::move()

Transfers ownership without copying.

```typescript
let source: unique_ptr<number> = new unique_ptr<number>(42);
let destination: unique_ptr<number> = std::move(source);
```

### Memory Management Annotations

#### @manual_memory

Indicates manual memory management required.

```typescript
@manual_memory
class ManualClass {
    // Manual memory management required
}
```

#### @stack_allocated

Hints for stack allocation.

```typescript
@stack_allocated
class StackClass {
    // Prefer stack allocation
}
```

#### @immutable

Indicates immutable objects.

```typescript
@immutable
class ImmutableClass {
    // Object is immutable
}
```

## Error Codes

### Common Error Scenarios

1. **Null Pointer Access**: Accessing null ARC objects
2. **Use After Move**: Accessing moved objects
3. **Double Release**: Releasing the same object twice
4. **Invalid Weak Reference**: Accessing weak references after object deallocation

### Error Handling

All ARC functions handle null pointers gracefully:

- `__tsc_retain(NULL)` returns `NULL`
- `__tsc_release(NULL)` does nothing
- `__tsc_weak_load(NULL)` returns `NULL`

## Thread Safety

ARC operations are thread-safe:

- Reference counting uses atomic operations
- Weak reference operations are synchronized
- Memory allocation/deallocation is thread-safe

## Performance Characteristics

### Operation Costs

- **Allocation**: ~100-200 cycles (malloc + header setup)
- **Retain**: ~10-20 cycles (atomic increment)
- **Release**: ~20-50 cycles (atomic decrement + conditional deallocation)
- **Weak Load**: ~5-10 cycles (indirection + null check)

### Memory Overhead

- **Object Header**: 16-32 bytes per ARC object
- **Weak References**: Additional indirection overhead
- **Cycle Detection**: Minimal overhead when not used

## Best Practices

1. **Use Appropriate Types**: Choose the right smart pointer type
2. **Prefer Move Semantics**: Use `std::move()` when possible
3. **Break Cycles**: Use `weak_ptr<T>` to prevent reference cycles
4. **Profile Performance**: Monitor ARC overhead in performance-critical code
5. **Handle Errors**: Check for null pointers and invalid operations
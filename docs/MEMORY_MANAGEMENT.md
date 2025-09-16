# Memory Management System

## Overview

The TSC compiler implements a comprehensive memory management system that combines RAII (Resource Acquisition Is Initialization) principles with Automatic Reference Counting (ARC) and debug memory tracking. This system ensures safe, efficient, and leak-free memory management throughout the compiler.

## Architecture

### 1. RAII Wrappers (`include/tsc/runtime/RAII.h`)

The RAII system provides C++ smart pointer classes that automatically manage memory lifecycle:

#### UniquePtr<T>
- **Purpose**: Exclusive ownership of a single object
- **Features**: 
  - Move-only semantics (no copying)
  - Automatic destruction when going out of scope
  - Custom deleters support
  - Exception-safe resource management

```cpp
{
    UniquePtr<TestObject> ptr(new TestObject(42));
    // Object automatically destroyed when ptr goes out of scope
}
```

#### SharedPtr<T>
- **Purpose**: Shared ownership with reference counting
- **Features**:
  - Thread-safe reference counting
  - Automatic destruction when last reference is released
  - Copy and move semantics
  - Custom deleters support

```cpp
{
    SharedPtr<TestObject> ptr1(new TestObject(42));
    {
        SharedPtr<TestObject> ptr2 = ptr1; // Reference count = 2
        // Both ptr1 and ptr2 share ownership
    } // Reference count = 1
} // Object destroyed when ptr1 goes out of scope
```

#### ARCPtr<T>
- **Purpose**: Integration with ARC runtime system
- **Features**:
  - Uses `__tsc_retain`/`__tsc_release` for reference counting
  - Compatible with ARC-managed objects
  - Automatic memory management through ARC

```cpp
{
    ARCPtr<TestObject> ptr(arc_object);
    // Object managed by ARC runtime
}
```

#### MallocPtr
- **Purpose**: RAII wrapper for malloc/free
- **Features**:
  - Automatic free() when going out of scope
  - Size tracking
  - Exception-safe malloc management

```cpp
{
    MallocPtr ptr(1024); // Allocates 1024 bytes
    // Automatically freed when ptr goes out of scope
}
```

### 2. ARC Runtime (`runtime/arc/arc_runtime.c`)

The ARC (Automatic Reference Counting) system provides:

#### Core Functions
- `__tsc_retain(obj)`: Increment reference count
- `__tsc_release(obj)`: Decrement reference count and deallocate if zero
- `__tsc_alloc(size, destructor, type_info)`: Allocate ARC-managed object
- `__tsc_dealloc(obj)`: Deallocate ARC-managed object

#### Features
- Thread-safe atomic reference counting
- Automatic destructor calling
- Memory statistics tracking
- Debug information support
- Cycle detection infrastructure

### 3. Malloc Tracking (`runtime/malloc_tracking.c`)

Debug-only memory leak detection system:

#### Features
- **Thread-safe allocation tracking**: Hash table with mutex protection
- **Leak detection**: Reports unfreed memory blocks at program exit
- **Source location tracking**: File, line, and function information
- **Performance monitoring**: Allocation statistics and peak usage
- **Conditional compilation**: Only active in debug builds

#### Usage
```c
#ifdef TSC_DEBUG_MEMORY
void* ptr = TSC_MALLOC(1024);  // Automatically tracked
TSC_FREE(ptr);                 // Automatically untracked
#endif
```

## Integration Points

### 1. Compiler Integration
- **AST Nodes**: Use RAII wrappers for automatic cleanup
- **Symbol Tables**: ARC-managed symbol storage
- **Type System**: Smart pointers for type objects
- **Code Generation**: Memory-safe LLVM IR generation

### 2. Runtime Integration
- **String Operations**: ARC-managed string objects
- **Array Operations**: Reference-counted arrays
- **Exception Handling**: Safe exception object management

## Performance Characteristics

### Benchmarks
- **UniquePtr operations**: ~0.067 microseconds per operation
- **SharedPtr operations**: Thread-safe with minimal overhead
- **ARC operations**: Atomic operations with cache-friendly design
- **Malloc tracking**: Zero overhead in release builds

### Memory Overhead
- **ARC objects**: +16 bytes header per object
- **SharedPtr**: +8 bytes reference count per object
- **UniquePtr**: No additional overhead
- **Malloc tracking**: Only in debug builds

## Thread Safety

### Guarantees
- **ARC operations**: Fully thread-safe with atomic operations
- **SharedPtr**: Thread-safe reference counting
- **UniquePtr**: Move-only, no concurrent access needed
- **Malloc tracking**: Thread-safe with mutex protection

### Concurrency Patterns
```cpp
// Safe concurrent access
SharedPtr<Data> shared_data = make_shared<Data>();

std::thread t1([&shared_data]() {
    auto local_copy = shared_data; // Safe copy
    // Use local_copy
});

std::thread t2([&shared_data]() {
    auto local_copy = shared_data; // Safe copy
    // Use local_copy
});
```

## Debug Features

### Memory Leak Detection
```cpp
// Enable debug tracking
#define TSC_DEBUG_MEMORY

// Automatic leak reporting at program exit
MemoryManager::reportLeaks();
```

### Statistics Monitoring
```cpp
// Get allocation statistics
auto stats = __tsc_get_malloc_stats();
std::cout << "Total allocations: " << stats.total_allocations << std::endl;
std::cout << "Peak memory: " << stats.peak_allocations << std::endl;
```

### ARC Debug Information
```cpp
// Print reference counts
__tsc_print_ref_counts(obj);

// Dump object information
__tsc_dump_object_info(obj);
```

## Best Practices

### 1. Use RAII Principles
```cpp
// Good: Automatic cleanup
{
    UniquePtr<Resource> resource = make_unique<Resource>();
    // Use resource
} // Automatically cleaned up

// Bad: Manual management
Resource* resource = new Resource();
// ... use resource ...
delete resource; // Easy to forget
```

### 2. Prefer Smart Pointers
```cpp
// Good: Use make_unique/make_shared
auto ptr = make_unique<MyClass>(arg1, arg2);

// Avoid: Raw new/delete
MyClass* ptr = new MyClass(arg1, arg2);
```

### 3. Move Semantics
```cpp
// Good: Move when transferring ownership
UniquePtr<Data> data = std::move(other_data);

// Avoid: Copying unique resources
UniquePtr<Data> data = other_data; // Compile error - good!
```

### 4. ARC Integration
```cpp
// Good: Use ARC for runtime objects
ARCPtr<RuntimeObject> obj = make_arc(runtime_object);

// Avoid: Manual ARC management
__tsc_retain(obj);
// ... use obj ...
__tsc_release(obj); // Easy to forget
```

## Testing

### Test Coverage
- **Unit Tests**: All RAII wrapper classes
- **Integration Tests**: ARC runtime integration
- **Concurrency Tests**: Thread-safe operations
- **Performance Tests**: Benchmarking and profiling
- **Leak Detection**: Memory leak validation

### Running Tests
```bash
# Run memory management tests
./tests/MemoryManagementTests

# Run all tests
make test
```

## Future Enhancements

### Planned Features
1. **Cycle Detection**: Automatic detection and breaking of reference cycles
2. **Weak References**: Non-owning references to ARC objects
3. **Memory Pools**: Efficient allocation for small objects
4. **Garbage Collection**: Optional GC for complex object graphs
5. **Memory Profiling**: Advanced profiling and analysis tools

### Performance Optimizations
1. **Lock-free Reference Counting**: Reduce mutex contention
2. **Memory Pool Allocation**: Reduce malloc/free overhead
3. **Cache-friendly Data Structures**: Improve memory access patterns
4. **Compile-time Optimizations**: Eliminate unnecessary reference counting

## Conclusion

The TSC memory management system provides a robust, efficient, and safe foundation for the compiler. By combining RAII principles with ARC and debug tracking, it ensures that memory management is both automatic and reliable, reducing the likelihood of memory leaks and improving overall system stability.

The system is designed to be:
- **Safe**: Automatic cleanup prevents memory leaks
- **Efficient**: Minimal overhead with smart optimizations
- **Debuggable**: Comprehensive tracking and reporting
- **Thread-safe**: Concurrent access without data races
- **Extensible**: Easy to add new memory management features
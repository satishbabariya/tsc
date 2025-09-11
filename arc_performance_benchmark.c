#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Performance measurement utilities
double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

// Benchmark object creation and destruction
void benchmark_object_lifecycle() {
    printf("=== Benchmark: Object Lifecycle ===\n");
    
    const int iterations = 100000;
    double start_time = get_time_ms();
    
    for (int i = 0; i < iterations; i++) {
        TSCObject* obj = tsc_object_new(sizeof(double), tsc_type_number());
        if (obj) {
            double* value = (double*)obj->block->data;
            *value = (double)i;
            tsc_object_unref(obj);
        }
    }
    
    double end_time = get_time_ms();
    double duration = end_time - start_time;
    
    printf("Created and destroyed %d objects in %.2f ms\n", iterations, duration);
    printf("Average time per object: %.4f ms\n", duration / iterations);
    printf("Objects per second: %.0f\n", iterations / (duration / 1000.0));
}

// Benchmark reference counting
void benchmark_reference_counting() {
    printf("\n=== Benchmark: Reference Counting ===\n");
    
    const int iterations = 50000;
    double start_time = get_time_ms();
    
    for (int i = 0; i < iterations; i++) {
        TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
        TSCObject* obj2 = tsc_object_ref(obj1);
        TSCObject* obj3 = tsc_object_ref(obj1);
        TSCObject* obj4 = tsc_object_ref(obj2);
        
        // Simulate some work
        double* value = (double*)obj1->block->data;
        *value = (double)i;
        
        tsc_object_unref(obj1);
        tsc_object_unref(obj2);
        tsc_object_unref(obj3);
        tsc_object_unref(obj4);
    }
    
    double end_time = get_time_ms();
    double duration = end_time - start_time;
    
    printf("Created and destroyed %d reference chains in %.2f ms\n", iterations, duration);
    printf("Average time per chain: %.4f ms\n", duration / iterations);
    printf("Chains per second: %.0f\n", iterations / (duration / 1000.0));
}

// Benchmark string operations
void benchmark_string_operations() {
    printf("\n=== Benchmark: String Operations ===\n");
    
    const int iterations = 10000;
    double start_time = get_time_ms();
    
    for (int i = 0; i < iterations; i++) {
        TSCObject* str1 = tsc_object_new(50, tsc_type_string());
        TSCObject* str2 = tsc_object_new(50, tsc_type_string());
        
        if (str1 && str2) {
            char* data1 = (char*)str1->block->data;
            char* data2 = (char*)str2->block->data;
            snprintf(data1, 50, "Hello %d", i);
            snprintf(data2, 50, "World %d", i);
            
            TSCObject* result = tsc_string_concat(str1, str2);
            if (result) {
                tsc_object_unref(result);
            }
        }
        
        tsc_object_unref(str1);
        tsc_object_unref(str2);
    }
    
    double end_time = get_time_ms();
    double duration = end_time - start_time;
    
    printf("Performed %d string concatenations in %.2f ms\n", iterations, duration);
    printf("Average time per concatenation: %.4f ms\n", duration / iterations);
    printf("Concatenations per second: %.0f\n", iterations / (duration / 1000.0));
}

// Benchmark memory allocation patterns
void benchmark_memory_patterns() {
    printf("\n=== Benchmark: Memory Allocation Patterns ===\n");
    
    const int num_objects = 10000;
    TSCObject** objects = malloc(sizeof(TSCObject*) * num_objects);
    
    if (!objects) {
        printf("Failed to allocate object array\n");
        return;
    }
    
    // Test allocation pattern
    double start_time = get_time_ms();
    
    for (int i = 0; i < num_objects; i++) {
        objects[i] = tsc_object_new(sizeof(double), tsc_type_number());
        if (objects[i]) {
            double* value = (double*)objects[i]->block->data;
            *value = (double)i;
        }
    }
    
    double alloc_time = get_time_ms();
    printf("Allocated %d objects in %.2f ms\n", num_objects, alloc_time - start_time);
    
    // Test deallocation pattern
    double dealloc_start = get_time_ms();
    
    for (int i = 0; i < num_objects; i++) {
        tsc_object_unref(objects[i]);
    }
    
    double dealloc_time = get_time_ms();
    printf("Deallocated %d objects in %.2f ms\n", num_objects, dealloc_time - dealloc_start);
    
    free(objects);
    
    printf("Total time: %.2f ms\n", dealloc_time - start_time);
    printf("Average time per object: %.4f ms\n", (dealloc_time - start_time) / num_objects);
}

// Benchmark concurrent access (simulated)
void benchmark_concurrent_simulation() {
    printf("\n=== Benchmark: Concurrent Access Simulation ===\n");
    
    const int num_threads = 4;
    const int operations_per_thread = 25000;
    
    TSCObject* shared_obj = tsc_object_new(sizeof(double), tsc_type_number());
    if (!shared_obj) {
        printf("Failed to create shared object\n");
        return;
    }
    
    double start_time = get_time_ms();
    
    // Simulate concurrent access by creating many references
    for (int t = 0; t < num_threads; t++) {
        for (int i = 0; i < operations_per_thread; i++) {
            TSCObject* ref = tsc_object_ref(shared_obj);
            if (ref) {
                // Simulate some work
                double* value = (double*)ref->block->data;
                *value = (double)i;
                tsc_object_unref(ref);
            }
        }
    }
    
    double end_time = get_time_ms();
    double duration = end_time - start_time;
    
    printf("Simulated %d concurrent operations in %.2f ms\n", 
           num_threads * operations_per_thread, duration);
    printf("Operations per second: %.0f\n", 
           (num_threads * operations_per_thread) / (duration / 1000.0));
    
    tsc_object_unref(shared_obj);
}

// Memory usage analysis
void analyze_memory_usage() {
    printf("\n=== Memory Usage Analysis ===\n");
    
    printf("TSCObject size: %zu bytes\n", sizeof(TSCObject));
    printf("TSCARCBlock size: %zu bytes\n", sizeof(TSCARCBlock));
    printf("TypeInfo size: %zu bytes\n", sizeof(TypeInfo));
    
    // Test actual memory usage
    const int num_objects = 1000;
    TSCObject** objects = malloc(sizeof(TSCObject*) * num_objects);
    
    if (!objects) {
        printf("Failed to allocate object array\n");
        return;
    }
    
    // Create objects and measure memory
    for (int i = 0; i < num_objects; i++) {
        objects[i] = tsc_object_new(sizeof(double), tsc_type_number());
    }
    
    printf("Created %d objects\n", num_objects);
    printf("Live blocks: %zu\n", tsc_arc_get_live_count());
    
    // Calculate theoretical memory usage
    size_t theoretical_size = num_objects * (sizeof(TSCObject) + sizeof(TSCARCBlock) + sizeof(double));
    printf("Theoretical memory usage: %zu bytes (%.2f KB)\n", 
           theoretical_size, theoretical_size / 1024.0);
    
    // Clean up
    for (int i = 0; i < num_objects; i++) {
        tsc_object_unref(objects[i]);
    }
    
    free(objects);
    
    printf("After cleanup: %zu live blocks\n", tsc_arc_get_live_count());
}

int main() {
    printf("ARC Performance Benchmark Suite\n");
    printf("===============================\n");
    
    benchmark_object_lifecycle();
    benchmark_reference_counting();
    benchmark_string_operations();
    benchmark_memory_patterns();
    benchmark_concurrent_simulation();
    analyze_memory_usage();
    
    printf("\n=== Benchmark Complete ===\n");
    return 0;
}
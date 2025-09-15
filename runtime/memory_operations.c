// Memory operations runtime for TSC
#include <stdlib.h>
#include "arc/arc_runtime.h"

// Function definitions for mangled malloc functions
// These provide compatibility with existing code while transitioning to ARC
void* malloc_1(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_2(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_3(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_4(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_5(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_6(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_7(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_8(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_9(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_10(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_11(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_12(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_13(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_14(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_15(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

void* malloc_16(size_t size) {
    return __tsc_alloc(size, NULL, NULL);
}

// Aliases for mangled malloc functions
__asm__(".weak malloc.1");
__asm__(".set malloc.1, malloc_1");
__asm__(".weak malloc.2");
__asm__(".set malloc.2, malloc_2");
__asm__(".weak malloc.3");
__asm__(".set malloc.3, malloc_3");
__asm__(".weak malloc.4");
__asm__(".set malloc.4, malloc_4");
__asm__(".weak malloc.5");
__asm__(".set malloc.5, malloc_5");
__asm__(".weak malloc.6");
__asm__(".set malloc.6, malloc_6");
__asm__(".weak malloc.7");
__asm__(".set malloc.7, malloc_7");
__asm__(".weak malloc.8");
__asm__(".set malloc.8, malloc_8");
__asm__(".weak malloc.9");
__asm__(".set malloc.9, malloc_9");
__asm__(".weak malloc.10");
__asm__(".set malloc.10, malloc_10");
__asm__(".weak malloc.11");
__asm__(".set malloc.11, malloc_11");
__asm__(".weak malloc.12");
__asm__(".set malloc.12, malloc_12");
__asm__(".weak malloc.13");
__asm__(".set malloc.13, malloc_13");
__asm__(".weak malloc.14");
__asm__(".set malloc.14, malloc_14");
__asm__(".weak malloc.15");
__asm__(".set malloc.15, malloc_15");
__asm__(".weak malloc.16");
__asm__(".set malloc.16, malloc_16");
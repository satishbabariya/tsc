// Print operations runtime for TSC
#include <stdio.h>

// Simple print function - takes any value and prints it
void print(void* value) {
    if (value == NULL) {
        printf("null\n");
        return;
    }
    
    // For now, assume all values are strings
    // In the future, this could be enhanced to handle different types
    char* str = (char*)value;
    printf("%s\n", str);
}
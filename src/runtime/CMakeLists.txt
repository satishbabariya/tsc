# Runtime memory management library
add_library(tsc_memory_runtime
    ReferenceCounter.cpp
    GarbageCollector.cpp
    LLVMMemoryManager.cpp
)

target_include_directories(tsc_memory_runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(tsc_memory_runtime
    PRIVATE
        LLVM
)

target_compile_definitions(tsc_memory_runtime
    PUBLIC
        ${LLVM_DEFINITIONS}
)

# Set C++ standard
target_compile_features(tsc_memory_runtime
    PRIVATE
        cxx_std_17
)

# Compiler-specific options
target_compile_options(tsc_memory_runtime
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

# Debug build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tsc_memory_runtime
        PRIVATE
            -g
            -O0
            -fsanitize=address
            -fsanitize=leak
            -fno-omit-frame-pointer
    )
    
    target_link_options(tsc_memory_runtime
        PRIVATE
            -fsanitize=address
            -fsanitize=leak
    )
endif()

# Release build options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(tsc_memory_runtime
        PRIVATE
            -O3
            -DNDEBUG
            -flto
    )
    
    target_link_options(tsc_memory_runtime
        PRIVATE
            -flto
    )
endif()

# Install target
install(TARGETS tsc_memory_runtime
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/tsc/runtime
    FILES_MATCHING PATTERN "*.h"
)
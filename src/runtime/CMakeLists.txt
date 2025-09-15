# Runtime memory management library
add_library(tsc_runtime
    ReferenceCounter.cpp
    GarbageCollector.cpp
    LLVMMemoryManager.cpp
)

target_include_directories(tsc_runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tsc_runtime
    PRIVATE
        LLVM
)

# Set C++ standard
target_compile_features(tsc_runtime
    PRIVATE
        cxx_std_17
)

# Compiler-specific options
target_compile_options(tsc_runtime
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

# Debug build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tsc_runtime
        PRIVATE
            -g
            -O0
            -fsanitize=address
            -fsanitize=leak
            -fno-omit-frame-pointer
    )
    
    target_link_options(tsc_runtime
        PRIVATE
            -fsanitize=address
            -fsanitize=leak
    )
endif()

# Release build options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(tsc_runtime
        PRIVATE
            -O3
            -DNDEBUG
            -flto
    )
    
    target_link_options(tsc_runtime
        PRIVATE
            -flto
    )
endif()

# Install target
install(TARGETS tsc_runtime
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/tsc/runtime
    FILES_MATCHING PATTERN "*.h"
)
cmake_minimum_required(VERSION 3.20)

# Configurable LLVM installation path
# Users can override this with: cmake -DLLVM_ROOT=/path/to/llvm ..
if(NOT DEFINED LLVM_ROOT)
    # Fatal error if LLVM_ROOT is not set
    message(FATAL_ERROR "LLVM_ROOT is not set. Please specify the correct path with: cmake -DLLVM_ROOT=/path/to/llvm ..")
endif()

# Validate LLVM installation path
if(NOT EXISTS "${LLVM_ROOT}")
    message(FATAL_ERROR "LLVM installation not found at: ${LLVM_ROOT}\n"
            "Please specify the correct path with: cmake -DLLVM_ROOT=/path/to/llvm ..")
endif()

# Use LLVM compilers to match the LLVM library version
set(CMAKE_C_COMPILER "${LLVM_ROOT}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang++")

# Validate compiler existence
if(NOT EXISTS "${CMAKE_C_COMPILER}")
    message(FATAL_ERROR "LLVM clang compiler not found at: ${CMAKE_C_COMPILER}\n"
            "Please check your LLVM installation at: ${LLVM_ROOT}")
endif()

if(NOT EXISTS "${CMAKE_CXX_COMPILER}")
    message(FATAL_ERROR "LLVM clang++ compiler not found at: ${CMAKE_CXX_COMPILER}\n"
            "Please check your LLVM installation at: ${LLVM_ROOT}")
endif()

project(TSC VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable parallel compilation (this is handled by make -j, not compiler flags)

# Find LLVM - Use configurable installation path
set(CMAKE_PREFIX_PATH "${LLVM_ROOT}/lib/cmake/llvm")

# Display configuration information
message(STATUS "=== TSC Build Configuration ===")
message(STATUS "LLVM Root: ${LLVM_ROOT}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "LLVM CMake Path: ${CMAKE_PREFIX_PATH}")
message(STATUS "===============================")

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Try to find missing dependencies and set them as optional
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(ZSTD QUIET libzstd)
    pkg_check_modules(LIBEDIT QUIET libedit)
    pkg_check_modules(CURL QUIET libcurl)
endif()

# Set variables to indicate missing dependencies are optional
if(NOT ZSTD_FOUND)
    set(ZSTD_FOUND FALSE)
endif()
if(NOT LIBEDIT_FOUND)
    set(LIBEDIT_FOUND FALSE)
endif()
if(NOT CURL_FOUND)
    set(CURL_FOUND FALSE)
endif()

# Detect build target architecture and configure LLVM libraries accordingly
if(LLVM_MONOLITHIC_LIB)
    set(llvm_libs ${LLVM_MONOLITHIC_LIB})
    message(STATUS "Using monolithic LLVM library: ${LLVM_MONOLITHIC_LIB}")
elseif(EXISTS "${LLVM_ROOT}/lib/libLLVM.so")
    set(llvm_libs "${LLVM_ROOT}/lib/libLLVM.so")
    message(STATUS "Using shared LLVM library: ${llvm_libs}")
else()
    # Allow override of target architecture for cross-compilation
    if(NOT DEFINED TSC_TARGET_ARCH)
        # Detect the target architecture
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            set(TARGET_ARCH "AArch64")
            message(STATUS "Detected ARM64/AArch64 target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
            set(TARGET_ARCH "X86")
            message(STATUS "Detected x86_64 target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|armv7")
            set(TARGET_ARCH "ARM")
            message(STATUS "Detected ARM target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv64")
            set(TARGET_ARCH "RISCV")
            message(STATUS "Detected RISC-V 64-bit target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv32")
            set(TARGET_ARCH "RISCV")
            message(STATUS "Detected RISC-V 32-bit target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "mips64|mips")
            set(TARGET_ARCH "Mips")
            message(STATUS "Detected MIPS target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "powerpc64|ppc64|powerpc|ppc")
            set(TARGET_ARCH "PowerPC")
            message(STATUS "Detected PowerPC target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "sparc64|sparc")
            set(TARGET_ARCH "Sparc")
            message(STATUS "Detected SPARC target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "s390x|systemz")
            set(TARGET_ARCH "SystemZ")
            message(STATUS "Detected SystemZ target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "wasm32|wasm64|webassembly")
            set(TARGET_ARCH "WebAssembly")
            message(STATUS "Detected WebAssembly target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "nvptx|gpu")
            set(TARGET_ARCH "NVPTX")
            message(STATUS "Detected NVIDIA PTX target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "amdgpu|gcn")
            set(TARGET_ARCH "AMDGPU")
            message(STATUS "Detected AMD GPU target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "hexagon")
            set(TARGET_ARCH "Hexagon")
            message(STATUS "Detected Hexagon target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "bpf")
            set(TARGET_ARCH "BPF")
            message(STATUS "Detected BPF target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
            set(TARGET_ARCH "AVR")
            message(STATUS "Detected AVR target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "msp430")
            set(TARGET_ARCH "MSP430")
            message(STATUS "Detected MSP430 target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "lanai")
            set(TARGET_ARCH "Lanai")
            message(STATUS "Detected Lanai target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ve")
            set(TARGET_ARCH "VE")
            message(STATUS "Detected VE target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch")
            set(TARGET_ARCH "LoongArch")
            message(STATUS "Detected LoongArch target architecture")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "xcore")
            set(TARGET_ARCH "XCore")
            message(STATUS "Detected XCore target architecture")
        else()
            set(TARGET_ARCH "AArch64")  # Default fallback
            message(WARNING "Unknown target architecture: ${CMAKE_SYSTEM_PROCESSOR}, defaulting to AArch64")
        endif()
    else()
        set(TARGET_ARCH ${TSC_TARGET_ARCH})
        message(STATUS "Using user-specified target architecture: ${TARGET_ARCH}")
    endif()
    
    # Configure LLVM libraries based on detected target
    set(llvm_components
        Core
        Support
        Analysis
        ExecutionEngine
        InstCombine
        Object
        RuntimeDyld
        ScalarOpts
        Target
    )
    
    # Add target-specific components
    if(TARGET_ARCH STREQUAL "AArch64")
        list(APPEND llvm_components
            AArch64CodeGen
            AArch64AsmParser
            AArch64Desc
            AArch64Disassembler
            AArch64Info
            AArch64Utils
        )
    elseif(TARGET_ARCH STREQUAL "X86")
        list(APPEND llvm_components
            X86CodeGen
            X86AsmParser
            X86Desc
            X86Disassembler
            X86Info
            X86TargetMCA
        )
    elseif(TARGET_ARCH STREQUAL "ARM")
        list(APPEND llvm_components
            ARMCodeGen
            ARMAsmParser
            ARMDesc
            ARMDisassembler
            ARMInfo
            ARMUtils
        )
    elseif(TARGET_ARCH STREQUAL "RISCV")
        list(APPEND llvm_components
            RISCVCodeGen
            RISCVAsmParser
            RISCVDesc
            RISCVDisassembler
            RISCVInfo
            RISCVTargetMCA
        )
    elseif(TARGET_ARCH STREQUAL "Mips")
        list(APPEND llvm_components
            MipsCodeGen
            MipsAsmParser
            MipsDesc
            MipsDisassembler
            MipsInfo
        )
    elseif(TARGET_ARCH STREQUAL "PowerPC")
        list(APPEND llvm_components
            PowerPCCodeGen
            PowerPCAsmParser
            PowerPCDesc
            PowerPCDisassembler
            PowerPCInfo
        )
    elseif(TARGET_ARCH STREQUAL "Sparc")
        list(APPEND llvm_components
            SparcCodeGen
            SparcAsmParser
            SparcDesc
            SparcDisassembler
            SparcInfo
        )
    elseif(TARGET_ARCH STREQUAL "SystemZ")
        list(APPEND llvm_components
            SystemZCodeGen
            SystemZAsmParser
            SystemZDesc
            SystemZDisassembler
            SystemZInfo
        )
    elseif(TARGET_ARCH STREQUAL "WebAssembly")
        list(APPEND llvm_components
            WebAssemblyCodeGen
            WebAssemblyAsmParser
            WebAssemblyDesc
            WebAssemblyDisassembler
            WebAssemblyInfo
        )
    elseif(TARGET_ARCH STREQUAL "NVPTX")
        list(APPEND llvm_components
            NVPTXCodeGen
            NVPTXDesc
            NVPTXInfo
        )
    elseif(TARGET_ARCH STREQUAL "AMDGPU")
        list(APPEND llvm_components
            AMDGPUCodeGen
            AMDGPUAsmParser
            AMDGPUDesc
            AMDGPUDisassembler
            AMDGPUInfo
            AMDGPUUtils
        )
    elseif(TARGET_ARCH STREQUAL "Hexagon")
        list(APPEND llvm_components
            HexagonCodeGen
            HexagonAsmParser
            HexagonDesc
            HexagonDisassembler
            HexagonInfo
        )
    elseif(TARGET_ARCH STREQUAL "BPF")
        list(APPEND llvm_components
            BPFCodeGen
            BPFAsmParser
            BPFDesc
            BPFDisassembler
            BPFInfo
        )
    elseif(TARGET_ARCH STREQUAL "AVR")
        list(APPEND llvm_components
            AVRCodeGen
            AVRAssembler
            AVRAsmParser
            AVRDesc
            AVRDisassembler
            AVRInfo
        )
    elseif(TARGET_ARCH STREQUAL "MSP430")
        list(APPEND llvm_components
            MSP430CodeGen
            MSP430AsmParser
            MSP430Desc
            MSP430Disassembler
            MSP430Info
        )
    elseif(TARGET_ARCH STREQUAL "Lanai")
        list(APPEND llvm_components
            LanaiCodeGen
            LanaiAsmParser
            LanaiDesc
            LanaiDisassembler
            LanaiInfo
        )
    elseif(TARGET_ARCH STREQUAL "VE")
        list(APPEND llvm_components
            VECodeGen
            VEAsmParser
            VEDesc
            VEDisassembler
            VEInfo
        )
    elseif(TARGET_ARCH STREQUAL "LoongArch")
        list(APPEND llvm_components
            LoongArchCodeGen
            LoongArchAsmParser
            LoongArchDesc
            LoongArchDisassembler
            LoongArchInfo
        )
    elseif(TARGET_ARCH STREQUAL "XCore")
        list(APPEND llvm_components
            XCoreCodeGen
            XCoreDesc
            XCoreDisassembler
            XCoreInfo
        )
    endif()
    
    # Map components to library names
    llvm_map_components_to_libnames(llvm_libs ${llvm_components})
    
    message(STATUS "Configured LLVM libraries for ${TARGET_ARCH} target:")
    foreach(lib ${llvm_libs})
        message(STATUS "  - ${lib}")
    endforeach()
endif()

# Compiler flags for performance and debugging
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

# AddressSanitizer support
set(CMAKE_CXX_FLAGS_ASAN "-g -O1 -fsanitize=address -fno-omit-frame-pointer -DDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address")

# Warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Project directories
set(TSC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TSC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TSC_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Include directories
include_directories(${TSC_INCLUDE_DIR})

# Add subdirectories
add_subdirectory(src)

# Enable testing
enable_testing()
# add_subdirectory(test)  # Disabled - GTest not available

# Create runtime library
add_library(tsc_runtime STATIC
    runtime.c
    runtime_exceptions.c
    runtime/arc/arc_runtime.c
    runtime/arc/weak_references.c
    runtime/arc/cycle_detection.c
)

# Create main executable
add_executable(tsc
    src/main.cpp
)

target_link_libraries(tsc
    tsc_lib
    tsc_runtime
    ${llvm_libs}
)

# Install targets
install(TARGETS tsc
    RUNTIME DESTINATION bin
)
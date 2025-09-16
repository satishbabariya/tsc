/**
 * @file Common.h
 * @brief Core common definitions and utilities for the TSC compiler
 * @author TSC Compiler Team
 * @version 1.0
 * 
 * This file contains fundamental types, utilities, and configurations
 * used throughout the TSC compiler. It follows the static compilation
 * principles with manual memory management.
 */

#pragma once

// Standard library includes
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <array>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include <type_traits>
#include <utility>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <cstdint>
#include <cstddef>

// C++20 features (commented out for C++17 compatibility)
// #include <source_location>
// #include <concepts>
// #include <ranges>

namespace tsc {

// ============================================================================
// CORE TYPE DEFINITIONS
// ============================================================================

/// @brief String reference type for efficient string handling
using StringRef = std::string_view;

/// @brief Main string type used throughout the compiler
using String = std::string;

/// @brief Unsigned integer types for size and indexing
using Size = std::size_t;
using Index = std::ptrdiff_t;

/// @brief Byte type for memory operations
using Byte = std::uint8_t;

// ============================================================================
// MEMORY MANAGEMENT ALIASES
// ============================================================================

/// @brief Smart pointer aliases following TSC manual memory management
template<typename T>
using unique_ptr = std::unique_ptr<T>;

template<typename T>
using shared_ptr = std::shared_ptr<T>;

template<typename T>
using weak_ptr = std::weak_ptr<T>;

/// @brief Memory allocation helpers
using std::make_unique;
using std::make_shared;

// ============================================================================
// SOURCE LOCATION TRACKING
// ============================================================================

/**
 * @brief Represents a location in source code for error reporting
 * 
 * This class provides precise location information including filename,
 * line number, and column number for accurate error reporting and
 * debugging support.
 */
class SourceLocation {
public:
    /// @brief Default constructor for invalid locations
    SourceLocation() = default;
    
    /// @brief Construct with specific location information
    /// @param filename Source file name
    /// @param line Line number (1-based)
    /// @param column Column number (1-based)
    SourceLocation(const String& filename, unsigned line, unsigned column)
        : filename_(filename), line_(line), column_(column) {}
    
    /// @brief Construct from std::source_location (C++20 feature)
    /// @param loc Standard source location
    // explicit SourceLocation(const std::source_location& loc)
    //     : filename_(loc.file_name()), line_(loc.line()), column_(loc.column()) {}

    // Getters
    const String& getFilename() const noexcept { return filename_; }
    unsigned getLine() const noexcept { return line_; }
    unsigned getColumn() const noexcept { return column_; }
    
    /// @brief Check if this location is valid
    bool isValid() const noexcept { return !filename_.empty() && line_ > 0; }
    
    /// @brief Get formatted string representation
    String toString() const {
        if (!isValid()) return "<unknown location>";
        return filename_ + ":" + std::to_string(line_) + ":" + std::to_string(column_);
    }

private:
    String filename_;
    unsigned line_ = 0;
    unsigned column_ = 0;
};

// ============================================================================
// DIAGNOSTIC SYSTEM
// ============================================================================

/// @brief Diagnostic severity levels
enum class DiagnosticLevel : std::uint8_t {
    Note = 0,    ///< Informational message
    Warning = 1, ///< Warning that doesn't prevent compilation
    Error = 2,   ///< Error that prevents successful compilation
    Fatal = 3    ///< Fatal error that stops compilation immediately
};

/// @brief Convert diagnostic level to string
constexpr const char* toString(DiagnosticLevel level) noexcept {
    switch (level) {
        case DiagnosticLevel::Note:    return "note";
        case DiagnosticLevel::Warning: return "warning";
        case DiagnosticLevel::Error:   return "error";
        case DiagnosticLevel::Fatal:   return "fatal error";
        default: return "unknown";
    }
}

// ============================================================================
// TARGET CONFIGURATION
// ============================================================================

/**
 * @brief Target architecture and platform information
 * 
 * Contains all information needed to configure LLVM for the target
 * platform, including CPU features and optimization settings.
 */
struct TargetInfo {
    String triple;                    ///< LLVM target triple (e.g., "x86_64-pc-linux-gnu")
    String cpu;                       ///< Target CPU architecture
    std::vector<String> features;     ///< Target-specific features
    bool is64Bit = true;              ///< Whether target is 64-bit
    unsigned pointerSize = 8;          ///< Pointer size in bytes
    unsigned intSize = 32;            ///< Default integer size in bits
    bool littleEndian = true;         ///< Endianness
    
    /// @brief Check if target info is valid
    bool isValid() const noexcept {
        return !triple.empty() && !cpu.empty() && pointerSize > 0;
    }
};

// ============================================================================
// COMPILER OPTIONS
// ============================================================================

/**
 * @brief Comprehensive compiler configuration options
 * 
 * This class encapsulates all configurable options for the TSC compiler,
 * including optimization levels, target settings, and feature flags.
 */
class CompilerOptions {
public:
    // ========================================================================
    // INPUT/OUTPUT CONFIGURATION
    // ========================================================================
    
    std::vector<String> inputFiles;   ///< Source files to compile
    String outputFile;                ///< Output executable/library name
    String outputDir = ".";           ///< Output directory
    String tempDir = "/tmp";          ///< Temporary directory for intermediate files
    
    // ========================================================================
    // OPTIMIZATION CONFIGURATION
    // ========================================================================
    
    /// @brief Optimization levels
    enum class OptLevel : std::uint8_t {
        O0 = 0,  ///< No optimization
        O1 = 1,  ///< Basic optimization
        O2 = 2,  ///< Standard optimization
        O3 = 3,  ///< Aggressive optimization
        Os = 4,  ///< Optimize for size
        Oz = 5   ///< Maximum size optimization
    };
    
    OptLevel optimizationLevel = OptLevel::O0;  ///< Current optimization level
    
    // ========================================================================
    // DEBUG INFORMATION
    // ========================================================================
    
    bool generateDebugInfo = false;   ///< Generate debug information
    bool generateDWARF = true;        ///< Use DWARF debug format
    bool generateLineTables = true;   ///< Generate line number tables
    
    // ========================================================================
    // TARGET CONFIGURATION
    // ========================================================================
    
    TargetInfo target;                ///< Target platform information
    
    // ========================================================================
    // LANGUAGE FEATURES
    // ========================================================================
    
    bool strictMode = true;          ///< Enable strict TypeScript mode
    bool allowUnusedVariables = false; ///< Allow unused variable declarations
    bool enableTypeChecking = true;   ///< Enable static type checking
    bool enableInference = true;      ///< Enable type inference
    
    // ========================================================================
    // MEMORY MANAGEMENT
    // ========================================================================
    
    bool enableBoundsChecking = false;     ///< Enable array bounds checking (debug only)
    bool enableMemoryLeakDetection = false; ///< Enable memory leak detection (debug only)
    bool enableStackProtection = true;     ///< Enable stack protection
    
    // ========================================================================
    // PERFORMANCE CONFIGURATION
    // ========================================================================
    
    unsigned parallelJobs = 10;       ///< Number of parallel compilation jobs
    bool enableLTO = false;           ///< Enable Link Time Optimization
    bool enablePGO = false;           ///< Enable Profile Guided Optimization
    
    // ========================================================================
    // MODULE SYSTEM
    // ========================================================================
    
    std::vector<String> modulePaths;  ///< Module search paths
    bool separateCompilation = true;   ///< Enable separate compilation
    bool incrementalCompilation = false; ///< Enable incremental compilation
    
    // ========================================================================
    // ADVANCED FEATURES
    // ========================================================================
    
    bool enableCoroutines = true;     ///< Enable async/await support
    bool enableGenerics = true;       ///< Enable generic programming
    bool enableReflection = false;    ///< Enable runtime reflection
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    
    /// @brief Check if options are valid
    bool isValid() const noexcept {
        return !inputFiles.empty() && target.isValid();
    }
    
    /// @brief Get optimization level as string
    String getOptLevelString() const {
        switch (optimizationLevel) {
            case OptLevel::O0: return "O0";
            case OptLevel::O1: return "O1";
            case OptLevel::O2: return "O2";
            case OptLevel::O3: return "O3";
            case OptLevel::Os: return "Os";
            case OptLevel::Oz: return "Oz";
            default: return "O0";
        }
    }
};

// ============================================================================
// ERROR HANDLING
// ============================================================================

/**
 * @brief Base exception class for compiler errors
 * 
 * Provides structured error reporting with source location information
 * for better debugging and error messages.
 */
class CompilerError : public std::exception {
public:
    /// @brief Construct with error message and optional location
    /// @param message Error description
    /// @param loc Source location where error occurred
    CompilerError(const String& message, const SourceLocation& loc = {})
        : message_(message), location_(loc) {}
    
    /// @brief Get error message
    const char* what() const noexcept override { return message_.c_str(); }
    
    /// @brief Get source location
    const SourceLocation& getLocation() const noexcept { return location_; }
    
    /// @brief Get formatted error string
    String getFormattedMessage() const {
        if (location_.isValid()) {
            return location_.toString() + ": " + message_;
        }
        return message_;
    }

private:
    String message_;
    SourceLocation location_;
};

// ============================================================================
// UTILITY MACROS
// ============================================================================

/// @brief Assert macro with source location
#define TSC_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            throw CompilerError(message, SourceLocation(__FILE__, __LINE__, 0)); \
        } \
    } while (0)

/// @brief Unreachable code marker
#define TSC_UNREACHABLE() \
    do { \
        TSC_ASSERT(false, "Unreachable code reached"); \
        __builtin_unreachable(); \
    } while (0)

/// @brief Suppress unused variable warnings
#define TSC_UNUSED(x) (void)(x)

// ============================================================================
// TYPE TRAITS AND CONCEPTS
// ============================================================================

/// @brief Check if type is a smart pointer
template<typename T>
struct is_smart_ptr : std::false_type {};

template<typename T>
struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_ptr<std::weak_ptr<T>> : std::true_type {};

/// @brief SFINAE helper for types that can be converted to string
template<typename T>
struct is_string_convertible {
    template<typename U>
    static auto test(int) -> decltype(std::to_string(std::declval<U>()), std::true_type{});
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<T>(0))::value;
};

/// @brief SFINAE helper for AST node types
template<typename T>
struct is_ast_node {
    static constexpr bool value = std::is_base_of_v<class ASTNode, T>;
};

// ============================================================================
// CONSTANTS
// ============================================================================

/// @brief Maximum identifier length
constexpr Size MAX_IDENTIFIER_LENGTH = 255;

/// @brief Maximum nesting depth for recursive structures
constexpr Size MAX_NESTING_DEPTH = 1000;

/// @brief Default buffer size for I/O operations
constexpr Size DEFAULT_BUFFER_SIZE = 4096;

/// @brief Version information
constexpr const char* TSC_VERSION = "1.0.0";
constexpr const char* TSC_COPYRIGHT = "Copyright (c) 2024 TSC Compiler Team";

} // namespace tsc
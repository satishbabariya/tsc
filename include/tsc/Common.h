#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include <source_location>

namespace tsc {

// Forward declarations
class SourceLocation;
class DiagnosticEngine;
class CompilerOptions;

// Memory management aliases following TSC architecture
template<typename T>
using unique_ptr = std::unique_ptr<T>;

template<typename T>
using shared_ptr = std::shared_ptr<T>;

template<typename T>
using weak_ptr = std::weak_ptr<T>;

// String types optimized for compiler use
using StringRef = std::string_view;
using String = std::string;

// Source location tracking for error reporting
class SourceLocation {
public:
    SourceLocation() = default;
    SourceLocation(const String& filename, unsigned line, unsigned column)
        : filename_(filename), line_(line), column_(column) {}

    const String& getFilename() const { return filename_; }
    unsigned getLine() const { return line_; }
    unsigned getColumn() const { return column_; }
    
    bool isValid() const { return !filename_.empty(); }

private:
    String filename_;
    unsigned line_ = 0;
    unsigned column_ = 0;
};

// Diagnostic severity levels
enum class DiagnosticLevel {
    Note,
    Warning,
    Error,
    Fatal
};

// Compiler target information
struct TargetInfo {
    String triple;          // LLVM target triple (e.g., "x86_64-pc-linux-gnu")
    String cpu;             // Target CPU
    std::vector<String> features; // Target features
    bool is64Bit = true;
    unsigned pointerSize = 8; // in bytes
};

// Compilation options
class CompilerOptions {
public:
    // Input/Output
    std::vector<String> inputFiles;
    String outputFile;
    String outputDir = ".";
    
    // Optimization levels
    enum class OptLevel { O0, O1, O2, O3, Os, Oz };
    OptLevel optimizationLevel = OptLevel::O0;
    
    // Debug information
    bool generateDebugInfo = false;
    bool generateDWARF = true;
    
    // Target configuration
    TargetInfo target;
    
    // Language features
    bool strictMode = true;
    bool allowUnusedVariables = false;
    
    // Memory management
    bool enableBoundsChecking = false; // Debug builds only
    bool enableMemoryLeakDetection = false; // Debug builds only
    
    // Parallel compilation
    unsigned parallelJobs = 10;
    
    // Module system
    std::vector<String> modulePaths;
    bool separateCompilation = true;
    
    // Async/await support
    bool enableCoroutines = true;
};

// Error handling utilities
class CompilerError : public std::exception {
public:
    CompilerError(const String& message, const SourceLocation& loc = {})
        : message_(message), location_(loc) {}
    
    const char* what() const noexcept override { return message_.c_str(); }
    const SourceLocation& getLocation() const { return location_; }
    
private:
    String message_;
    SourceLocation location_;
};

// Utility macros for error handling
#define TSC_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            throw CompilerError(message, SourceLocation(__FILE__, __LINE__, 0)); \
        } \
    } while (0)

// Memory allocation helpers following manual memory management principles
// Use std:: versions to avoid ambiguity
using std::make_unique;
using std::make_shared;

} // namespace tsc
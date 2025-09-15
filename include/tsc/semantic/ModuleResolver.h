#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

namespace tsc {

// Module resolution result
struct ModuleResolutionResult {
    bool isSuccess;
    String resolvedPath;
    String errorMessage;
    
    // Default constructor for use in containers
    ModuleResolutionResult() : isSuccess(false), resolvedPath(""), errorMessage("") {}
    
    ModuleResolutionResult(bool success, const String& path, const String& error = "")
        : isSuccess(success), resolvedPath(path), errorMessage(error) {}
    
    static ModuleResolutionResult createSuccess(const String& path) {
        return ModuleResolutionResult(true, path);
    }
    
    static ModuleResolutionResult createFailure(const String& error) {
        return ModuleResolutionResult(false, "", error);
    }
};

// Module dependency information
struct ModuleDependency {
    String moduleSpecifier;    // Original specifier (e.g., "./math")
    String resolvedPath;       // Resolved file path
    SourceLocation location;   // Location of the import statement
    
    ModuleDependency(const String& specifier, const String& path, const SourceLocation& loc)
        : moduleSpecifier(specifier), resolvedPath(path), location(loc) {}
};

// Module information
struct ModuleInfo {
    String filePath;           // Absolute path to the module file
    std::vector<ModuleDependency> dependencies;  // Direct dependencies
    std::unordered_set<String> exportedSymbols;  // Symbols exported by this module
    std::unordered_set<String> importedSymbols;  // Symbols imported by this module
    
    ModuleInfo(const String& path) : filePath(path) {}
};

// Static module resolver for TypeScript modules
class ModuleResolver {
public:
    ModuleResolver(DiagnosticEngine& diagnostics);
    
    // Core resolution methods
    ModuleResolutionResult resolveModule(const String& moduleSpecifier, 
                                       const String& fromFile);
    
    // Dependency scanning
    std::vector<ModuleDependency> scanDependencies(const String& filePath);
    ModuleInfo analyzeModule(const String& filePath);
    
    // Path resolution utilities
    String resolveRelativePath(const String& moduleSpecifier, const String& fromFile);
    String resolveAbsolutePath(const String& moduleSpecifier);
    String normalizePath(const String& path);
    
    // File system utilities
    bool fileExists(const String& path);
    String findModuleFile(const String& basePath, const String& moduleName);
    
    // Error reporting
    void reportResolutionError(const String& specifier, const String& fromFile, 
                              const String& error);
    
    // Configuration
    void setBaseDirectory(const String& baseDir);
    void addSearchPath(const String& path);
    
private:
    DiagnosticEngine& diagnostics_;
    String baseDirectory_;
    std::vector<String> searchPaths_;
    
    // Cache for resolved modules to improve performance
    std::unordered_map<String, ModuleResolutionResult> resolutionCache_;
    
    // Helper methods
    String getDirectoryFromFile(const String& filePath);
    String appendFileExtension(const String& path);
    bool isValidModuleSpecifier(const String& specifier);
    void validatePath(const String& path);
};

} // namespace tsc
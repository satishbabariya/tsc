#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/ModuleResolver.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

namespace tsc {

// Dependency information for a single module
struct ModuleDependencyInfo {
    String modulePath;                    // Absolute path to the module file
    std::vector<String> directDependencies;  // Direct dependencies (absolute paths)
    std::unordered_set<String> exportedSymbols;  // Symbols exported by this module
    std::unordered_set<String> importedSymbols;  // Symbols imported by this module
    bool isScanned;                       // Whether this module has been scanned
    std::chrono::system_clock::time_point lastScanTime;  // When this module was last scanned
    
    ModuleDependencyInfo(const String& path) 
        : modulePath(path), isScanned(false), lastScanTime(std::chrono::system_clock::now()) {}
};

// Dependency graph representing relationships between modules
class DependencyGraph {
public:
    // Add a module to the graph
    void addModule(const String& modulePath);
    
    // Add a dependency relationship
    void addDependency(const String& fromModule, const String& toModule);
    
    // Get direct dependencies of a module
    const std::vector<String>& getDependencies(const String& modulePath) const;
    
    // Get all modules in the graph
    const std::unordered_set<String>& getModules() const { return modules_; }
    
    // Check if a module exists in the graph
    bool hasModule(const String& modulePath) const;
    
    // Get topological sort order for compilation
    std::vector<String> getCompilationOrder() const;
    
    // Check for circular dependencies
    bool hasCircularDependencies() const;
    std::vector<std::vector<String>> getCircularDependencies() const;
    
    // Enhanced circular dependency detection with detailed reporting
    struct CircularDependencyInfo {
        std::vector<String> cycle;
        String cycleDescription;  // Human-readable description of the cycle
        std::vector<String> involvedModules;  // All modules involved in the cycle
        
        CircularDependencyInfo(const std::vector<String>& c) : cycle(c) {
            buildDescription();
        }
        
    private:
        void buildDescription();
    };
    
    std::vector<CircularDependencyInfo> getDetailedCircularDependencies() const;
    void reportCircularDependencies(DiagnosticEngine& diagnostics) const;
    
    // Get dependency info for a module
    const ModuleDependencyInfo* getDependencyInfo(const String& modulePath) const;
    ModuleDependencyInfo* getDependencyInfo(const String& modulePath);
    
private:
    std::unordered_set<String> modules_;
    std::unordered_map<String, std::vector<String>> dependencies_;
    std::unordered_map<String, std::unique_ptr<ModuleDependencyInfo>> dependencyInfo_;
    
    // Helper methods for topological sort and cycle detection
    void topologicalSortDFS(const String& module, std::unordered_set<String>& visited,
                           std::unordered_set<String>& tempVisited, std::vector<String>& result) const;
    bool detectCycleDFS(const String& module, std::unordered_set<String>& visited,
                       std::unordered_set<String>& tempVisited, std::vector<String>& cycle) const;
    bool detectCycleDFSWithPath(const String& module, std::unordered_set<String>& visited,
                               std::unordered_set<String>& tempVisited, 
                               std::vector<String>& currentPath,
                               std::vector<String>& cycle) const;
};

// Dependency scanner for analyzing TypeScript files
class DependencyScanner {
public:
    DependencyScanner(ModuleResolver& moduleResolver, DiagnosticEngine& diagnostics);
    
    // Scan a single module for dependencies
    ModuleDependencyInfo scanModule(const String& modulePath);
    
    // Scan multiple modules and build dependency graph
    std::unique_ptr<DependencyGraph> scanProject(const std::vector<String>& modulePaths);
    
    // Scan project and validate for circular dependencies
    std::unique_ptr<DependencyGraph> scanProjectWithValidation(const std::vector<String>& modulePaths);
    
    // Get compilation order for a set of modules
    std::vector<String> getCompilationOrder(const std::vector<String>& modulePaths);
    
    // Check if a module needs to be rescanned (file changed)
    bool needsRescan(const String& modulePath) const;
    
    // Clear cache for a specific module
    void clearCache(const String& modulePath);
    
    // Clear all cached data
    void clearAllCache();
    
private:
    ModuleResolver& moduleResolver_;
    DiagnosticEngine& diagnostics_;
    
    // Cache for dependency information
    std::unordered_map<String, std::unique_ptr<ModuleDependencyInfo>> dependencyCache_;
    
    // Helper methods
    void extractImportsFromAST(Module& ast, const String& modulePath, 
                              ModuleDependencyInfo& info);
    void extractExportsFromAST(Module& ast, const String& modulePath,
                              ModuleDependencyInfo& info);
    std::chrono::system_clock::time_point getFileModificationTime(const String& filePath) const;
    bool isFileModified(const String& filePath, 
                       const std::chrono::system_clock::time_point& lastScanTime) const;
};

} // namespace tsc
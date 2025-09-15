#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>
#include "tsc/semantic/SymbolTable.h"

namespace tsc {
namespace semantic {

// Cycle detection annotation types
enum class CycleAnnotation {
    NONE,
    CYCLE_SAFE,
    CYCLE_UNSAFE,
    OWNERSHIP_HIERARCHY,
    OBSERVER_PATTERN,
    RESOURCE_MANAGER,
    CACHE_PATTERN,
    FACTORY_PATTERN
};

// Reference relationship types
enum class ReferenceType {
    STRONG,     // shared_ptr<T>
    WEAK,       // weak_ptr<T>
    UNIQUE,     // unique_ptr<T>
    RAW         // Raw pointer (dangerous)
};

// Object reference information
struct ObjectReference {
    std::string fieldName;
    std::string targetType;
    ReferenceType refType;
    int lineNumber;
    int columnNumber;
    
    ObjectReference(const std::string& name, const std::string& type, 
                   ReferenceType refType, int line = 0, int col = 0)
        : fieldName(name), targetType(type), refType(refType), lineNumber(line), columnNumber(col) {}
};

// Class information for cycle analysis
struct ClassInfo {
    std::string className;
    std::vector<ObjectReference> references;
    CycleAnnotation annotation;
    bool isOwnershipParent;
    bool isOwnershipChild;
    std::set<std::string> parentTypes;
    std::set<std::string> childTypes;
    
    ClassInfo(const std::string& name) 
        : className(name), annotation(CycleAnnotation::NONE), 
          isOwnershipParent(false), isOwnershipChild(false) {}
};

// Cycle detection result
struct CycleDetectionResult {
    bool hasCycle;
    std::vector<std::string> cyclePath;
    std::string cycleType;
    int severity; // 0 = info, 1 = warning, 2 = error
    std::string suggestion;
    int lineNumber;
    int columnNumber;
    
    CycleDetectionResult() : hasCycle(false), severity(0), lineNumber(0), columnNumber(0) {}
};

// Static cycle detector class
class CycleDetector {
private:
    std::map<std::string, std::shared_ptr<ClassInfo>> classMap_;
    std::vector<CycleDetectionResult> results_;
    SymbolTable* symbolTable_;
    
    // Helper methods
    CycleAnnotation parseAnnotation(const std::string& decoratorName);
    ReferenceType parseReferenceType(const std::string& typeName);
    void buildReferenceGraph();
    void detectCycles();
    void detectOwnershipViolations();
    void detectPatternViolations();
    
    // Cycle detection algorithms
    bool hasCycleDFS(const std::string& startClass, std::set<std::string>& visited, 
                     std::set<std::string>& recursionStack, std::vector<std::string>& path);
    
    // Pattern-specific analysis
    void analyzeOwnershipHierarchy();
    void analyzeObserverPattern();
    void analyzeResourceManager();
    void analyzeCachePattern();
    void analyzeFactoryPattern();
    
    // Utility methods
    std::string getTypeName(const std::string& typeName);
    bool isSmartPointerType(const std::string& typeName);
    bool isWeakPointerType(const std::string& typeName);
    void addResult(const CycleDetectionResult& result);
    
public:
    CycleDetector(SymbolTable* symbolTable);
    ~CycleDetector() = default;
    
    // Main analysis methods
    void analyzeSymbolTable();
    void analyzeFile(const std::string& filename);
    
    // Result access
    const std::vector<CycleDetectionResult>& getResults() const { return results_; }
    bool hasErrors() const;
    bool hasWarnings() const;
    
    // Reporting
    void printResults() const;
    void generateReport(const std::string& filename) const;
    
    // Configuration
    void setStrictMode(bool strict) { strictMode_ = strict; }
    void setVerboseMode(bool verbose) { verboseMode_ = verbose; }
    
private:
    bool strictMode_;
    bool verboseMode_;
};

} // namespace semantic
} // namespace tsc
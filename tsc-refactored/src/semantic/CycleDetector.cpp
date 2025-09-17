#include "tsc/semantic/CycleDetector.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>

namespace tsc {
    namespace semantic {
        CycleDetector::CycleDetector(SymbolTable *symbolTable)
            : symbolTable_(symbolTable), strictMode_(false), verboseMode_(false) {
        }

        void CycleDetector::analyzeSymbolTable() {
            if (verboseMode_) {
                std::cout << "🔍 Starting static cycle detection analysis..." << std::endl;
            }

            // Clear previous results
            results_.clear();
            classMap_.clear();

            // This is a placeholder implementation
            // In a real implementation, we'd traverse the symbol table and build class information

            if (verboseMode_) {
                std::cout << "📊 Analyzed " << classMap_.size() << " classes" << std::endl;
            }

            // Build reference graph and detect cycles
            buildReferenceGraph();
            detectCycles();
            detectOwnershipViolations();
            detectPatternViolations();

            if (verboseMode_) {
                std::cout << "✅ Static cycle detection completed" << std::endl;
            }
        }

        CycleAnnotation CycleDetector::parseAnnotation(const std::string &decoratorName) {
            if (decoratorName == "cycle_safe") return CycleAnnotation::CYCLE_SAFE;
            if (decoratorName == "cycle_unsafe") return CycleAnnotation::CYCLE_UNSAFE;
            if (decoratorName == "ownership_hierarchy") return CycleAnnotation::OWNERSHIP_HIERARCHY;
            if (decoratorName == "observer_pattern") return CycleAnnotation::OBSERVER_PATTERN;
            if (decoratorName == "resource_manager") return CycleAnnotation::RESOURCE_MANAGER;
            if (decoratorName == "cache_pattern") return CycleAnnotation::CACHE_PATTERN;
            if (decoratorName == "factory_pattern") return CycleAnnotation::FACTORY_PATTERN;
            return CycleAnnotation::NONE;
        }

        ReferenceType CycleDetector::parseReferenceType(const std::string &typeName) {
            if (typeName.find("shared_ptr<") != std::string::npos) return ReferenceType::STRONG;
            if (typeName.find("weak_ptr<") != std::string::npos) return ReferenceType::WEAK;
            if (typeName.find("unique_ptr<") != std::string::npos) return ReferenceType::UNIQUE;
            return ReferenceType::RAW;
        }

        std::string CycleDetector::getTypeName(const std::string &typeName) {
            // This is a simplified implementation
            return typeName;
        }

        void CycleDetector::buildReferenceGraph() {
            if (verboseMode_) {
                std::cout << "🔗 Building reference graph..." << std::endl;
            }

            // Build parent-child relationships based on annotations
            for (auto &[className, classInfo]: classMap_) {
                for (auto &ref: classInfo->references) {
                    if (ref.refType == ReferenceType::STRONG || ref.refType == ReferenceType::WEAK) {
                        // Extract target type from smart pointer
                        std::string targetType = ref.targetType;
                        size_t start = targetType.find('<');
                        size_t end = targetType.find('>');
                        if (start != std::string::npos && end != std::string::npos) {
                            targetType = targetType.substr(start + 1, end - start - 1);
                        }

                        if (classMap_.find(targetType) != classMap_.end()) {
                            if (ref.refType == ReferenceType::STRONG) {
                                classInfo->childTypes.insert(targetType);
                                classMap_[targetType]->parentTypes.insert(className);
                            }
                        }
                    }
                }
            }

            if (verboseMode_) {
                std::cout << "📊 Reference graph built with " << classMap_.size() << " classes" << std::endl;
            }
        }

        void CycleDetector::detectCycles() {
            if (verboseMode_) {
                std::cout << "🔍 Detecting reference cycles..." << std::endl;
            }

            std::set<std::string> visited;
            std::set<std::string> recursionStack;

            for (auto &[className, classInfo]: classMap_) {
                if (visited.find(className) == visited.end()) {
                    std::vector<std::string> path;
                    if (hasCycleDFS(className, visited, recursionStack, path)) {
                        // Cycle detected
                        CycleDetectionResult result;
                        result.hasCycle = true;
                        result.cyclePath = path;
                        result.cycleType = "Reference Cycle";
                        result.severity = 2; // Error
                        result.suggestion = "Use weak_ptr to break the cycle";
                        result.lineNumber = classInfo->references.empty() ? 0 : classInfo->references[0].lineNumber;
                        result.columnNumber = classInfo->references.empty() ? 0 : classInfo->references[0].columnNumber;

                        addResult(result);
                    }
                }
            }
        }

        bool CycleDetector::hasCycleDFS(const std::string &startClass,
                                        std::set<std::string> &visited,
                                        std::set<std::string> &recursionStack,
                                        std::vector<std::string> &path) {
            visited.insert(startClass);
            recursionStack.insert(startClass);
            path.push_back(startClass);

            auto classInfo = classMap_[startClass];
            for (auto &ref: classInfo->references) {
                if (ref.refType == ReferenceType::STRONG) {
                    std::string targetType = ref.targetType;
                    size_t start = targetType.find('<');
                    size_t end = targetType.find('>');
                    if (start != std::string::npos && end != std::string::npos) {
                        targetType = targetType.substr(start + 1, end - start - 1);
                    }

                    if (classMap_.find(targetType) != classMap_.end()) {
                        if (recursionStack.find(targetType) != recursionStack.end()) {
                            // Cycle found
                            return true;
                        }

                        if (visited.find(targetType) == visited.end()) {
                            if (hasCycleDFS(targetType, visited, recursionStack, path)) {
                                return true;
                            }
                        }
                    }
                }
            }

            recursionStack.erase(startClass);
            path.pop_back();
            return false;
        }

        void CycleDetector::detectOwnershipViolations() {
            if (verboseMode_) {
                std::cout << "🔍 Detecting ownership hierarchy violations..." << std::endl;
            }

            for (auto &[className, classInfo]: classMap_) {
                if (classInfo->annotation == CycleAnnotation::OWNERSHIP_HIERARCHY) {
                    // Check if this class has both parent and child references
                    bool hasParentRefs = false;
                    bool hasChildRefs = false;

                    for (auto &ref: classInfo->references) {
                        if (ref.refType == ReferenceType::STRONG) {
                            hasChildRefs = true;
                        } else if (ref.refType == ReferenceType::WEAK) {
                            hasParentRefs = true;
                        }
                    }

                    // In ownership hierarchy, children should use weak references to parents
                    if (hasChildRefs && !hasParentRefs) {
                        CycleDetectionResult result;
                        result.hasCycle = false;
                        result.cycleType = "Ownership Hierarchy Violation";
                        result.severity = 1; // Warning
                        result.suggestion = "Children should use weak_ptr to reference parents";
                        result.lineNumber = classInfo->references.empty() ? 0 : classInfo->references[0].lineNumber;
                        result.columnNumber = classInfo->references.empty() ? 0 : classInfo->references[0].columnNumber;

                        addResult(result);
                    }
                }
            }
        }

        void CycleDetector::detectPatternViolations() {
            if (verboseMode_) {
                std::cout << "🔍 Detecting pattern violations..." << std::endl;
            }

            // Analyze observer pattern
            analyzeObserverPattern();

            // Analyze resource manager pattern
            analyzeResourceManager();

            // Analyze cache pattern
            analyzeCachePattern();

            // Analyze factory pattern
            analyzeFactoryPattern();
        }

        void CycleDetector::analyzeObserverPattern() {
            for (auto &[className, classInfo]: classMap_) {
                if (classInfo->annotation == CycleAnnotation::OBSERVER_PATTERN) {
                    // In observer pattern, observers should use weak references to subjects
                    bool hasStrongRefs = false;
                    for (auto &ref: classInfo->references) {
                        if (ref.refType == ReferenceType::STRONG) {
                            hasStrongRefs = true;
                            break;
                        }
                    }

                    if (hasStrongRefs) {
                        CycleDetectionResult result;
                        result.hasCycle = false;
                        result.cycleType = "Observer Pattern Violation";
                        result.severity = 1; // Warning
                        result.suggestion = "Observers should use weak_ptr to reference subjects";
                        result.lineNumber = classInfo->references.empty() ? 0 : classInfo->references[0].lineNumber;
                        result.columnNumber = classInfo->references.empty() ? 0 : classInfo->references[0].columnNumber;

                        addResult(result);
                    }
                }
            }
        }

        void CycleDetector::analyzeResourceManager() {
            // Similar analysis for resource manager pattern
            // Resources should use weak references to managers
        }

        void CycleDetector::analyzeCachePattern() {
            // Similar analysis for cache pattern
            // Cache entries should use weak references to caches
        }

        void CycleDetector::analyzeFactoryPattern() {
            // Similar analysis for factory pattern
            // Prototypes should use weak references to factories
        }

        void CycleDetector::addResult(const CycleDetectionResult &result) {
            results_.push_back(result);
        }

        bool CycleDetector::hasErrors() const {
            return std::any_of(results_.begin(), results_.end(),
                               [](const CycleDetectionResult &r) { return r.severity == 2; });
        }

        bool CycleDetector::hasWarnings() const {
            return std::any_of(results_.begin(), results_.end(),
                               [](const CycleDetectionResult &r) { return r.severity == 1; });
        }

        void CycleDetector::printResults() const {
            std::cout << "\n🔍 Static Cycle Detection Results" << std::endl;
            std::cout << "===================================" << std::endl;

            if (results_.empty()) {
                std::cout << "✅ No cycles or violations detected!" << std::endl;
                return;
            }

            for (const auto &result: results_) {
                std::string severityStr;
                switch (result.severity) {
                    case 0: severityStr = "ℹ️  INFO";
                        break;
                    case 1: severityStr = "⚠️  WARNING";
                        break;
                    case 2: severityStr = "❌ ERROR";
                        break;
                    default: severityStr = "❓ UNKNOWN";
                        break;
                }

                std::cout << severityStr << ": " << result.cycleType << std::endl;
                if (!result.cyclePath.empty()) {
                    std::cout << "  Cycle path: ";
                    for (size_t i = 0; i < result.cyclePath.size(); i++) {
                        if (i > 0) std::cout << " -> ";
                        std::cout << result.cyclePath[i];
                    }
                    std::cout << std::endl;
                }
                std::cout << "  Suggestion: " << result.suggestion << std::endl;
                if (result.lineNumber > 0) {
                    std::cout << "  Location: line " << result.lineNumber << ", column " << result.columnNumber <<
                            std::endl;
                }
                std::cout << std::endl;
            }
        }

        void CycleDetector::generateReport(const std::string &filename) const {
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cerr << "❌ Failed to open report file: " << filename << std::endl;
                return;
            }

            file << "# Static Cycle Detection Report\n\n";
            file << "## Summary\n";
            file << "- Total issues: " << results_.size() << "\n";
            file << "- Errors: " << std::count_if(results_.begin(), results_.end(),
                                                  [](const CycleDetectionResult &r) {
                                                      return r.severity == 2;
                                                  }) << "\n";
            file << "- Warnings: " << std::count_if(results_.begin(), results_.end(),
                                                    [](const CycleDetectionResult &r) {
                                                        return r.severity == 1;
                                                    }) << "\n\n";

            file << "## Detailed Results\n\n";
            for (const auto &result: results_) {
                file << "### " << result.cycleType << "\n";
                file << "- Severity: " << result.severity << "\n";
                file << "- Suggestion: " << result.suggestion << "\n";
                if (result.lineNumber > 0) {
                    file << "- Location: line " << result.lineNumber << ", column " << result.columnNumber << "\n";
                }
                file << "\n";
            }

            file.close();
            std::cout << "📄 Report generated: " << filename << std::endl;
        }
    } // namespace semantic
} // namespace tsc

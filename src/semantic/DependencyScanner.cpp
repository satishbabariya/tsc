#include "tsc/semantic/DependencyScanner.h"
#include "tsc/parser/Parser.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/semantic/TypeSystem.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>

namespace tsc {
    // DependencyGraph implementation
    void DependencyGraph::addModule(const String &modulePath) {
        modules_.insert(modulePath);
        if (dependencyInfo_.find(modulePath) == dependencyInfo_.end()) {
            dependencyInfo_[modulePath] = std::make_unique<ModuleDependencyInfo>(modulePath);
        }
    }

    void DependencyGraph::addDependency(const String &fromModule, const String &toModule) {
        addModule(fromModule);
        addModule(toModule);

        auto &deps = dependencies_[fromModule];
        if (std::find(deps.begin(), deps.end(), toModule) == deps.end()) {
            deps.push_back(toModule);
        }
    }

    const std::vector<String> &DependencyGraph::getDependencies(const String &modulePath) const {
        static const std::vector<String> empty;
        auto it = dependencies_.find(modulePath);
        return (it != dependencies_.end()) ? it->second : empty;
    }

    bool DependencyGraph::hasModule(const String &modulePath) const {
        return modules_.find(modulePath) != modules_.end();
    }

    std::vector<String> DependencyGraph::getCompilationOrder() const {
        std::vector<String> result;
        std::unordered_set<String> visited;
        std::unordered_set<String> tempVisited;

        std::cout << "DEBUG: Computing compilation order for " << modules_.size() << " modules" << std::endl;
        for (const String &module: modules_) {
            std::cout << "DEBUG: Module: " << module << std::endl;
            auto it = dependencies_.find(module);
            if (it != dependencies_.end()) {
                std::cout << "DEBUG:   Dependencies: ";
                for (size_t i = 0; i < it->second.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << it->second[i];
                }
                std::cout << std::endl;
            } else {
                std::cout << "DEBUG:   No dependencies" << std::endl;
            }
        }

        for (const String &module: modules_) {
            if (visited.find(module) == visited.end()) {
                topologicalSortDFS(module, visited, tempVisited, result);
            }
        }

        std::cout << "DEBUG: Final compilation order: ";
        for (size_t i = 0; i < result.size(); ++i) {
            if (i > 0) std::cout << " -> ";
            std::cout << result[i];
        }
        std::cout << std::endl;

        return result;
    }

    bool DependencyGraph::hasCircularDependencies() const {
        std::cout << "DEBUG: hasCircularDependencies() called with " << modules_.size() << " modules" << std::endl;
        std::unordered_set<String> visited;
        std::unordered_set<String> tempVisited;
        std::vector<String> cycle;

        for (const String &module: modules_) {
            std::cout << "DEBUG: Checking module: " << module << std::endl;
            if (visited.find(module) == visited.end()) {
                if (detectCycleDFS(module, visited, tempVisited, cycle)) {
                    std::cout << "DEBUG: Cycle detected involving: " << module << std::endl;
                    return true;
                }
            }
        }

        std::cout << "DEBUG: No cycles detected" << std::endl;
        return false;
    }

    std::vector<std::vector<String> > DependencyGraph::getCircularDependencies() const {
        std::vector<std::vector<String> > cycles;
        std::unordered_set<String> visited;
        std::unordered_set<String> tempVisited;

        for (const String &module: modules_) {
            if (visited.find(module) == visited.end()) {
                std::vector<String> cycle;
                if (detectCycleDFS(module, visited, tempVisited, cycle)) {
                    cycles.push_back(cycle);
                }
            }
        }

        return cycles;
    }

    std::vector<DependencyGraph::CircularDependencyInfo> DependencyGraph::getDetailedCircularDependencies() const {
        std::vector<CircularDependencyInfo> detailedCycles;
        std::unordered_set<String> visited;
        std::unordered_set<String> tempVisited;
        std::unordered_set<String> cycleNodes; // Track nodes that are part of cycles

        for (const String &module: modules_) {
            if (visited.find(module) == visited.end()) {
                std::vector<String> currentPath;
                std::vector<String> cycle;
                if (detectCycleDFSWithPath(module, visited, tempVisited, currentPath, cycle)) {
                    // Check if this cycle is already represented by checking if any node is already in cycleNodes
                    bool isDuplicate = false;
                    for (const String &cycleNode: cycle) {
                        if (cycleNodes.find(cycleNode) != cycleNodes.end()) {
                            isDuplicate = true;
                            break;
                        }
                    }

                    if (!isDuplicate && !cycle.empty()) {
                        detailedCycles.emplace_back(cycle);
                        // Mark all nodes in this cycle as processed
                        for (const String &cycleNode: cycle) {
                            cycleNodes.insert(cycleNode);
                        }
                    }
                }
            }
        }

        return detailedCycles;
    }

    void DependencyGraph::reportCircularDependencies(DiagnosticEngine &diagnostics) const {
        auto detailedCycles = getDetailedCircularDependencies();

        if (detailedCycles.empty()) {
            return; // No circular dependencies found
        }

        for (size_t i = 0; i < detailedCycles.size(); ++i) {
            const auto &cycleInfo = detailedCycles[i];

            // Create a detailed error message
            String errorMsg = "Circular dependency detected";
            if (detailedCycles.size() > 1) {
                errorMsg += " (" + std::to_string(i + 1) + " of " + std::to_string(detailedCycles.size()) + ")";
            }
            errorMsg += ": " + cycleInfo.cycleDescription;

            // Report error for the first module in the cycle
            if (!cycleInfo.cycle.empty()) {
                diagnostics.error(errorMsg, SourceLocation(cycleInfo.cycle[0], 0, 0));
            }
        }
    }

    void DependencyGraph::CircularDependencyInfo::buildDescription() {
        if (cycle.empty()) {
            cycleDescription = "empty cycle";
            return;
        }

        // Build human-readable cycle description
        std::stringstream ss;
        for (size_t i = 0; i < cycle.size(); ++i) {
            if (i > 0) {
                ss << " → ";
            }
            // Extract just the filename from the full path for cleaner output
            String filename = cycle[i];
            size_t lastSlash = filename.find_last_of("/\\");
            if (lastSlash != String::npos) {
                filename = filename.substr(lastSlash + 1);
            }
            ss << filename;
        }

        // Add the cycle indicator
        if (cycle.size() > 1) {
            ss << " → " << cycle[0].substr(cycle[0].find_last_of("/\\") + 1);
        }

        cycleDescription = ss.str();

        // Build list of involved modules (deduplicated)
        std::unordered_set<String> uniqueModules(cycle.begin(), cycle.end());
        involvedModules.assign(uniqueModules.begin(), uniqueModules.end());
    }

    const ModuleDependencyInfo *DependencyGraph::getDependencyInfo(const String &modulePath) const {
        auto it = dependencyInfo_.find(modulePath);
        return (it != dependencyInfo_.end()) ? it->second.get() : nullptr;
    }

    ModuleDependencyInfo *DependencyGraph::getDependencyInfo(const String &modulePath) {
        auto it = dependencyInfo_.find(modulePath);
        return (it != dependencyInfo_.end()) ? it->second.get() : nullptr;
    }

    void DependencyGraph::topologicalSortDFS(const String &module, std::unordered_set<String> &visited,
                                             std::unordered_set<String> &tempVisited,
                                             std::vector<String> &result) const {
        if (tempVisited.find(module) != tempVisited.end()) {
            // Cycle detected, but we'll handle it in cycle detection
            return;
        }

        if (visited.find(module) != visited.end()) {
            return;
        }

        tempVisited.insert(module);

        // First add dependencies to the result
        auto it = dependencies_.find(module);
        if (it != dependencies_.end()) {
            for (const String &dep: it->second) {
                topologicalSortDFS(dep, visited, tempVisited, result);
            }
        }

        tempVisited.erase(module);
        visited.insert(module);

        // Add this module to the result (dependencies come first)
        result.push_back(module);
    }

    bool DependencyGraph::detectCycleDFS(const String &module, std::unordered_set<String> &visited,
                                         std::unordered_set<String> &tempVisited,
                                         std::vector<String> &cycle) const {
        if (tempVisited.find(module) != tempVisited.end()) {
            // Found a cycle - this module is already in the current path
            cycle.clear();
            cycle.push_back(module);
            return true;
        }

        if (visited.find(module) != visited.end()) {
            return false; // Already processed this module
        }

        // Mark as temporarily visited
        tempVisited.insert(module);

        // Recursively check dependencies
        auto it = dependencies_.find(module);
        if (it != dependencies_.end()) {
            for (const String &dep: it->second) {
                if (detectCycleDFS(dep, visited, tempVisited, cycle)) {
                    // Cycle found in dependency - add current module to cycle
                    cycle.push_back(module);
                    return true;
                }
            }
        }

        // Backtrack: mark as permanently visited
        tempVisited.erase(module);
        visited.insert(module);
        return false;
    }

    // Helper for cycle detection with path tracking (DFS)
    bool DependencyGraph::detectCycleDFSWithPath(const String &module, std::unordered_set<String> &visited,
                                                 std::unordered_set<String> &tempVisited,
                                                 std::vector<String> &currentPath,
                                                 std::vector<String> &cycle) const {
        if (tempVisited.find(module) != tempVisited.end()) {
            // Found a cycle - this module is already in the current path
            // Build the cycle path from the point where this module first appears in the path
            cycle.clear();

            // Find the index where this module first appears in the current path
            size_t cycleStartIndex = 0;
            for (size_t i = 0; i < currentPath.size(); ++i) {
                if (currentPath[i] == module) {
                    cycleStartIndex = i;
                    break;
                }
            }

            // Build the cycle from the cycle start to the end of the path
            for (size_t i = cycleStartIndex; i < currentPath.size(); ++i) {
                cycle.push_back(currentPath[i]);
            }

            // Add the current module to complete the cycle (it's the one causing the cycle)
            cycle.push_back(module);

            return true;
        }

        if (visited.find(module) != visited.end()) {
            return false; // Already processed this module
        }

        // Mark as temporarily visited and add to current path
        tempVisited.insert(module);
        currentPath.push_back(module);

        // Recursively check dependencies
        auto it = dependencies_.find(module);
        if (it != dependencies_.end()) {
            for (const String &dep: it->second) {
                if (detectCycleDFSWithPath(dep, visited, tempVisited, currentPath, cycle)) {
                    return true;
                }
            }
        }

        // Backtrack: remove from current path and mark as permanently visited
        currentPath.pop_back();
        tempVisited.erase(module);
        visited.insert(module);
        return false;
    }

    // DependencyScanner implementation
    DependencyScanner::DependencyScanner(ModuleResolver &moduleResolver, DiagnosticEngine &diagnostics)
        : moduleResolver_(moduleResolver), diagnostics_(diagnostics) {
    }

    ModuleDependencyInfo DependencyScanner::scanModule(const String &modulePath) {
        // Check cache first
        auto cached = dependencyCache_.find(modulePath);
        if (cached != dependencyCache_.end() && !needsRescan(modulePath)) {
            return *cached->second;
        }

        ModuleDependencyInfo info(modulePath);

        // Parse the file to get AST
        std::ifstream file(modulePath);
        if (!file.is_open()) {
            diagnostics_.error("Cannot open file for dependency scanning: " + modulePath,
                               SourceLocation(modulePath, 0, 0));
            return info;
        }

        // Read file content
        String content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Create lexer and parser
        Lexer lexer(diagnostics_);
        TypeSystem typeSystem;
        Parser parser(diagnostics_, typeSystem);

        try {
            auto tokens = lexer.tokenize(content, modulePath);
            auto module = parser.parse(tokens, modulePath);
            if (module) {
                extractImportsFromAST(*module, modulePath, info);
                extractExportsFromAST(*module, modulePath, info);
            }
        } catch (const std::exception &e) {
            diagnostics_.error("Failed to parse module for dependency scanning: " + String(e.what()),
                               SourceLocation(modulePath, 0, 0));
        }

        info.isScanned = true;
        info.lastScanTime = std::chrono::system_clock::now();

        // Cache the result
        dependencyCache_[modulePath] = std::make_unique<ModuleDependencyInfo>(info);

        return info;
    }

    std::unique_ptr<DependencyGraph> DependencyScanner::scanProject(const std::vector<String> &modulePaths) {
        auto graph = std::make_unique<DependencyGraph>();

        // First pass: scan all modules
        for (const String &modulePath: modulePaths) {
            ModuleDependencyInfo info = scanModule(modulePath);
            graph->addModule(modulePath);

            std::cout << "DEBUG: Module " << modulePath << " has " << info.directDependencies.size() << " dependencies:"
                    << std::endl;
            for (const String &dep: info.directDependencies) {
                std::cout << "  -> " << dep << std::endl;
                graph->addDependency(modulePath, dep);
            }
        }

        return graph;
    }

    std::unique_ptr<DependencyGraph> DependencyScanner::scanProjectWithValidation(
        const std::vector<String> &modulePaths) {
        auto graph = scanProject(modulePaths);

        // Check for circular dependencies and report them
        std::cout << "DEBUG: Checking for circular dependencies..." << std::endl;
        bool hasCycles = graph->hasCircularDependencies();
        std::cout << "DEBUG: hasCircularDependencies() returned: " << (hasCycles ? "true" : "false") << std::endl;

        if (hasCycles) {
            std::cout << "DEBUG: Reporting circular dependencies..." << std::endl;
            graph->reportCircularDependencies(diagnostics_);
        } else {
            std::cout << "DEBUG: No circular dependencies found" << std::endl;
        }

        return graph;
    }

    std::vector<String> DependencyScanner::getCompilationOrder(const std::vector<String> &modulePaths) {
        auto graph = scanProject(modulePaths);
        return graph->getCompilationOrder();
    }

    bool DependencyScanner::needsRescan(const String &modulePath) const {
        auto cached = dependencyCache_.find(modulePath);
        if (cached == dependencyCache_.end()) {
            return true;
        }

        return isFileModified(modulePath, cached->second->lastScanTime);
    }

    void DependencyScanner::clearCache(const String &modulePath) {
        dependencyCache_.erase(modulePath);
    }

    void DependencyScanner::clearAllCache() {
        dependencyCache_.clear();
    }

    void DependencyScanner::extractImportsFromAST(Module &ast, const String &modulePath,
                                                  ModuleDependencyInfo &info) {
        // Create a simple visitor to extract imports
        class ImportExtractor : public ASTVisitor {
        public:
            ImportExtractor(ModuleResolver &resolver, const String &fromFile,
                            ModuleDependencyInfo &info, DiagnosticEngine &diagnostics)
                : moduleResolver_(resolver), fromFile_(fromFile), info_(info),
                  diagnostics_(diagnostics) {
            }

            void visit(ImportDeclaration &node) override {
                ModuleResolutionResult result = moduleResolver_.resolveModule(
                    node.getModuleSpecifier(), fromFile_);

                if (result.isSuccess) {
                    info_.directDependencies.push_back(result.resolvedPath);

                    // Extract imported symbols
                    const auto &clause = node.getClause();
                    switch (clause.getType()) {
                        case ImportClause::Default:
                            info_.importedSymbols.insert(clause.getDefaultBinding());
                            break;
                        case ImportClause::Named: {
                            const auto &namedImports = clause.getNamedImports();
                            for (const auto &spec: namedImports) {
                                info_.importedSymbols.insert(spec.getLocalName());
                            }
                            break;
                        }
                        case ImportClause::Namespace:
                            info_.importedSymbols.insert(clause.getNamespaceBinding());
                            break;
                        case ImportClause::Mixed: {
                            info_.importedSymbols.insert(clause.getDefaultBinding());
                            const auto &namedImports = clause.getNamedImports();
                            for (const auto &spec: namedImports) {
                                info_.importedSymbols.insert(spec.getLocalName());
                            }
                            break;
                        }
                    }
                }
            }

            // Default implementations for other nodes
            void visit(NumericLiteral &node) override {
            }

            void visit(StringLiteral &node) override {
            }

            void visit(BooleanLiteral &node) override {
            }

            void visit(NullLiteral &node) override {
            }

            void visit(Identifier &node) override {
            }

            void visit(ThisExpression &node) override {
            }

            void visit(SuperExpression &node) override {
            }

            void visit(NewExpression &node) override {
            }

            void visit(BinaryExpression &node) override {
            }

            void visit(UnaryExpression &node) override {
            }

            void visit(AssignmentExpression &node) override {
            }

            void visit(ArrayAssignmentExpression &node) override {
            }

            void visit(CallExpression &node) override {
            }

            void visit(PropertyAccess &node) override {
            }

            void visit(IndexExpression &node) override {
            }

            void visit(ConditionalExpression &node) override {
            }

            void visit(FunctionExpression &node) override {
            }

            void visit(ArrowFunction &node) override {
            }

            void visit(ArrayLiteral &node) override {
            }

            void visit(ObjectLiteral &node) override {
            }

            void visit(TemplateLiteral &node) override {
            }

            void visit(DoWhileStatement &node) override {
            }

            void visit(ForOfStatement &node) override {
            }

            void visit(SwitchStatement &node) override {
            }

            void visit(CaseClause &node) override {
            }

            void visit(TypeParameter &node) override {
            }

            void visit(PropertyDeclaration &node) override {
            }

            void visit(MethodDeclaration &node) override {
            }

            void visit(EnumMember &node) override {
            }

            void visit(ExpressionStatement &node) override {
            }

            void visit(VariableDeclaration &node) override {
            }

            void visit(FunctionDeclaration &node) override {
            }

            void visit(ClassDeclaration &node) override {
            }

            void visit(InterfaceDeclaration &node) override {
            }

            void visit(EnumDeclaration &node) override {
            }

            void visit(TypeAliasDeclaration &node) override {
            }

            void visit(BlockStatement &node) override {
            }

            void visit(IfStatement &node) override {
            }

            void visit(WhileStatement &node) override {
            }

            void visit(ForStatement &node) override {
            }

            void visit(ReturnStatement &node) override {
            }

            void visit(BreakStatement &node) override {
            }

            void visit(ContinueStatement &node) override {
            }

            void visit(TryStatement &node) override {
            }

            void visit(CatchClause &node) override {
            }

            void visit(ThrowStatement &node) override {
            }

            void visit(ExportDeclaration &node) override {
            }

            void visit(MoveExpression &node) override {
            }

            void visit(DestructorDeclaration &node) override {
            }

            void visit(OptionalPropertyAccess &node) override {
            }

            void visit(OptionalIndexAccess &node) override {
            }

            void visit(OptionalCallExpr &node) override {
            }

            void visit(SpreadElement &node) override {
            }

            void visit(DestructuringPattern &node) override {
            }

            void visit(ArrayDestructuringPattern &node) override {
            }

            void visit(ObjectDestructuringPattern &node) override {
            }

            void visit(IdentifierPattern &node) override {
            }

            void visit(DestructuringAssignment &node) override {
            }

            void visit(Module &node) override {
                for (const auto &stmt: node.getStatements()) {
                    stmt->accept(*this);
                }
            }

        private:
            ModuleResolver &moduleResolver_;
            const String &fromFile_;
            ModuleDependencyInfo &info_;
            DiagnosticEngine &diagnostics_;
        };

        ImportExtractor extractor(moduleResolver_, modulePath, info, diagnostics_);
        ast.accept(extractor);
    }

    void DependencyScanner::extractExportsFromAST(Module &ast, const String &modulePath,
                                                  ModuleDependencyInfo &info) {
        // Create a simple visitor to extract exports
        class ExportExtractor : public ASTVisitor {
        public:
            ExportExtractor(ModuleResolver &resolver, const String &fromFile,
                            ModuleDependencyInfo &info, DiagnosticEngine &diagnostics)
                : moduleResolver_(resolver), fromFile_(fromFile), info_(info),
                  diagnostics_(diagnostics) {
            }

            void visit(ExportDeclaration &node) override {
                const auto &clause = node.getClause();
                switch (clause.getType()) {
                    case ExportClause::Default:
                        if (clause.getDefaultExport()) {
                            // Extract symbol name from default export
                            // This is simplified - in practice, we'd need to analyze the expression
                            info_.exportedSymbols.insert("default");
                        }
                        break;
                    case ExportClause::Named: {
                        const auto &namedExports = clause.getNamedExports();
                        for (const auto &spec: namedExports) {
                            info_.exportedSymbols.insert(spec.getExportedName());
                        }
                        break;
                    }
                    case ExportClause::ReExport:
                    case ExportClause::All:
                        // Re-exports - resolve the module and add its exports
                        if (!node.getModuleSpecifier().empty()) {
                            ModuleResolutionResult result = moduleResolver_.resolveModule(
                                node.getModuleSpecifier(), fromFile_);
                            if (result.isSuccess) {
                                info_.directDependencies.push_back(result.resolvedPath);
                            }
                        }
                        break;
                }
            }

            void visit(FunctionDeclaration &node) override {
                if (node.isExported()) {
                    info_.exportedSymbols.insert(node.getName());
                }
            }

            void visit(ClassDeclaration &node) override {
                if (node.isExported()) {
                    info_.exportedSymbols.insert(node.getName());
                }
            }

            void visit(VariableDeclaration &node) override {
                if (node.isExported()) {
                    info_.exportedSymbols.insert(node.getName());
                }
            }

            // Default implementations for other nodes
            void visit(NumericLiteral &node) override {
            }

            void visit(StringLiteral &node) override {
            }

            void visit(BooleanLiteral &node) override {
            }

            void visit(NullLiteral &node) override {
            }

            void visit(Identifier &node) override {
            }

            void visit(ThisExpression &node) override {
            }

            void visit(SuperExpression &node) override {
            }

            void visit(NewExpression &node) override {
            }

            void visit(BinaryExpression &node) override {
            }

            void visit(UnaryExpression &node) override {
            }

            void visit(AssignmentExpression &node) override {
            }

            void visit(ArrayAssignmentExpression &node) override {
            }

            void visit(CallExpression &node) override {
            }

            void visit(PropertyAccess &node) override {
            }

            void visit(IndexExpression &node) override {
            }

            void visit(ConditionalExpression &node) override {
            }

            void visit(FunctionExpression &node) override {
            }

            void visit(ArrowFunction &node) override {
            }

            void visit(ArrayLiteral &node) override {
            }

            void visit(ObjectLiteral &node) override {
            }

            void visit(TemplateLiteral &node) override {
            }

            void visit(DoWhileStatement &node) override {
            }

            void visit(ForOfStatement &node) override {
            }

            void visit(SwitchStatement &node) override {
            }

            void visit(CaseClause &node) override {
            }

            void visit(TypeParameter &node) override {
            }

            void visit(PropertyDeclaration &node) override {
            }

            void visit(MethodDeclaration &node) override {
            }

            void visit(EnumMember &node) override {
            }

            void visit(ExpressionStatement &node) override {
            }

            void visit(ImportDeclaration &node) override {
            }

            void visit(InterfaceDeclaration &node) override {
            }

            void visit(EnumDeclaration &node) override {
            }

            void visit(TypeAliasDeclaration &node) override {
            }

            void visit(BlockStatement &node) override {
            }

            void visit(IfStatement &node) override {
            }

            void visit(WhileStatement &node) override {
            }

            void visit(ForStatement &node) override {
            }

            void visit(ReturnStatement &node) override {
            }

            void visit(BreakStatement &node) override {
            }

            void visit(ContinueStatement &node) override {
            }

            void visit(TryStatement &node) override {
            }

            void visit(CatchClause &node) override {
            }

            void visit(ThrowStatement &node) override {
            }

            void visit(MoveExpression &node) override {
            }

            void visit(DestructorDeclaration &node) override {
            }

            void visit(Module &node) override {
                for (const auto &stmt: node.getStatements()) {
                    stmt->accept(*this);
                }
            }

            // Missing pure virtual method implementations
            void visit(OptionalPropertyAccess &node) override {
            }

            void visit(OptionalIndexAccess &node) override {
            }

            void visit(OptionalCallExpr &node) override {
            }

            void visit(SpreadElement &node) override {
            }

            void visit(DestructuringPattern &node) override {
            }

            void visit(ArrayDestructuringPattern &node) override {
            }

            void visit(ObjectDestructuringPattern &node) override {
            }

            void visit(IdentifierPattern &node) override {
            }

            void visit(DestructuringAssignment &node) override {
            }

        private:
            ModuleResolver &moduleResolver_;
            const String &fromFile_;
            ModuleDependencyInfo &info_;
            DiagnosticEngine &diagnostics_;
        };

        ExportExtractor extractor(moduleResolver_, modulePath, info, diagnostics_);
        ast.accept(extractor);
    }

    std::chrono::system_clock::time_point DependencyScanner::getFileModificationTime(const String &filePath) const {
        try {
            auto fileTime = std::filesystem::last_write_time(filePath);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
            return sctp;
        } catch (const std::exception &) {
            return std::chrono::system_clock::now();
        }
    }

    bool DependencyScanner::isFileModified(const String &filePath,
                                           const std::chrono::system_clock::time_point &lastScanTime) const {
        auto currentModTime = getFileModificationTime(filePath);
        return currentModTime > lastScanTime;
    }
} // namespace tsc

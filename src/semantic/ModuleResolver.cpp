#include "tsc/semantic/ModuleResolver.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace tsc {

ModuleResolver::ModuleResolver(DiagnosticEngine& diagnostics) 
    : diagnostics_(diagnostics) {
    // Initialize with current working directory as base
    baseDirectory_ = std::filesystem::current_path().string();
}

ModuleResolutionResult ModuleResolver::resolveModule(const String& moduleSpecifier, 
                                                   const String& fromFile) {
    // Check cache first
    String cacheKey = moduleSpecifier + "|" + fromFile;
    auto cached = resolutionCache_.find(cacheKey);
    if (cached != resolutionCache_.end()) {
        return cached->second;
    }
    
    // Validate module specifier
    if (!isValidModuleSpecifier(moduleSpecifier)) {
        String error = "Invalid module specifier: " + moduleSpecifier;
        reportResolutionError(moduleSpecifier, fromFile, error);
        ModuleResolutionResult result = ModuleResolutionResult::createFailure(error);
        resolutionCache_[cacheKey] = result;
        return result;
    }
    
    String resolvedPath;
    
    // Handle different types of module specifiers
    if (moduleSpecifier.substr(0, 2) == "./" || moduleSpecifier.substr(0, 3) == "../") {
        // Relative path
        resolvedPath = resolveRelativePath(moduleSpecifier, fromFile);
    } else if (moduleSpecifier.substr(0, 1) == "/") {
        // Absolute path
        resolvedPath = resolveAbsolutePath(moduleSpecifier);
    } else {
        // Invalid - we don't support package resolution
        String error = "Package resolution not supported. Only relative and absolute paths are allowed: " + moduleSpecifier;
        reportResolutionError(moduleSpecifier, fromFile, error);
        ModuleResolutionResult result = ModuleResolutionResult::createFailure(error);
        resolutionCache_[cacheKey] = result;
        return result;
    }
    
    // Validate the resolved path
    validatePath(resolvedPath);
    
    // Check if file exists
    if (!fileExists(resolvedPath)) {
        String error = "Module not found: " + resolvedPath;
        reportResolutionError(moduleSpecifier, fromFile, error);
        ModuleResolutionResult result = ModuleResolutionResult::createFailure(error);
        resolutionCache_[cacheKey] = result;
        return result;
    }
    
    ModuleResolutionResult result = ModuleResolutionResult::createSuccess(resolvedPath);
    resolutionCache_[cacheKey] = result;
    return result;
}

std::vector<ModuleDependency> ModuleResolver::scanDependencies(const String& filePath) {
    std::vector<ModuleDependency> dependencies;
    
    // Read the file and parse import statements
    std::ifstream file(filePath);
    if (!file.is_open()) {
        diagnostics_.error("Cannot open file for dependency scanning: " + filePath, SourceLocation(filePath, 0, 0));
        return dependencies;
    }
    
    // For now, we'll implement a simple text-based scanner
    // In a full implementation, this would use the parser to build an AST
    String line;
    unsigned lineNumber = 1;
    
    while (std::getline(file, line)) {
        // Look for import statements
        size_t importPos = line.find("import");
        if (importPos != String::npos) {
            // Find the 'from' keyword
            size_t fromPos = line.find("from", importPos);
            if (fromPos != String::npos) {
                // Extract module specifier
                size_t quoteStart = line.find("\"", fromPos);
                if (quoteStart != String::npos) {
                    size_t quoteEnd = line.find("\"", quoteStart + 1);
                    if (quoteEnd != String::npos) {
                        String moduleSpecifier = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                        
                        // Resolve the module
                        ModuleResolutionResult result = resolveModule(moduleSpecifier, filePath);
                        if (result.isSuccess) {
                            SourceLocation location(filePath, lineNumber, importPos + 1);
                            dependencies.emplace_back(moduleSpecifier, result.resolvedPath, location);
                        }
                    }
                }
            }
        }
        lineNumber++;
    }
    
    return dependencies;
}

ModuleInfo ModuleResolver::analyzeModule(const String& filePath) {
    ModuleInfo info(filePath);
    
    // Scan dependencies
    info.dependencies = scanDependencies(filePath);
    
    // TODO: Parse exports and imports from AST
    // For now, we'll implement basic text-based scanning
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        diagnostics_.error("Cannot open file for module analysis: " + filePath, SourceLocation(filePath, 0, 0));
        return info;
    }
    
    String line;
    while (std::getline(file, line)) {
        // Look for export statements
        if (line.find("export") != String::npos) {
            // Simple heuristic: look for identifiers after 'export'
            // This is a placeholder - full implementation would use AST parsing
            size_t exportPos = line.find("export");
            if (exportPos != String::npos) {
                // Extract symbol name (simplified)
                size_t start = line.find_first_not_of(" \t", exportPos + 6);
                if (start != String::npos) {
                    size_t end = line.find_first_of(" \t=;", start);
                    if (end != String::npos) {
                        String symbol = line.substr(start, end - start);
                        if (!symbol.empty() && symbol != "default" && symbol != "*") {
                            info.exportedSymbols.insert(symbol);
                        }
                    }
                }
            }
        }
        
        // Look for import statements
        if (line.find("import") != String::npos) {
            // Extract imported symbols (simplified)
            size_t importPos = line.find("import");
            if (importPos != String::npos) {
                size_t braceStart = line.find("{", importPos);
                if (braceStart != String::npos) {
                    size_t braceEnd = line.find("}", braceStart);
                    if (braceEnd != String::npos) {
                        String imports = line.substr(braceStart + 1, braceEnd - braceStart - 1);
                        // Parse comma-separated imports
                        size_t pos = 0;
                        while (pos < imports.length()) {
                            size_t commaPos = imports.find(",", pos);
                            size_t endPos = (commaPos == String::npos) ? imports.length() : commaPos;
                            
                            String symbol = imports.substr(pos, endPos - pos);
                            // Remove whitespace and 'as' aliases
                            size_t spacePos = symbol.find(" ");
                            if (spacePos != String::npos) {
                                symbol = symbol.substr(0, spacePos);
                            }
                            
                            if (!symbol.empty()) {
                                info.importedSymbols.insert(symbol);
                            }
                            
                            pos = (commaPos == String::npos) ? imports.length() : commaPos + 1;
                        }
                    }
                }
            }
        }
    }
    
    return info;
}

String ModuleResolver::resolveRelativePath(const String& moduleSpecifier, const String& fromFile) {
    String fromDir = getDirectoryFromFile(fromFile);
    std::filesystem::path basePath(fromDir);
    std::filesystem::path modulePath(moduleSpecifier);
    
    // Resolve relative path
    std::filesystem::path resolvedPath = basePath / modulePath;
    resolvedPath = resolvedPath.lexically_normal();
    
    return appendFileExtension(resolvedPath.string());
}

String ModuleResolver::resolveAbsolutePath(const String& moduleSpecifier) {
    std::filesystem::path modulePath(moduleSpecifier);
    modulePath = modulePath.lexically_normal();
    
    return appendFileExtension(modulePath.string());
}

String ModuleResolver::normalizePath(const String& path) {
    std::filesystem::path fsPath(path);
    return fsPath.lexically_normal().string();
}

bool ModuleResolver::fileExists(const String& path) {
    return std::filesystem::exists(path);
}

String ModuleResolver::findModuleFile(const String& basePath, const String& moduleName) {
    // Try different extensions
    std::vector<String> extensions = {".ts", ".d.ts"};
    
    for (const String& ext : extensions) {
        String fullPath = basePath + "/" + moduleName + ext;
        if (fileExists(fullPath)) {
            return fullPath;
        }
    }
    
    return "";
}

void ModuleResolver::reportResolutionError(const String& specifier, const String& fromFile, 
                                         const String& error) {
    diagnostics_.error(error, SourceLocation(fromFile, 0, 0));
}

void ModuleResolver::setBaseDirectory(const String& baseDir) {
    baseDirectory_ = baseDir;
}

void ModuleResolver::addSearchPath(const String& path) {
    searchPaths_.push_back(path);
}

String ModuleResolver::getDirectoryFromFile(const String& filePath) {
    std::filesystem::path path(filePath);
    return path.parent_path().string();
}

String ModuleResolver::appendFileExtension(const String& path) {
    // If path already has an extension, return as-is
    if (path.find('.') != String::npos) {
        return path;
    }
    
    // Try to find the file with .ts extension first
    String tsPath = path + ".ts";
    if (fileExists(tsPath)) {
        return tsPath;
    }
    
    // Try .d.ts extension
    String dtsPath = path + ".d.ts";
    if (fileExists(dtsPath)) {
        return dtsPath;
    }
    
    // Return original path if no extension found
    return path;
}

bool ModuleResolver::isValidModuleSpecifier(const String& specifier) {
    // Check for basic validity
    if (specifier.empty()) {
        return false;
    }
    
    // Check for invalid characters
    if (specifier.find("..") != String::npos && specifier != "..") {
        // Allow single ".." but not "..." or multiple consecutive dots
        return false;
    }
    
    return true;
}

void ModuleResolver::validatePath(const String& path) {
    // Check for directory traversal attacks
    if (path.find("../") != String::npos || path.find("..\\") != String::npos) {
        diagnostics_.error("Invalid path: directory traversal not allowed: " + path, SourceLocation(path, 0, 0));
    }
    
    // Check for absolute paths outside base directory
    if (path.substr(0, 1) == "/" && path.substr(0, baseDirectory_.length()) != baseDirectory_) {
        diagnostics_.error("Invalid path: outside base directory: " + path, SourceLocation(path, 0, 0));
    }
}

} // namespace tsc
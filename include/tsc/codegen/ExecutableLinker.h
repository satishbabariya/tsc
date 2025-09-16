#pragma once

#include "tsc/Common.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <vector>
#include <memory>

namespace tsc {

class ExecutableLinker {
public:
    explicit ExecutableLinker(DiagnosticEngine& diagnostics);
    ~ExecutableLinker();

    bool linkExecutable(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple);
    
    void setRuntimeLibraryPath(const String& path) { runtimeLibraryPath_ = path; }
    void setSystemLibraries(const std::vector<String>& libraries) { systemLibraries_ = libraries; }

private:
    DiagnosticEngine& diagnostics_;
    String runtimeLibraryPath_;
    std::vector<String> systemLibraries_;
    
    bool linkWithLLD(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple);
    bool linkWithSystemLinker(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple);
    String findRuntimeLibrary();
    void reportError(const String& message);
};

} // namespace tsc

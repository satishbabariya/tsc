#pragma once

#include "tsc/Common.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/ErrorCodes.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace tsc {

// Enhanced error reporting system with comprehensive error handling
class EnhancedErrorReporting {
public:
    // Constructor
    EnhancedErrorReporting(DiagnosticEngine& diagnostics);
    
    // Destructor
    ~EnhancedErrorReporting() = default;
    
    // Syntax error reporting
    void reportSyntaxError(const String& code, const SourceLocation& location,
                          const String& message, const String& suggestion = "");
    void reportUnexpectedToken(const SourceLocation& location, const String& expected,
                              const String& found, const String& suggestion = "");
    void reportExpectedToken(const SourceLocation& location, const String& expected,
                           const String& found, const String& suggestion = "");
    void reportMissingSemicolon(const SourceLocation& location, const String& suggestion = "");
    void reportMissingBrace(const SourceLocation& location, const String& braceType,
                           const String& suggestion = "");
    void reportMissingParenthesis(const SourceLocation& location, const String& parenType,
                                 const String& suggestion = "");
    void reportMissingBracket(const SourceLocation& location, const String& bracketType,
                             const String& suggestion = "");
    void reportInvalidExpression(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidStatement(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidDeclaration(const SourceLocation& location, const String& suggestion = "");
    void reportDuplicateDeclaration(const SourceLocation& location, const String& name,
                                   const String& suggestion = "");
    void reportInvalidForLoop(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidSwitchCase(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidTryCatch(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidFunctionSignature(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidClassDeclaration(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidInterfaceDeclaration(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidEnumDeclaration(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidTypeAlias(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidDestructuring(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidSpreadOperator(const SourceLocation& location, const String& suggestion = "");
    
    // Type error reporting
    void reportTypeError(const String& code, const SourceLocation& location,
                        const String& message, const String& suggestion = "");
    void reportTypeMismatch(const SourceLocation& location, const String& expected,
                           const String& actual, const String& suggestion = "");
    void reportUndefinedVariable(const SourceLocation& location, const String& name,
                                const String& suggestion = "");
    void reportUndefinedFunction(const SourceLocation& location, const String& name,
                                const String& suggestion = "");
    void reportUndefinedProperty(const SourceLocation& location, const String& property,
                                const String& type, const String& suggestion = "");
    void reportUndefinedType(const SourceLocation& location, const String& type,
                            const String& suggestion = "");
    void reportIncompatibleTypes(const SourceLocation& location, const String& type1,
                                const String& type2, const String& suggestion = "");
    void reportMissingTypeAnnotation(const SourceLocation& location, const String& name,
                                    const String& suggestion = "");
    void reportInvalidTypeAnnotation(const SourceLocation& location, const String& annotation,
                                    const String& suggestion = "");
    void reportCircularTypeReference(const SourceLocation& location, const String& suggestion = "");
    void reportGenericTypeError(const SourceLocation& location, const String& generic,
                               const String& suggestion = "");
    void reportUnionTypeError(const SourceLocation& location, const String& unionType,
                             const String& suggestion = "");
    void reportIntersectionTypeError(const SourceLocation& location, const String& intersection,
                                    const String& suggestion = "");
    void reportArrayTypeError(const SourceLocation& location, const String& array,
                             const String& suggestion = "");
    void reportObjectTypeError(const SourceLocation& location, const String& object,
                              const String& suggestion = "");
    void reportFunctionTypeError(const SourceLocation& location, const String& function,
                                const String& suggestion = "");
    void reportClassTypeError(const SourceLocation& location, const String& class_,
                             const String& suggestion = "");
    void reportInterfaceTypeError(const SourceLocation& location, const String& interface,
                                 const String& suggestion = "");
    void reportEnumTypeError(const SourceLocation& location, const String& enum_,
                            const String& suggestion = "");
    void reportTupleTypeError(const SourceLocation& location, const String& tuple,
                             const String& suggestion = "");
    void reportLiteralTypeError(const SourceLocation& location, const String& literal,
                               const String& suggestion = "");
    
    // Semantic error reporting
    void reportSemanticError(const String& code, const SourceLocation& location,
                            const String& message, const String& suggestion = "");
    void reportDuplicateIdentifier(const SourceLocation& location, const String& name,
                                  const String& suggestion = "");
    void reportCannotFindName(const SourceLocation& location, const String& name,
                             const String& suggestion = "");
    void reportCannotFindModule(const SourceLocation& location, const String& module,
                               const String& suggestion = "");
    void reportCannotFindNamespace(const SourceLocation& location, const String& namespace_,
                                  const String& suggestion = "");
    void reportCannotFindType(const SourceLocation& location, const String& type,
                             const String& suggestion = "");
    void reportCannotFindSymbol(const SourceLocation& location, const String& symbol,
                               const String& suggestion = "");
    void reportCannotFindFunction(const SourceLocation& location, const String& function,
                                 const String& suggestion = "");
    void reportCannotFindClass(const SourceLocation& location, const String& class_,
                              const String& suggestion = "");
    void reportCannotFindInterface(const SourceLocation& location, const String& interface,
                                  const String& suggestion = "");
    void reportCannotFindEnum(const SourceLocation& location, const String& enum_,
                             const String& suggestion = "");
    void reportCannotFindVariable(const SourceLocation& location, const String& variable,
                                 const String& suggestion = "");
    void reportCannotFindProperty(const SourceLocation& location, const String& property,
                                 const String& suggestion = "");
    void reportCannotFindMethod(const SourceLocation& location, const String& method,
                               const String& suggestion = "");
    void reportCannotFindConstructor(const SourceLocation& location, const String& constructor,
                                    const String& suggestion = "");
    void reportCannotFindDestructor(const SourceLocation& location, const String& destructor,
                                   const String& suggestion = "");
    void reportCannotFindAccessor(const SourceLocation& location, const String& accessor,
                                 const String& suggestion = "");
    void reportCannotFindParameter(const SourceLocation& location, const String& parameter,
                                  const String& suggestion = "");
    void reportCannotFindArgument(const SourceLocation& location, const String& argument,
                                 const String& suggestion = "");
    void reportCannotFindReturnType(const SourceLocation& location, const String& returnType,
                                   const String& suggestion = "");
    void reportCannotFindTypeParameter(const SourceLocation& location, const String& typeParam,
                                      const String& suggestion = "");
    
    // Code generation error reporting
    void reportCodeGenError(const String& code, const SourceLocation& location,
                           const String& message, const String& suggestion = "");
    void reportLLVMGenerationError(const SourceLocation& location, const String& error,
                                   const String& suggestion = "");
    void reportLLVMOptimizationError(const SourceLocation& location, const String& error,
                                    const String& suggestion = "");
    void reportLLVMLinkingError(const SourceLocation& location, const String& error,
                                const String& suggestion = "");
    void reportLLVMTargetError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMMemoryError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMTypeError(const SourceLocation& location, const String& error,
                            const String& suggestion = "");
    void reportLLVMFunctionError(const SourceLocation& location, const String& error,
                                const String& suggestion = "");
    void reportLLVMBlockError(const SourceLocation& location, const String& error,
                             const String& suggestion = "");
    void reportLLVMInstructionError(const SourceLocation& location, const String& error,
                                   const String& suggestion = "");
    void reportLLVMConstantError(const SourceLocation& location, const String& error,
                                const String& suggestion = "");
    void reportLLVMGlobalError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMLocalError(const SourceLocation& location, const String& error,
                             const String& suggestion = "");
    void reportLLVMPointerError(const SourceLocation& location, const String& error,
                               const String& suggestion = "");
    void reportLLVMArrayError(const SourceLocation& location, const String& error,
                             const String& suggestion = "");
    void reportLLVMStructError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMUnionError(const SourceLocation& location, const String& error,
                             const String& suggestion = "");
    void reportLLVMVectorError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMMetadataError(const SourceLocation& location, const String& error,
                                const String& suggestion = "");
    void reportLLVMDebugError(const SourceLocation& location, const String& error,
                              const String& suggestion = "");
    void reportLLVMVerificationError(const SourceLocation& location, const String& error,
                                    const String& suggestion = "");
    
    // Runtime error reporting
    void reportRuntimeError(const String& code, const SourceLocation& location,
                           const String& message, const String& suggestion = "");
    void reportDivisionByZero(const SourceLocation& location, const String& suggestion = "");
    void reportArrayIndexOutOfBounds(const SourceLocation& location, const String& index,
                                    const String& suggestion = "");
    void reportNullPointerDereference(const SourceLocation& location, const String& suggestion = "");
    void reportMemoryAccessViolation(const SourceLocation& location, const String& suggestion = "");
    void reportStackOverflow(const SourceLocation& location, const String& suggestion = "");
    void reportHeapOverflow(const SourceLocation& location, const String& suggestion = "");
    void reportMemoryLeak(const SourceLocation& location, const String& suggestion = "");
    void reportDoubleFree(const SourceLocation& location, const String& suggestion = "");
    void reportUseAfterFree(const SourceLocation& location, const String& suggestion = "");
    void reportBufferOverflow(const SourceLocation& location, const String& suggestion = "");
    void reportIntegerOverflow(const SourceLocation& location, const String& suggestion = "");
    void reportFloatingPointException(const SourceLocation& location, const String& suggestion = "");
    void reportInvalidCast(const SourceLocation& location, const String& from, const String& to,
                          const String& suggestion = "");
    void reportInvalidConversion(const SourceLocation& location, const String& from, const String& to,
                               const String& suggestion = "");
    void reportInvalidOperation(const SourceLocation& location, const String& operation,
                              const String& suggestion = "");
    void reportInvalidArgument(const SourceLocation& location, const String& argument,
                              const String& suggestion = "");
    void reportInvalidReturn(const SourceLocation& location, const String& returnValue,
                            const String& suggestion = "");
    void reportInvalidCall(const SourceLocation& location, const String& call,
                          const String& suggestion = "");
    void reportInvalidAccess(const SourceLocation& location, const String& access,
                            const String& suggestion = "");
    void reportInvalidAssignment(const SourceLocation& location, const String& assignment,
                                const String& suggestion = "");
    
    // Warning reporting
    void reportWarning(const String& code, const SourceLocation& location,
                      const String& message, const String& suggestion = "");
    void reportUnusedVariable(const SourceLocation& location, const String& variable,
                             const String& suggestion = "");
    void reportUnusedFunction(const SourceLocation& location, const String& function,
                             const String& suggestion = "");
    void reportUnusedParameter(const SourceLocation& location, const String& parameter,
                              const String& suggestion = "");
    void reportUnusedType(const SourceLocation& location, const String& type,
                         const String& suggestion = "");
    void reportUnusedImport(const SourceLocation& location, const String& import,
                           const String& suggestion = "");
    void reportUnusedExport(const SourceLocation& location, const String& export_,
                           const String& suggestion = "");
    void reportUnreachableCode(const SourceLocation& location, const String& suggestion = "");
    void reportDeadCode(const SourceLocation& location, const String& suggestion = "");
    void reportImplicitAny(const SourceLocation& location, const String& suggestion = "");
    void reportImplicitReturn(const SourceLocation& location, const String& suggestion = "");
    void reportImplicitConversion(const SourceLocation& location, const String& from, const String& to,
                                 const String& suggestion = "");
    void reportImplicitCast(const SourceLocation& location, const String& from, const String& to,
                           const String& suggestion = "");
    void reportImplicitAssignment(const SourceLocation& location, const String& assignment,
                                 const String& suggestion = "");
    void reportImplicitDeclaration(const SourceLocation& location, const String& declaration,
                                  const String& suggestion = "");
    void reportImplicitDefinition(const SourceLocation& location, const String& definition,
                                 const String& suggestion = "");
    void reportImplicitInstantiation(const SourceLocation& location, const String& instantiation,
                                    const String& suggestion = "");
    void reportImplicitSpecialization(const SourceLocation& location, const String& specialization,
                                     const String& suggestion = "");
    void reportImplicitTemplate(const SourceLocation& location, const String& template_,
                               const String& suggestion = "");
    void reportImplicitGeneric(const SourceLocation& location, const String& generic,
                              const String& suggestion = "");
    void reportImplicitType(const SourceLocation& location, const String& type,
                           const String& suggestion = "");
    
    // Info reporting
    void reportInfo(const String& code, const SourceLocation& location,
                   const String& message, const String& suggestion = "");
    void reportCompilationStarted(const SourceLocation& location, const String& filename,
                                 const String& suggestion = "");
    void reportCompilationCompleted(const SourceLocation& location, const String& suggestion = "");
    void reportOptimizationApplied(const SourceLocation& location, const String& optimization,
                                  const String& suggestion = "");
    void reportDebugInfoGenerated(const SourceLocation& location, const String& suggestion = "");
    void reportSourceMapGenerated(const SourceLocation& location, const String& suggestion = "");
    void reportMetadataGenerated(const SourceLocation& location, const String& suggestion = "");
    void reportSymbolTableBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportDependencyGraphBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportCallGraphBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportControlFlowGraphBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportDataFlowGraphBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportInterferenceGraphBuilt(const SourceLocation& location, const String& suggestion = "");
    void reportRegisterAllocationDone(const SourceLocation& location, const String& suggestion = "");
    void reportInstructionSchedulingDone(const SourceLocation& location, const String& suggestion = "");
    void reportPeepholeOptimizationDone(const SourceLocation& location, const String& suggestion = "");
    void reportLoopOptimizationDone(const SourceLocation& location, const String& suggestion = "");
    void reportFunctionInliningDone(const SourceLocation& location, const String& suggestion = "");
    void reportConstantPropagationDone(const SourceLocation& location, const String& suggestion = "");
    void reportDeadCodeEliminationDone(const SourceLocation& location, const String& suggestion = "");
    void reportCommonSubexpressionEliminationDone(const SourceLocation& location, const String& suggestion = "");
    
    // Utility methods
    void setSourceContent(const String& filename, const String& content);
    void setMaxErrors(size_t maxErrors);
    void setWarningsAsErrors(bool warningsAsErrors);
    void setSuppressWarnings(bool suppressWarnings);
    void suppressWarning(const String& code);
    void promoteWarningToError(const String& code);
    
    // Query methods
    bool hasErrors() const;
    bool hasWarnings() const;
    size_t getErrorCount() const;
    size_t getWarningCount() const;
    size_t getTotalCount() const;
    
    // Diagnostic access
    const std::vector<Diagnostic>& getDiagnostics() const;
    void clearDiagnostics();
    
    // Output methods
    void printDiagnostics() const;
    void printSummary() const;
    String formatDiagnostic(const Diagnostic& diagnostic) const;
    
private:
    DiagnosticEngine& diagnostics_;
    
    // Helper methods
    String formatMessage(const String& template_, const String& arg1 = "", const String& arg2 = "");
    String formatSuggestion(const String& template_, const String& arg1 = "", const String& arg2 = "");
    void addDiagnostic(DiagnosticLevel level, const String& code, const SourceLocation& location,
                      const String& message, const String& suggestion = "");
};

// Convenience macros for enhanced error reporting
#define TSC_SYNTAX_ERROR(reporter, code, location, message, suggestion) \
    reporter.reportSyntaxError(code, location, message, suggestion)

#define TSC_TYPE_ERROR(reporter, code, location, message, suggestion) \
    reporter.reportTypeError(code, location, message, suggestion)

#define TSC_SEMANTIC_ERROR(reporter, code, location, message, suggestion) \
    reporter.reportSemanticError(code, location, message, suggestion)

#define TSC_CODEGEN_ERROR(reporter, code, location, message, suggestion) \
    reporter.reportCodeGenError(code, location, message, suggestion)

#define TSC_RUNTIME_ERROR(reporter, code, location, message, suggestion) \
    reporter.reportRuntimeError(code, location, message, suggestion)

#define TSC_WARNING(reporter, code, location, message, suggestion) \
    reporter.reportWarning(code, location, message, suggestion)

#define TSC_INFO(reporter, code, location, message, suggestion) \
    reporter.reportInfo(code, location, message, suggestion)

} // namespace tsc
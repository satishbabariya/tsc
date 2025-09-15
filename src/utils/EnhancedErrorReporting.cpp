#include "tsc/utils/EnhancedErrorReporting.h"
#include <sstream>
#include <iomanip>

namespace tsc {

EnhancedErrorReporting::EnhancedErrorReporting(DiagnosticEngine& diagnostics)
    : diagnostics_(diagnostics) {
}

// Syntax error reporting methods
void EnhancedErrorReporting::reportSyntaxError(const String& code, const SourceLocation& location,
                                              const String& message, const String& suggestion) {
    addDiagnostic(DiagnosticLevel::Error, code, location, message, suggestion);
}

void EnhancedErrorReporting::reportUnexpectedToken(const SourceLocation& location, const String& expected,
                                                   const String& found, const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::UNEXPECTED_TOKEN_MSG, found, expected);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Syntax::UNEXPECTED_TOKEN_SUGGESTION, expected) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::UNEXPECTED_TOKEN, location, message, sug);
}

void EnhancedErrorReporting::reportExpectedToken(const SourceLocation& location, const String& expected,
                                                const String& found, const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::EXPECTED_TOKEN_MSG, expected, found);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Syntax::EXPECTED_TOKEN_SUGGESTION, expected) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::EXPECTED_TOKEN, location, message, sug);
}

void EnhancedErrorReporting::reportMissingSemicolon(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::MISSING_SEMICOLON_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::MISSING_SEMICOLON_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::MISSING_SEMICOLON, location, message, sug);
}

void EnhancedErrorReporting::reportMissingBrace(const SourceLocation& location, const String& braceType,
                                               const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::MISSING_BRACE_MSG, braceType);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Syntax::MISSING_BRACE_SUGGESTION, braceType) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::MISSING_BRACE, location, message, sug);
}

void EnhancedErrorReporting::reportMissingParenthesis(const SourceLocation& location, const String& parenType,
                                                     const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::MISSING_PARENTHESIS_MSG, parenType);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Syntax::MISSING_PARENTHESIS_SUGGESTION, parenType) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::MISSING_PARENTHESIS, location, message, sug);
}

void EnhancedErrorReporting::reportMissingBracket(const SourceLocation& location, const String& bracketType,
                                                  const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::MISSING_BRACKET_MSG, bracketType);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Syntax::MISSING_BRACKET_SUGGESTION, bracketType) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::MISSING_BRACKET, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidExpression(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_EXPRESSION_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_EXPRESSION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_EXPRESSION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidStatement(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_STATEMENT_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_STATEMENT_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_STATEMENT, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidDeclaration(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_DECLARATION_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_DECLARATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_DECLARATION, location, message, sug);
}

void EnhancedErrorReporting::reportDuplicateDeclaration(const SourceLocation& location, const String& name,
                                                        const String& suggestion) {
    String message = formatMessage(ErrorMessages::Syntax::DUPLICATE_DECLARATION_MSG, name);
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::DUPLICATE_DECLARATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::DUPLICATE_DECLARATION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidForLoop(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_FOR_LOOP_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_FOR_LOOP_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_FOR_LOOP, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidSwitchCase(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_SWITCH_CASE_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_SWITCH_CASE_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_SWITCH_CASE, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidTryCatch(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_TRY_CATCH_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_TRY_CATCH_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_TRY_CATCH, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidFunctionSignature(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_FUNCTION_SIGNATURE_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_FUNCTION_SIGNATURE_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_FUNCTION_SIGNATURE, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidClassDeclaration(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_CLASS_DECLARATION_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_CLASS_DECLARATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_CLASS_DECLARATION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidInterfaceDeclaration(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_INTERFACE_DECLARATION_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_INTERFACE_DECLARATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_INTERFACE_DECLARATION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidEnumDeclaration(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_ENUM_DECLARATION_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_ENUM_DECLARATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_ENUM_DECLARATION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidTypeAlias(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_TYPE_ALIAS_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_TYPE_ALIAS_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_TYPE_ALIAS, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidDestructuring(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_DESTRUCTURING_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_DESTRUCTURING_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_DESTRUCTURING, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidSpreadOperator(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Syntax::INVALID_SPREAD_OPERATOR_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Syntax::INVALID_SPREAD_OPERATOR_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Syntax::INVALID_SPREAD_OPERATOR, location, message, sug);
}

// Type error reporting methods
void EnhancedErrorReporting::reportTypeError(const String& code, const SourceLocation& location,
                                             const String& message, const String& suggestion) {
    addDiagnostic(DiagnosticLevel::Error, code, location, message, suggestion);
}

void EnhancedErrorReporting::reportTypeMismatch(const SourceLocation& location, const String& expected,
                                                const String& actual, const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::TYPE_MISMATCH_MSG, actual, expected);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::TYPE_MISMATCH_SUGGESTION, expected, actual) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::TYPE_MISMATCH, location, message, sug);
}

void EnhancedErrorReporting::reportUndefinedVariable(const SourceLocation& location, const String& name,
                                                     const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::UNDEFINED_VARIABLE_MSG, name);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::UNDEFINED_VARIABLE_SUGGESTION, name) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::UNDEFINED_VARIABLE, location, message, sug);
}

void EnhancedErrorReporting::reportUndefinedFunction(const SourceLocation& location, const String& name,
                                                     const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::UNDEFINED_FUNCTION_MSG, name);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::UNDEFINED_FUNCTION_SUGGESTION, name) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::UNDEFINED_FUNCTION, location, message, sug);
}

void EnhancedErrorReporting::reportUndefinedProperty(const SourceLocation& location, const String& property,
                                                     const String& type, const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::UNDEFINED_PROPERTY_MSG, property, type);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::UNDEFINED_PROPERTY_SUGGESTION, property, type) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::UNDEFINED_PROPERTY, location, message, sug);
}

void EnhancedErrorReporting::reportUndefinedType(const SourceLocation& location, const String& type,
                                                 const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::UNDEFINED_TYPE_MSG, type);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::UNDEFINED_TYPE_SUGGESTION, type) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::UNDEFINED_TYPE, location, message, sug);
}

void EnhancedErrorReporting::reportIncompatibleTypes(const SourceLocation& location, const String& type1,
                                                     const String& type2, const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::INCOMPATIBLE_TYPES_MSG, type1, type2);
    String sug = suggestion.empty() ? ErrorSuggestions::Type::INCOMPATIBLE_TYPES_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::INCOMPATIBLE_TYPES, location, message, sug);
}

void EnhancedErrorReporting::reportMissingTypeAnnotation(const SourceLocation& location, const String& name,
                                                         const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::MISSING_TYPE_ANNOTATION_MSG, name);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::MISSING_TYPE_ANNOTATION_SUGGESTION, name, "any") : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::MISSING_TYPE_ANNOTATION, location, message, sug);
}

void EnhancedErrorReporting::reportInvalidTypeAnnotation(const SourceLocation& location, const String& annotation,
                                                         const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::INVALID_TYPE_ANNOTATION_MSG, annotation);
    String sug = suggestion.empty() ? ErrorSuggestions::Type::INVALID_TYPE_ANNOTATION_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::INVALID_TYPE_ANNOTATION, location, message, sug);
}

void EnhancedErrorReporting::reportCircularTypeReference(const SourceLocation& location, const String& suggestion) {
    String message = ErrorMessages::Type::CIRCULAR_TYPE_REFERENCE_MSG;
    String sug = suggestion.empty() ? ErrorSuggestions::Type::CIRCULAR_TYPE_REFERENCE_SUGGESTION : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::CIRCULAR_TYPE_REFERENCE, location, message, sug);
}

void EnhancedErrorReporting::reportGenericTypeError(const SourceLocation& location, const String& generic,
                                                    const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::GENERIC_TYPE_ERROR_MSG, generic);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::GENERIC_TYPE_ERROR_SUGGESTION, generic) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::GENERIC_TYPE_ERROR, location, message, sug);
}

// Continue with more methods...
void EnhancedErrorReporting::reportUnionTypeError(const SourceLocation& location, const String& unionType,
                                                  const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::UNION_TYPE_ERROR_MSG, unionType);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::UNION_TYPE_ERROR_SUGGESTION, unionType) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::UNION_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportIntersectionTypeError(const SourceLocation& location, const String& intersection,
                                                         const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::INTERSECTION_TYPE_ERROR_MSG, intersection);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::INTERSECTION_TYPE_ERROR_SUGGESTION, intersection) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::INTERSECTION_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportArrayTypeError(const SourceLocation& location, const String& array,
                                                  const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::ARRAY_TYPE_ERROR_MSG, array);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::ARRAY_TYPE_ERROR_SUGGESTION, array) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::ARRAY_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportObjectTypeError(const SourceLocation& location, const String& object,
                                                   const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::OBJECT_TYPE_ERROR_MSG, object);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::OBJECT_TYPE_ERROR_SUGGESTION, object) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::OBJECT_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportFunctionTypeError(const SourceLocation& location, const String& function,
                                                     const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::FUNCTION_TYPE_ERROR_MSG, function);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::FUNCTION_TYPE_ERROR_SUGGESTION, function) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::FUNCTION_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportClassTypeError(const SourceLocation& location, const String& class_,
                                                  const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::CLASS_TYPE_ERROR_MSG, class_);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::CLASS_TYPE_ERROR_SUGGESTION, class_) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::CLASS_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportInterfaceTypeError(const SourceLocation& location, const String& interface,
                                                      const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::INTERFACE_TYPE_ERROR_MSG, interface);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::INTERFACE_TYPE_ERROR_SUGGESTION, interface) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::INTERFACE_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportEnumTypeError(const SourceLocation& location, const String& enum_,
                                                 const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::ENUM_TYPE_ERROR_MSG, enum_);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::ENUM_TYPE_ERROR_SUGGESTION, enum_) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::ENUM_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportTupleTypeError(const SourceLocation& location, const String& tuple,
                                                   const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::TUPLE_TYPE_ERROR_MSG, tuple);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::TUPLE_TYPE_ERROR_SUGGESTION, tuple) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::TUPLE_TYPE_ERROR, location, message, sug);
}

void EnhancedErrorReporting::reportLiteralTypeError(const SourceLocation& location, const String& literal,
                                                    const String& suggestion) {
    String message = formatMessage(ErrorMessages::Type::LITERAL_TYPE_ERROR_MSG, literal);
    String sug = suggestion.empty() ? formatSuggestion(ErrorSuggestions::Type::LITERAL_TYPE_ERROR_SUGGESTION, literal) : suggestion;
    addDiagnostic(DiagnosticLevel::Error, ErrorCodes::Type::LITERAL_TYPE_ERROR, location, message, sug);
}

// Warning reporting methods
void EnhancedErrorReporting::reportWarning(const String& code, const SourceLocation& location,
                                          const String& message, const String& suggestion) {
    addDiagnostic(DiagnosticLevel::Warning, code, location, message, suggestion);
}

// Utility methods
void EnhancedErrorReporting::setSourceContent(const String& filename, const String& content) {
    diagnostics_.setSourceContent(filename, content);
}

void EnhancedErrorReporting::setMaxErrors(size_t maxErrors) {
    diagnostics_.setMaxErrors(maxErrors);
}

void EnhancedErrorReporting::setWarningsAsErrors(bool warningsAsErrors) {
    diagnostics_.setWarningsAsErrors(warningsAsErrors);
}

void EnhancedErrorReporting::setSuppressWarnings(bool suppressWarnings) {
    diagnostics_.setSuppressWarnings(suppressWarnings);
}

void EnhancedErrorReporting::suppressWarning(const String& code) {
    diagnostics_.suppressWarning(code);
}

void EnhancedErrorReporting::promoteWarningToError(const String& code) {
    diagnostics_.promoteWarningToError(code);
}

bool EnhancedErrorReporting::hasErrors() const {
    return diagnostics_.hasErrors();
}

bool EnhancedErrorReporting::hasWarnings() const {
    return diagnostics_.hasWarnings();
}

size_t EnhancedErrorReporting::getErrorCount() const {
    return diagnostics_.getErrorCount();
}

size_t EnhancedErrorReporting::getWarningCount() const {
    return diagnostics_.getWarningCount();
}

size_t EnhancedErrorReporting::getTotalCount() const {
    return diagnostics_.getTotalCount();
}

const std::vector<Diagnostic>& EnhancedErrorReporting::getDiagnostics() const {
    return diagnostics_.getDiagnostics();
}

void EnhancedErrorReporting::clearDiagnostics() {
    diagnostics_.clearDiagnostics();
}

void EnhancedErrorReporting::printDiagnostics() const {
    diagnostics_.printDiagnostics();
}

void EnhancedErrorReporting::printSummary() const {
    diagnostics_.printSummary();
}

String EnhancedErrorReporting::formatDiagnostic(const Diagnostic& diagnostic) const {
    return diagnostics_.formatDiagnostic(diagnostic);
}

// Helper methods
String EnhancedErrorReporting::formatMessage(const String& template_, const String& arg1, const String& arg2) {
    String result = template_;
    
    // Replace {0} with arg1
    size_t pos = result.find("{0}");
    if (pos != String::npos) {
        result.replace(pos, 3, arg1);
    }
    
    // Replace {1} with arg2
    pos = result.find("{1}");
    if (pos != String::npos) {
        result.replace(pos, 3, arg2);
    }
    
    return result;
}

String EnhancedErrorReporting::formatSuggestion(const String& template_, const String& arg1, const String& arg2) {
    return formatMessage(template_, arg1, arg2);
}

void EnhancedErrorReporting::addDiagnostic(DiagnosticLevel level, const String& code, const SourceLocation& location,
                                           const String& message, const String& suggestion) {
    if (!suggestion.empty()) {
        // Add the suggestion as a note
        diagnostics_.note("Suggestion: " + suggestion, location, code + "_suggestion");
    }
    
    // Add the main diagnostic
    diagnostics_.error(message, location, code);
}

} // namespace tsc
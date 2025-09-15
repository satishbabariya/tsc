#pragma once

#include "tsc/Common.h"

namespace tsc {

// Comprehensive error code system for TSC compiler
namespace ErrorCodes {

// Syntax Errors (TS1000-TS1999)
namespace Syntax {
    constexpr const char* UNEXPECTED_TOKEN = "TS1001";
    constexpr const char* EXPECTED_TOKEN = "TS1002";
    constexpr const char* MISSING_SEMICOLON = "TS1003";
    constexpr const char* MISSING_BRACE = "TS1004";
    constexpr const char* MISSING_PARENTHESIS = "TS1005";
    constexpr const char* MISSING_BRACKET = "TS1006";
    constexpr const char* INVALID_EXPRESSION = "TS1007";
    constexpr const char* INVALID_STATEMENT = "TS1008";
    constexpr const char* INVALID_DECLARATION = "TS1009";
    constexpr const char* DUPLICATE_DECLARATION = "TS1010";
    constexpr const char* INVALID_FOR_LOOP = "TS1011";
    constexpr const char* INVALID_SWITCH_CASE = "TS1012";
    constexpr const char* INVALID_TRY_CATCH = "TS1013";
    constexpr const char* INVALID_FUNCTION_SIGNATURE = "TS1014";
    constexpr const char* INVALID_CLASS_DECLARATION = "TS1015";
    constexpr const char* INVALID_INTERFACE_DECLARATION = "TS1016";
    constexpr const char* INVALID_ENUM_DECLARATION = "TS1017";
    constexpr const char* INVALID_TYPE_ALIAS = "TS1018";
    constexpr const char* INVALID_DESTRUCTURING = "TS1019";
    constexpr const char* INVALID_SPREAD_OPERATOR = "TS1020";
}

// Type Errors (TS2000-TS2999)
namespace Type {
    constexpr const char* TYPE_MISMATCH = "TS2001";
    constexpr const char* UNDEFINED_VARIABLE = "TS2002";
    constexpr const char* UNDEFINED_FUNCTION = "TS2003";
    constexpr const char* UNDEFINED_PROPERTY = "TS2004";
    constexpr const char* UNDEFINED_TYPE = "TS2005";
    constexpr const char* INCOMPATIBLE_TYPES = "TS2006";
    constexpr const char* MISSING_TYPE_ANNOTATION = "TS2007";
    constexpr const char* INVALID_TYPE_ANNOTATION = "TS2008";
    constexpr const char* CIRCULAR_TYPE_REFERENCE = "TS2009";
    constexpr const char* GENERIC_TYPE_ERROR = "TS2010";
    constexpr const char* UNION_TYPE_ERROR = "TS2011";
    constexpr const char* INTERSECTION_TYPE_ERROR = "TS2012";
    constexpr const char* ARRAY_TYPE_ERROR = "TS2013";
    constexpr const char* OBJECT_TYPE_ERROR = "TS2014";
    constexpr const char* FUNCTION_TYPE_ERROR = "TS2015";
    constexpr const char* CLASS_TYPE_ERROR = "TS2016";
    constexpr const char* INTERFACE_TYPE_ERROR = "TS2017";
    constexpr const char* ENUM_TYPE_ERROR = "TS2018";
    constexpr const char* TUPLE_TYPE_ERROR = "TS2019";
    constexpr const char* LITERAL_TYPE_ERROR = "TS2020";
}

// Semantic Errors (TS3000-TS3999)
namespace Semantic {
    constexpr const char* DUPLICATE_IDENTIFIER = "TS3001";
    constexpr const char* CANNOT_FIND_NAME = "TS3002";
    constexpr const char* CANNOT_FIND_MODULE = "TS3003";
    constexpr const char* CANNOT_FIND_NAMESPACE = "TS3004";
    constexpr const char* CANNOT_FIND_TYPE = "TS3005";
    constexpr const char* CANNOT_FIND_SYMBOL = "TS3006";
    constexpr const char* CANNOT_FIND_FUNCTION = "TS3007";
    constexpr const char* CANNOT_FIND_CLASS = "TS3008";
    constexpr const char* CANNOT_FIND_INTERFACE = "TS3009";
    constexpr const char* CANNOT_FIND_ENUM = "TS3010";
    constexpr const char* CANNOT_FIND_VARIABLE = "TS3011";
    constexpr const char* CANNOT_FIND_PROPERTY = "TS3012";
    constexpr const char* CANNOT_FIND_METHOD = "TS3013";
    constexpr const char* CANNOT_FIND_CONSTRUCTOR = "TS3014";
    constexpr const char* CANNOT_FIND_DESTRUCTOR = "TS3015";
    constexpr const char* CANNOT_FIND_ACCESSOR = "TS3016";
    constexpr const char* CANNOT_FIND_PARAMETER = "TS3017";
    constexpr const char* CANNOT_FIND_ARGUMENT = "TS3018";
    constexpr const char* CANNOT_FIND_RETURN_TYPE = "TS3019";
    constexpr const char* CANNOT_FIND_TYPE_PARAMETER = "TS3020";
}

// Code Generation Errors (TS4000-TS4999)
namespace CodeGen {
    constexpr const char* LLVM_GENERATION_ERROR = "TS4001";
    constexpr const char* LLVM_OPTIMIZATION_ERROR = "TS4002";
    constexpr const char* LLVM_LINKING_ERROR = "TS4003";
    constexpr const char* LLVM_TARGET_ERROR = "TS4004";
    constexpr const char* LLVM_MEMORY_ERROR = "TS4005";
    constexpr const char* LLVM_TYPE_ERROR = "TS4006";
    constexpr const char* LLVM_FUNCTION_ERROR = "TS4007";
    constexpr const char* LLVM_BLOCK_ERROR = "TS4008";
    constexpr const char* LLVM_INSTRUCTION_ERROR = "TS4009";
    constexpr const char* LLVM_CONSTANT_ERROR = "TS4010";
    constexpr const char* LLVM_GLOBAL_ERROR = "TS4011";
    constexpr const char* LLVM_LOCAL_ERROR = "TS4012";
    constexpr const char* LLVM_POINTER_ERROR = "TS4013";
    constexpr const char* LLVM_ARRAY_ERROR = "TS4014";
    constexpr const char* LLVM_STRUCT_ERROR = "TS4015";
    constexpr const char* LLVM_UNION_ERROR = "TS4016";
    constexpr const char* LLVM_VECTOR_ERROR = "TS4017";
    constexpr const char* LLVM_METADATA_ERROR = "TS4018";
    constexpr const char* LLVM_DEBUG_ERROR = "TS4019";
    constexpr const char* LLVM_VERIFICATION_ERROR = "TS4020";
}

// Runtime Errors (TS5000-TS5999)
namespace Runtime {
    constexpr const char* DIVISION_BY_ZERO = "TS5001";
    constexpr const char* ARRAY_INDEX_OUT_OF_BOUNDS = "TS5002";
    constexpr const char* NULL_POINTER_DEREFERENCE = "TS5003";
    constexpr const char* MEMORY_ACCESS_VIOLATION = "TS5004";
    constexpr const char* STACK_OVERFLOW = "TS5005";
    constexpr const char* HEAP_OVERFLOW = "TS5006";
    constexpr const char* MEMORY_LEAK = "TS5007";
    constexpr const char* DOUBLE_FREE = "TS5008";
    constexpr const char* USE_AFTER_FREE = "TS5009";
    constexpr const char* BUFFER_OVERFLOW = "TS5010";
    constexpr const char* INTEGER_OVERFLOW = "TS5011";
    constexpr const char* FLOATING_POINT_EXCEPTION = "TS5012";
    constexpr const char* INVALID_CAST = "TS5013";
    constexpr const char* INVALID_CONVERSION = "TS5014";
    constexpr const char* INVALID_OPERATION = "TS5015";
    constexpr const char* INVALID_ARGUMENT = "TS5016";
    constexpr const char* INVALID_RETURN = "TS5017";
    constexpr const char* INVALID_CALL = "TS5018";
    constexpr const char* INVALID_ACCESS = "TS5019";
    constexpr const char* INVALID_ASSIGNMENT = "TS5020";
}

// Warnings (TS6000-TS6999)
namespace Warning {
    constexpr const char* UNUSED_VARIABLE = "TS6001";
    constexpr const char* UNUSED_FUNCTION = "TS6002";
    constexpr const char* UNUSED_PARAMETER = "TS6003";
    constexpr const char* UNUSED_TYPE = "TS6004";
    constexpr const char* UNUSED_IMPORT = "TS6005";
    constexpr const char* UNUSED_EXPORT = "TS6006";
    constexpr const char* UNREACHABLE_CODE = "TS6007";
    constexpr const char* DEAD_CODE = "TS6008";
    constexpr const char* IMPLICIT_ANY = "TS6009";
    constexpr const char* IMPLICIT_RETURN = "TS6010";
    constexpr const char* IMPLICIT_CONVERSION = "TS6011";
    constexpr const char* IMPLICIT_CAST = "TS6012";
    constexpr const char* IMPLICIT_ASSIGNMENT = "TS6013";
    constexpr const char* IMPLICIT_DECLARATION = "TS6014";
    constexpr const char* IMPLICIT_DEFINITION = "TS6015";
    constexpr const char* IMPLICIT_INSTANTIATION = "TS6016";
    constexpr const char* IMPLICIT_SPECIALIZATION = "TS6017";
    constexpr const char* IMPLICIT_TEMPLATE = "TS6018";
    constexpr const char* IMPLICIT_GENERIC = "TS6019";
    constexpr const char* IMPLICIT_TYPE = "TS6020";
}

// Info Messages (TS7000-TS7999)
namespace Info {
    constexpr const char* COMPILATION_STARTED = "TS7001";
    constexpr const char* COMPILATION_COMPLETED = "TS7002";
    constexpr const char* OPTIMIZATION_APPLIED = "TS7003";
    constexpr const char* DEBUG_INFO_GENERATED = "TS7004";
    constexpr const char* SOURCE_MAP_GENERATED = "TS7005";
    constexpr const char* METADATA_GENERATED = "TS7006";
    constexpr const char* SYMBOL_TABLE_BUILT = "TS7007";
    constexpr const char* DEPENDENCY_GRAPH_BUILT = "TS7008";
    constexpr const char* CALL_GRAPH_BUILT = "TS7009";
    constexpr const char* CONTROL_FLOW_GRAPH_BUILT = "TS7010";
    constexpr const char* DATA_FLOW_GRAPH_BUILT = "TS7011";
    constexpr const char* INTERFERENCE_GRAPH_BUILT = "TS7012";
    constexpr const char* REGISTER_ALLOCATION_DONE = "TS7013";
    constexpr const char* INSTRUCTION_SCHEDULING_DONE = "TS7014";
    constexpr const char* PEEPHOLE_OPTIMIZATION_DONE = "TS7015";
    constexpr const char* LOOP_OPTIMIZATION_DONE = "TS7016";
    constexpr const char* FUNCTION_INLINING_DONE = "TS7017";
    constexpr const char* CONSTANT_PROPAGATION_DONE = "TS7018";
    constexpr const char* DEAD_CODE_ELIMINATION_DONE = "TS7019";
    constexpr const char* COMMON_SUBEXPRESSION_ELIMINATION_DONE = "TS7020";
}

} // namespace ErrorCodes

// Error message templates with suggestions
namespace ErrorMessages {

// Syntax error messages
namespace Syntax {
    constexpr const char* UNEXPECTED_TOKEN_MSG = "Unexpected token '{0}'. Expected '{1}'.";
    constexpr const char* EXPECTED_TOKEN_MSG = "Expected '{0}' but found '{1}'.";
    constexpr const char* MISSING_SEMICOLON_MSG = "Missing semicolon after statement.";
    constexpr const char* MISSING_BRACE_MSG = "Missing '{0}' brace.";
    constexpr const char* MISSING_PARENTHESIS_MSG = "Missing '{0}' parenthesis.";
    constexpr const char* MISSING_BRACKET_MSG = "Missing '{0}' bracket.";
    constexpr const char* INVALID_EXPRESSION_MSG = "Invalid expression syntax.";
    constexpr const char* INVALID_STATEMENT_MSG = "Invalid statement syntax.";
    constexpr const char* INVALID_DECLARATION_MSG = "Invalid declaration syntax.";
    constexpr const char* DUPLICATE_DECLARATION_MSG = "Duplicate declaration of '{0}'.";
    constexpr const char* INVALID_FOR_LOOP_MSG = "Invalid for loop syntax.";
    constexpr const char* INVALID_SWITCH_CASE_MSG = "Invalid switch case syntax.";
    constexpr const char* INVALID_TRY_CATCH_MSG = "Invalid try-catch syntax.";
    constexpr const char* INVALID_FUNCTION_SIGNATURE_MSG = "Invalid function signature.";
    constexpr const char* INVALID_CLASS_DECLARATION_MSG = "Invalid class declaration.";
    constexpr const char* INVALID_INTERFACE_DECLARATION_MSG = "Invalid interface declaration.";
    constexpr const char* INVALID_ENUM_DECLARATION_MSG = "Invalid enum declaration.";
    constexpr const char* INVALID_TYPE_ALIAS_MSG = "Invalid type alias declaration.";
    constexpr const char* INVALID_DESTRUCTURING_MSG = "Invalid destructuring pattern.";
    constexpr const char* INVALID_SPREAD_OPERATOR_MSG = "Invalid spread operator usage.";
}

// Type error messages
namespace Type {
    constexpr const char* TYPE_MISMATCH_MSG = "Type '{0}' is not assignable to type '{1}'.";
    constexpr const char* UNDEFINED_VARIABLE_MSG = "Cannot find name '{0}'.";
    constexpr const char* UNDEFINED_FUNCTION_MSG = "Cannot find function '{0}'.";
    constexpr const char* UNDEFINED_PROPERTY_MSG = "Property '{0}' does not exist on type '{1}'.";
    constexpr const char* UNDEFINED_TYPE_MSG = "Cannot find type '{0}'.";
    constexpr const char* INCOMPATIBLE_TYPES_MSG = "Types '{0}' and '{1}' are incompatible.";
    constexpr const char* MISSING_TYPE_ANNOTATION_MSG = "Missing type annotation for '{0}'.";
    constexpr const char* INVALID_TYPE_ANNOTATION_MSG = "Invalid type annotation '{0}'.";
    constexpr const char* CIRCULAR_TYPE_REFERENCE_MSG = "Circular type reference detected.";
    constexpr const char* GENERIC_TYPE_ERROR_MSG = "Generic type error in '{0}'.";
    constexpr const char* UNION_TYPE_ERROR_MSG = "Union type error in '{0}'.";
    constexpr const char* INTERSECTION_TYPE_ERROR_MSG = "Intersection type error in '{0}'.";
    constexpr const char* ARRAY_TYPE_ERROR_MSG = "Array type error in '{0}'.";
    constexpr const char* OBJECT_TYPE_ERROR_MSG = "Object type error in '{0}'.";
    constexpr const char* FUNCTION_TYPE_ERROR_MSG = "Function type error in '{0}'.";
    constexpr const char* CLASS_TYPE_ERROR_MSG = "Class type error in '{0}'.";
    constexpr const char* INTERFACE_TYPE_ERROR_MSG = "Interface type error in '{0}'.";
    constexpr const char* ENUM_TYPE_ERROR_MSG = "Enum type error in '{0}'.";
    constexpr const char* TUPLE_TYPE_ERROR_MSG = "Tuple type error in '{0}'.";
    constexpr const char* LITERAL_TYPE_ERROR_MSG = "Literal type error in '{0}'.";
}

// Semantic error messages
namespace Semantic {
    constexpr const char* DUPLICATE_IDENTIFIER_MSG = "Duplicate identifier '{0}'.";
    constexpr const char* CANNOT_FIND_NAME_MSG = "Cannot find name '{0}'.";
    constexpr const char* CANNOT_FIND_MODULE_MSG = "Cannot find module '{0}'.";
    constexpr const char* CANNOT_FIND_NAMESPACE_MSG = "Cannot find namespace '{0}'.";
    constexpr const char* CANNOT_FIND_TYPE_MSG = "Cannot find type '{0}'.";
    constexpr const char* CANNOT_FIND_SYMBOL_MSG = "Cannot find symbol '{0}'.";
    constexpr const char* CANNOT_FIND_FUNCTION_MSG = "Cannot find function '{0}'.";
    constexpr const char* CANNOT_FIND_CLASS_MSG = "Cannot find class '{0}'.";
    constexpr const char* CANNOT_FIND_INTERFACE_MSG = "Cannot find interface '{0}'.";
    constexpr const char* CANNOT_FIND_ENUM_MSG = "Cannot find enum '{0}'.";
    constexpr const char* CANNOT_FIND_VARIABLE_MSG = "Cannot find variable '{0}'.";
    constexpr const char* CANNOT_FIND_PROPERTY_MSG = "Cannot find property '{0}'.";
    constexpr const char* CANNOT_FIND_METHOD_MSG = "Cannot find method '{0}'.";
    constexpr const char* CANNOT_FIND_CONSTRUCTOR_MSG = "Cannot find constructor '{0}'.";
    constexpr const char* CANNOT_FIND_DESTRUCTOR_MSG = "Cannot find destructor '{0}'.";
    constexpr const char* CANNOT_FIND_ACCESSOR_MSG = "Cannot find accessor '{0}'.";
    constexpr const char* CANNOT_FIND_PARAMETER_MSG = "Cannot find parameter '{0}'.";
    constexpr const char* CANNOT_FIND_ARGUMENT_MSG = "Cannot find argument '{0}'.";
    constexpr const char* CANNOT_FIND_RETURN_TYPE_MSG = "Cannot find return type '{0}'.";
    constexpr const char* CANNOT_FIND_TYPE_PARAMETER_MSG = "Cannot find type parameter '{0}'.";
}

// Code generation error messages
namespace CodeGen {
    constexpr const char* LLVM_GENERATION_ERROR_MSG = "LLVM IR generation failed: {0}.";
    constexpr const char* LLVM_OPTIMIZATION_ERROR_MSG = "LLVM optimization failed: {0}.";
    constexpr const char* LLVM_LINKING_ERROR_MSG = "LLVM linking failed: {0}.";
    constexpr const char* LLVM_TARGET_ERROR_MSG = "LLVM target error: {0}.";
    constexpr const char* LLVM_MEMORY_ERROR_MSG = "LLVM memory error: {0}.";
    constexpr const char* LLVM_TYPE_ERROR_MSG = "LLVM type error: {0}.";
    constexpr const char* LLVM_FUNCTION_ERROR_MSG = "LLVM function error: {0}.";
    constexpr const char* LLVM_BLOCK_ERROR_MSG = "LLVM block error: {0}.";
    constexpr const char* LLVM_INSTRUCTION_ERROR_MSG = "LLVM instruction error: {0}.";
    constexpr const char* LLVM_CONSTANT_ERROR_MSG = "LLVM constant error: {0}.";
    constexpr const char* LLVM_GLOBAL_ERROR_MSG = "LLVM global error: {0}.";
    constexpr const char* LLVM_LOCAL_ERROR_MSG = "LLVM local error: {0}.";
    constexpr const char* LLVM_POINTER_ERROR_MSG = "LLVM pointer error: {0}.";
    constexpr const char* LLVM_ARRAY_ERROR_MSG = "LLVM array error: {0}.";
    constexpr const char* LLVM_STRUCT_ERROR_MSG = "LLVM struct error: {0}.";
    constexpr const char* LLVM_UNION_ERROR_MSG = "LLVM union error: {0}.";
    constexpr const char* LLVM_VECTOR_ERROR_MSG = "LLVM vector error: {0}.";
    constexpr const char* LLVM_METADATA_ERROR_MSG = "LLVM metadata error: {0}.";
    constexpr const char* LLVM_DEBUG_ERROR_MSG = "LLVM debug error: {0}.";
    constexpr const char* LLVM_VERIFICATION_ERROR_MSG = "LLVM verification error: {0}.";
}

// Runtime error messages
namespace Runtime {
    constexpr const char* DIVISION_BY_ZERO_MSG = "Division by zero detected.";
    constexpr const char* ARRAY_INDEX_OUT_OF_BOUNDS_MSG = "Array index {0} is out of bounds.";
    constexpr const char* NULL_POINTER_DEREFERENCE_MSG = "Null pointer dereference detected.";
    constexpr const char* MEMORY_ACCESS_VIOLATION_MSG = "Memory access violation detected.";
    constexpr const char* STACK_OVERFLOW_MSG = "Stack overflow detected.";
    constexpr const char* HEAP_OVERFLOW_MSG = "Heap overflow detected.";
    constexpr const char* MEMORY_LEAK_MSG = "Memory leak detected.";
    constexpr const char* DOUBLE_FREE_MSG = "Double free detected.";
    constexpr const char* USE_AFTER_FREE_MSG = "Use after free detected.";
    constexpr const char* BUFFER_OVERFLOW_MSG = "Buffer overflow detected.";
    constexpr const char* INTEGER_OVERFLOW_MSG = "Integer overflow detected.";
    constexpr const char* FLOATING_POINT_EXCEPTION_MSG = "Floating point exception detected.";
    constexpr const char* INVALID_CAST_MSG = "Invalid cast from '{0}' to '{1}'.";
    constexpr const char* INVALID_CONVERSION_MSG = "Invalid conversion from '{0}' to '{1}'.";
    constexpr const char* INVALID_OPERATION_MSG = "Invalid operation '{0}'.";
    constexpr const char* INVALID_ARGUMENT_MSG = "Invalid argument '{0}'.";
    constexpr const char* INVALID_RETURN_MSG = "Invalid return value '{0}'.";
    constexpr const char* INVALID_CALL_MSG = "Invalid function call '{0}'.";
    constexpr const char* INVALID_ACCESS_MSG = "Invalid access to '{0}'.";
    constexpr const char* INVALID_ASSIGNMENT_MSG = "Invalid assignment to '{0}'.";
}

// Warning messages
namespace Warning {
    constexpr const char* UNUSED_VARIABLE_MSG = "Variable '{0}' is declared but never used.";
    constexpr const char* UNUSED_FUNCTION_MSG = "Function '{0}' is declared but never used.";
    constexpr const char* UNUSED_PARAMETER_MSG = "Parameter '{0}' is declared but never used.";
    constexpr const char* UNUSED_TYPE_MSG = "Type '{0}' is declared but never used.";
    constexpr const char* UNUSED_IMPORT_MSG = "Import '{0}' is declared but never used.";
    constexpr const char* UNUSED_EXPORT_MSG = "Export '{0}' is declared but never used.";
    constexpr const char* UNREACHABLE_CODE_MSG = "Unreachable code detected.";
    constexpr const char* DEAD_CODE_MSG = "Dead code detected.";
    constexpr const char* IMPLICIT_ANY_MSG = "Implicit 'any' type detected.";
    constexpr const char* IMPLICIT_RETURN_MSG = "Implicit return type detected.";
    constexpr const char* IMPLICIT_CONVERSION_MSG = "Implicit conversion from '{0}' to '{1}'.";
    constexpr const char* IMPLICIT_CAST_MSG = "Implicit cast from '{0}' to '{1}'.";
    constexpr const char* IMPLICIT_ASSIGNMENT_MSG = "Implicit assignment to '{0}'.";
    constexpr const char* IMPLICIT_DECLARATION_MSG = "Implicit declaration of '{0}'.";
    constexpr const char* IMPLICIT_DEFINITION_MSG = "Implicit definition of '{0}'.";
    constexpr const char* IMPLICIT_INSTANTIATION_MSG = "Implicit instantiation of '{0}'.";
    constexpr const char* IMPLICIT_SPECIALIZATION_MSG = "Implicit specialization of '{0}'.";
    constexpr const char* IMPLICIT_TEMPLATE_MSG = "Implicit template '{0}'.";
    constexpr const char* IMPLICIT_GENERIC_MSG = "Implicit generic '{0}'.";
    constexpr const char* IMPLICIT_TYPE_MSG = "Implicit type '{0}'.";
}

// Info messages
namespace Info {
    constexpr const char* COMPILATION_STARTED_MSG = "Compilation started for '{0}'.";
    constexpr const char* COMPILATION_COMPLETED_MSG = "Compilation completed successfully.";
    constexpr const char* OPTIMIZATION_APPLIED_MSG = "Optimization '{0}' applied.";
    constexpr const char* DEBUG_INFO_GENERATED_MSG = "Debug information generated.";
    constexpr const char* SOURCE_MAP_GENERATED_MSG = "Source map generated.";
    constexpr const char* METADATA_GENERATED_MSG = "Metadata generated.";
    constexpr const char* SYMBOL_TABLE_BUILT_MSG = "Symbol table built.";
    constexpr const char* DEPENDENCY_GRAPH_BUILT_MSG = "Dependency graph built.";
    constexpr const char* CALL_GRAPH_BUILT_MSG = "Call graph built.";
    constexpr const char* CONTROL_FLOW_GRAPH_BUILT_MSG = "Control flow graph built.";
    constexpr const char* DATA_FLOW_GRAPH_BUILT_MSG = "Data flow graph built.";
    constexpr const char* INTERFERENCE_GRAPH_BUILT_MSG = "Interference graph built.";
    constexpr const char* REGISTER_ALLOCATION_DONE_MSG = "Register allocation completed.";
    constexpr const char* INSTRUCTION_SCHEDULING_DONE_MSG = "Instruction scheduling completed.";
    constexpr const char* PEEPHOLE_OPTIMIZATION_DONE_MSG = "Peephole optimization completed.";
    constexpr const char* LOOP_OPTIMIZATION_DONE_MSG = "Loop optimization completed.";
    constexpr const char* FUNCTION_INLINING_DONE_MSG = "Function inlining completed.";
    constexpr const char* CONSTANT_PROPAGATION_DONE_MSG = "Constant propagation completed.";
    constexpr const char* DEAD_CODE_ELIMINATION_DONE_MSG = "Dead code elimination completed.";
    constexpr const char* COMMON_SUBEXPRESSION_ELIMINATION_DONE_MSG = "Common subexpression elimination completed.";
}

} // namespace ErrorMessages

// Error suggestion templates
namespace ErrorSuggestions {

// Syntax suggestions
namespace Syntax {
    constexpr const char* UNEXPECTED_TOKEN_SUGGESTION = "Did you mean '{0}'?";
    constexpr const char* EXPECTED_TOKEN_SUGGESTION = "Add '{0}' here.";
    constexpr const char* MISSING_SEMICOLON_SUGGESTION = "Add a semicolon ';' at the end of the statement.";
    constexpr const char* MISSING_BRACE_SUGGESTION = "Add '{0}' brace here.";
    constexpr const char* MISSING_PARENTHESIS_SUGGESTION = "Add '{0}' parenthesis here.";
    constexpr const char* MISSING_BRACKET_SUGGESTION = "Add '{0}' bracket here.";
    constexpr const char* INVALID_EXPRESSION_SUGGESTION = "Check the expression syntax.";
    constexpr const char* INVALID_STATEMENT_SUGGESTION = "Check the statement syntax.";
    constexpr const char* INVALID_DECLARATION_SUGGESTION = "Check the declaration syntax.";
    constexpr const char* DUPLICATE_DECLARATION_SUGGESTION = "Remove the duplicate declaration or rename one of them.";
    constexpr const char* INVALID_FOR_LOOP_SUGGESTION = "Check the for loop syntax: for (init; condition; increment) { body }.";
    constexpr const char* INVALID_SWITCH_CASE_SUGGESTION = "Check the switch case syntax: case value: { statements }.";
    constexpr const char* INVALID_TRY_CATCH_SUGGESTION = "Check the try-catch syntax: try { } catch (error) { }.";
    constexpr const char* INVALID_FUNCTION_SIGNATURE_SUGGESTION = "Check the function signature syntax.";
    constexpr const char* INVALID_CLASS_DECLARATION_SUGGESTION = "Check the class declaration syntax.";
    constexpr const char* INVALID_INTERFACE_DECLARATION_SUGGESTION = "Check the interface declaration syntax.";
    constexpr const char* INVALID_ENUM_DECLARATION_SUGGESTION = "Check the enum declaration syntax.";
    constexpr const char* INVALID_TYPE_ALIAS_SUGGESTION = "Check the type alias declaration syntax.";
    constexpr const char* INVALID_DESTRUCTURING_SUGGESTION = "Check the destructuring pattern syntax.";
    constexpr const char* INVALID_SPREAD_OPERATOR_SUGGESTION = "Check the spread operator usage.";
}

// Type suggestions
namespace Type {
    constexpr const char* TYPE_MISMATCH_SUGGESTION = "Consider using type '{0}' instead of '{1}'.";
    constexpr const char* UNDEFINED_VARIABLE_SUGGESTION = "Declare the variable '{0}' or check the spelling.";
    constexpr const char* UNDEFINED_FUNCTION_SUGGESTION = "Declare the function '{0}' or check the spelling.";
    constexpr const char* UNDEFINED_PROPERTY_SUGGESTION = "Add property '{0}' to type '{1}' or check the spelling.";
    constexpr const char* UNDEFINED_TYPE_SUGGESTION = "Import or declare the type '{0}'.";
    constexpr const char* INCOMPATIBLE_TYPES_SUGGESTION = "Use compatible types or add type conversion.";
    constexpr const char* MISSING_TYPE_ANNOTATION_SUGGESTION = "Add type annotation: '{0}: {1}'.";
    constexpr const char* INVALID_TYPE_ANNOTATION_SUGGESTION = "Use a valid type annotation.";
    constexpr const char* CIRCULAR_TYPE_REFERENCE_SUGGESTION = "Break the circular reference by using forward declarations.";
    constexpr const char* GENERIC_TYPE_ERROR_SUGGESTION = "Check the generic type parameters.";
    constexpr const char* UNION_TYPE_ERROR_SUGGESTION = "Check the union type definition.";
    constexpr const char* INTERSECTION_TYPE_ERROR_SUGGESTION = "Check the intersection type definition.";
    constexpr const char* ARRAY_TYPE_ERROR_SUGGESTION = "Check the array type definition.";
    constexpr const char* OBJECT_TYPE_ERROR_SUGGESTION = "Check the object type definition.";
    constexpr const char* FUNCTION_TYPE_ERROR_SUGGESTION = "Check the function type definition.";
    constexpr const char* CLASS_TYPE_ERROR_SUGGESTION = "Check the class type definition.";
    constexpr const char* INTERFACE_TYPE_ERROR_SUGGESTION = "Check the interface type definition.";
    constexpr const char* ENUM_TYPE_ERROR_SUGGESTION = "Check the enum type definition.";
    constexpr const char* TUPLE_TYPE_ERROR_SUGGESTION = "Check the tuple type definition.";
    constexpr const char* LITERAL_TYPE_ERROR_SUGGESTION = "Check the literal type definition.";
}

// Semantic suggestions
namespace Semantic {
    constexpr const char* DUPLICATE_IDENTIFIER_SUGGESTION = "Rename one of the identifiers to make them unique.";
    constexpr const char* CANNOT_FIND_NAME_SUGGESTION = "Check the spelling or import the symbol.";
    constexpr const char* CANNOT_FIND_MODULE_SUGGESTION = "Check the module path or install the module.";
    constexpr const char* CANNOT_FIND_NAMESPACE_SUGGESTION = "Check the namespace declaration or import.";
    constexpr const char* CANNOT_FIND_TYPE_SUGGESTION = "Import or declare the type.";
    constexpr const char* CANNOT_FIND_SYMBOL_SUGGESTION = "Check the symbol declaration or import.";
    constexpr const char* CANNOT_FIND_FUNCTION_SUGGESTION = "Declare the function or check the spelling.";
    constexpr const char* CANNOT_FIND_CLASS_SUGGESTION = "Declare the class or check the spelling.";
    constexpr const char* CANNOT_FIND_INTERFACE_SUGGESTION = "Declare the interface or check the spelling.";
    constexpr const char* CANNOT_FIND_ENUM_SUGGESTION = "Declare the enum or check the spelling.";
    constexpr const char* CANNOT_FIND_VARIABLE_SUGGESTION = "Declare the variable or check the spelling.";
    constexpr const char* CANNOT_FIND_PROPERTY_SUGGESTION = "Add the property to the type or check the spelling.";
    constexpr const char* CANNOT_FIND_METHOD_SUGGESTION = "Add the method to the type or check the spelling.";
    constexpr const char* CANNOT_FIND_CONSTRUCTOR_SUGGESTION = "Declare the constructor or check the spelling.";
    constexpr const char* CANNOT_FIND_DESTRUCTOR_SUGGESTION = "Declare the destructor or check the spelling.";
    constexpr const char* CANNOT_FIND_ACCESSOR_SUGGESTION = "Declare the accessor or check the spelling.";
    constexpr const char* CANNOT_FIND_PARAMETER_SUGGESTION = "Check the parameter declaration or spelling.";
    constexpr const char* CANNOT_FIND_ARGUMENT_SUGGESTION = "Check the argument or function call.";
    constexpr const char* CANNOT_FIND_RETURN_TYPE_SUGGESTION = "Check the return type declaration.";
    constexpr const char* CANNOT_FIND_TYPE_PARAMETER_SUGGESTION = "Check the type parameter declaration.";
}

} // namespace ErrorSuggestions

} // namespace tsc
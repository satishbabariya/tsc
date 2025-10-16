#!/bin/bash

echo "=== FOCUSED UNIT TEST VALIDATION ==="
echo "Date: $(date)"
echo "Testing unit tests that should work with our method call and console.log fixes"
echo ""

# Create results file
RESULTS_FILE="focused_validation_$(date +%Y%m%d_%H%M%S).txt"
echo "=== FOCUSED VALIDATION RESULTS ===" > $RESULTS_FILE
echo "Date: $(date)" >> $RESULTS_FILE
echo "" >> $RESULTS_FILE

# Function to test a specific file
test_file() {
    local file=$1
    local test_name=$(basename "$file")
    echo -n "Testing $test_name: "
    
    # Run the test and capture output
    output=$(./build/tsc "$file" 2>&1)
    error_code=$?
    
    if echo "$output" | grep -q "Compilation failed"; then
        echo "FAILED"
        echo "$test_name: FAILED" >> $RESULTS_FILE
        
        # Capture specific errors
        errors=$(echo "$output" | grep -c "error:")
        if [ $errors -gt 0 ]; then
            echo "  Errors: $errors" >> $RESULTS_FILE
            echo "$output" | grep "error:" | head -3 >> $RESULTS_FILE
        fi
    else
        echo "PASSED"
        echo "$test_name: PASSED" >> $RESULTS_FILE
    fi
}

# Test specific unit tests that should work with our fixes
echo "=== TESTING UNIT TESTS WITH METHOD CALLS ==="

# Test basic method call tests
test_file "tests/unit/basic_memory_test.ts"
test_file "tests/unit/array_element_access_test.ts"
test_file "tests/unit/array_push_simple.ts"
test_file "tests/unit/basic_cycle_test.ts"
test_file "tests/unit/constructor_param_test.ts"
test_file "tests/unit/debug_array_t_test.ts"
test_file "tests/unit/debug_destructor_signature_test.ts"
test_file "tests/unit/debug_destructor_test.ts"
test_file "tests/unit/destructor_calling_test.ts"
test_file "tests/unit/destructor_syntax_test.ts"
test_file "tests/unit/empty_string_test.ts"
test_file "tests/unit/function_object_test.ts"
test_file "tests/unit/generic_array_edge_cases_test.ts"
test_file "tests/unit/generic_multiple_parameters_test.ts"
test_file "tests/unit/generic_nested_types_test.ts"
test_file "tests/unit/generic_type_constraints_test.ts"
test_file "tests/unit/gradual_complexity_test.ts"
test_file "tests/unit/hello_world_test.ts"
test_file "tests/unit/interface_basic.ts"
test_file "tests/unit/interface_inheritance.ts"
test_file "tests/unit/interface_methods.ts"
test_file "tests/unit/interface_optional.ts"
test_file "tests/unit/large_number_test.ts"
test_file "tests/unit/long_expression_test.ts"
test_file "tests/unit/minimal_array_t_test.ts"
test_file "tests/unit/minimal_destructor_test.ts"
test_file "tests/unit/minimal_gradual_test.ts"
test_file "tests/unit/minimal_raii_test.ts"
test_file "tests/unit/minimal_test.ts"
test_file "tests/unit/multiple_resources_test.ts"
test_file "tests/unit/no_array_method_test.ts"
test_file "tests/unit/number_constructor_test.ts"
test_file "tests/unit/object_literal_test.ts"
test_file "tests/unit/same_destructor_test.ts"
test_file "tests/unit/simple_array_test.ts"
test_file "tests/unit/simple_comprehensive_test.ts"
test_file "tests/unit/simple_console_test.ts"
test_file "tests/unit/simple_destructor_only_test.ts"
test_file "tests/unit/simple_destructors_no_properties_test.ts"
test_file "tests/unit/simple_destructor_test.ts"
test_file "tests/unit/simple_generic_constraint_test.ts"
test_file "tests/unit/simple_gradual_test.ts"
test_file "tests/unit/simple_memory_test.ts"
test_file "tests/unit/simple_object_test.ts"
test_file "tests/unit/simple_raii_test.ts"
test_file "tests/unit/simple_test.ts"
test_file "tests/unit/type_interactions_functions_simple.ts"
test_file "tests/unit/type_interactions_functions.ts"
test_file "tests/unit/union_types_literals.ts"
test_file "tests/unit/unusual_text_test.ts"

echo ""
echo "=== TESTING EXAMPLES WITH METHOD CALLS ==="

# Test example files that should work
test_file "tests/examples/hello.ts"
test_file "tests/examples/simple_test.ts"
test_file "tests/examples/function_simple.ts"
test_file "tests/examples/if_simple.ts"
test_file "tests/examples/if_test.ts"
test_file "tests/examples/loops_bool.ts"
test_file "tests/examples/loops_minimal.ts"
test_file "tests/examples/loops_simple.ts"
test_file "tests/examples/loops_test.ts"
test_file "tests/examples/return_test.ts"
test_file "tests/examples/semantic_test.ts"
test_file "tests/examples/simple_call_fixed.ts"
test_file "tests/examples/simple_call.ts"
test_file "tests/examples/simple_type_test.ts"
test_file "tests/examples/simple_var.ts"
test_file "tests/examples/switch_ast_only.ts"
test_file "tests/examples/switch_simple.ts"
test_file "tests/examples/switch_test.ts"
test_file "tests/examples/switch_working.ts"
test_file "tests/examples/throw_simple.ts"
test_file "tests/examples/throw_test.ts"

echo ""
echo "=== TESTING VALIDATION TESTS ==="

# Test validation files
test_file "tests/validation/01_literals.ts"
test_file "tests/validation/02_variables.ts"
test_file "tests/validation/04_functions_decl.ts"
test_file "tests/validation/04_functions.ts"
test_file "tests/validation/05_control_flow.ts"
test_file "tests/validation/08_basic_test.ts"
test_file "tests/validation/08_edge_cases_simple.ts"
test_file "tests/validation/08_edge_final.ts"

echo ""
echo "=== VALIDATION COMPLETE ==="
echo "Results saved to: $RESULTS_FILE"
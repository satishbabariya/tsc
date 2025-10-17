#!/bin/bash

# Fix the first console_log function creation to be variadic
sed -i '1594s/{getAnyType(), getAnyType()}/{getAnyType()}/' src/codegen/LLVMCodeGen.cpp
sed -i '1595s/false/true/' src/codegen/LLVMCodeGen.cpp

# Fix the second console_log function creation to be variadic  
sed -i '3324s/{getAnyType(), getAnyType()}/{getAnyType()}/' src/codegen/LLVMCodeGen.cpp
sed -i '3325s/false/true/' src/codegen/LLVMCodeGen.cpp

echo "Fixed console_log function signatures to be variadic"

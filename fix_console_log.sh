#!/bin/bash

# Fix the first console_log function creation (around line 1590)
sed -i '1594s/{getAnyType()}/{getAnyType(), getAnyType()}/' src/codegen/LLVMCodeGen.cpp

# Fix the second console_log function creation (around line 3324)  
sed -i '3324s/{getAnyType()}/{getAnyType(), getAnyType()}/' src/codegen/LLVMCodeGen.cpp

echo "Fixed console_log function signatures to accept 2 parameters"

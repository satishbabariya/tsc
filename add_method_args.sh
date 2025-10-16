#!/bin/bash

# Find the line number where we need to add the code
LINE_NUM=$(grep -n "Found existing method function" src/codegen/LLVMCodeGen.cpp | cut -d: -f1)

# Create a temporary file with the new code
cat > temp_method_args.cpp << 'METHOD_EOF'
                            // Process arguments for method call
                            std::cout << "DEBUG: CallExpression - Processing method call with " << node.getArguments().size() << " arguments" << std::endl;
                            
                            // For method calls, we need to pass the object as the first argument
                            // Generate the object being called on
                            propertyAccess->getObject()->accept(static_cast<ASTVisitor &>(*this));
                            llvm::Value *objectInstance = getCurrentValue();
                            
                            if (!objectInstance) {
                                reportError("Failed to generate object instance for method call", node.getLocation());
                                setCurrentValue(createNullValue(getAnyType()));
                                return;
                            }
                            
                            // Prepare arguments for the method call
                            std::vector<llvm::Value *> methodArgs;
                            
                            // Add 'this' pointer as first argument (object instance)
                            methodArgs.push_back(objectInstance);
                            
                            // Add method arguments
                            std::cout << "DEBUG: CallExpression - Starting argument processing loop, node.getArguments().size() = " << node.getArguments().size() << std::endl;
                            for (size_t i = 0; i < node.getArguments().size(); ++i) {
                                const auto &arg = node.getArguments()[i];
                                std::cout << "DEBUG: CallExpression - Processing argument " << i << " of type: " << typeid(*arg.get()).name() << std::endl;
                                arg->accept(static_cast<ASTVisitor &>(*this));
                                llvm::Value *argValue = getCurrentValue();
                                if (!argValue) {
                                    reportError("Failed to generate argument for method call", node.getLocation());
                                    setCurrentValue(createNullValue(getAnyType()));
                                    return;
                                }
                                std::cout << "DEBUG: CallExpression - Generated argument " << i << " value: " << (argValue ? "valid" : "null") << std::endl;
                                methodArgs.push_back(argValue);
                            }
                            
                            std::cout << "DEBUG: CallExpression - After processing all arguments, methodArgs.size() = " << methodArgs.size() << std::endl;
                            
                            // Generate the function call
                            llvm::Value *methodCallResult;
                            if (function->getReturnType()->isVoidTy()) {
                                methodCallResult = builder_->CreateCall(function, methodArgs);
                                std::cout << "DEBUG: Created void method call to " << function->getName().str() << std::endl;
                            } else {
                                methodCallResult = builder_->CreateCall(function, methodArgs, "call_result");
                                std::cout << "DEBUG: Created non-void method call to " << function->getName().str() << std::endl;
                            }
                            
                            setCurrentValue(methodCallResult);
                            return;
METHOD_EOF

# Insert the code after the specified line
head -n $LINE_NUM src/codegen/LLVMCodeGen.cpp > temp1.cpp
cat temp_method_args.cpp >> temp1.cpp
tail -n +$((LINE_NUM + 1)) src/codegen/LLVMCodeGen.cpp >> temp1.cpp

# Replace the original file
mv temp1.cpp src/codegen/LLVMCodeGen.cpp

# Clean up
rm temp_method_args.cpp

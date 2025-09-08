#include "tsc/codegen/LLVMCodeGen.h"
#include "tsc/Compiler.h"

// LLVM includes for implementation
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"

#include <iostream>

namespace tsc {

// CodeGenContext implementation
CodeGenContext::CodeGenContext(llvm::LLVMContext& llvmContext, llvm::Module& module, 
                               llvm::IRBuilder<>& builder, DiagnosticEngine& diagnostics)
    : llvmContext_(llvmContext), module_(module), builder_(builder), diagnostics_(diagnostics) {
    // Initialize with global scope
    symbolStack_.push_back(std::unordered_map<String, llvm::Value*>());
}

void CodeGenContext::setSymbolValue(const String& name, llvm::Value* value) {
    if (!symbolStack_.empty()) {
        symbolStack_.back()[name] = value;
    }
}

llvm::Value* CodeGenContext::getSymbolValue(const String& name) const {
    // Search from innermost to outermost scope
    for (auto it = symbolStack_.rbegin(); it != symbolStack_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    return nullptr;
}

bool CodeGenContext::hasSymbol(const String& name) const {
    return getSymbolValue(name) != nullptr;
}

void CodeGenContext::enterFunction(llvm::Function* function) {
    functionStack_.push(function);
}

void CodeGenContext::exitFunction() {
    if (!functionStack_.empty()) {
        functionStack_.pop();
    }
}

llvm::Function* CodeGenContext::getCurrentFunction() const {
    return functionStack_.empty() ? nullptr : functionStack_.top();
}

void CodeGenContext::enterScope() {
    symbolStack_.push_back(std::unordered_map<String, llvm::Value*>());
}

void CodeGenContext::exitScope() {
    if (symbolStack_.size() > 1) {
        symbolStack_.pop_back();
    }
}

void CodeGenContext::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
    errorCount_++;
}

// LLVMCodeGen implementation
LLVMCodeGen::LLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options)
    : diagnostics_(diagnostics), options_(options), currentValue_(nullptr) {
    
    // Initialize LLVM
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    
    // Create LLVM context and module
    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("tsc_module", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);
    
    // Create code generation context
    codeGenContext_ = std::make_unique<CodeGenContext>(*context_, *module_, *builder_, diagnostics_);
    
    // Setup target machine
    if (!setupTargetMachine()) {
        reportError("Failed to setup target machine", SourceLocation());
    }
    
    // Declare built-in functions
    declareBuiltinFunctions();
}

LLVMCodeGen::~LLVMCodeGen() = default;

bool LLVMCodeGen::generateCode(Module& module, const SymbolTable& symbolTable, 
                              const TypeSystem& typeSystem) {
    symbolTable_ = &symbolTable;
    typeSystem_ = &typeSystem;
    
    try {
        // Generate code for the module
        module.accept(*this);
        
        // Verify the generated module
        std::string errorStr;
        llvm::raw_string_ostream errorStream(errorStr);
        if (llvm::verifyModule(*module_, &errorStream)) {
            reportError("LLVM module verification failed: " + errorStr, SourceLocation());
            return false;
        }
        
        // Optimize the module
        optimizeModule();
        
        return !hasErrors();
        
    } catch (const std::exception& e) {
        reportError("Code generation failed: " + String(e.what()), SourceLocation());
        return false;
    }
}

String LLVMCodeGen::getLLVMIRString() const {
    std::string result;
    llvm::raw_string_ostream stream(result);
    module_->print(stream, nullptr);
    return result;
}

// Visitor implementations
void LLVMCodeGen::visit(NumericLiteral& node) {
    setCurrentValue(createNumberLiteral(node.getValue()));
}

void LLVMCodeGen::visit(StringLiteral& node) {
    setCurrentValue(createStringLiteral(node.getValue()));
}

void LLVMCodeGen::visit(BooleanLiteral& node) {
    setCurrentValue(createBooleanLiteral(node.getValue()));
}

void LLVMCodeGen::visit(NullLiteral& node) {
    setCurrentValue(createNullValue(getAnyType()));
}

void LLVMCodeGen::visit(Identifier& node) {
    // First, check if this identifier refers to a function
    llvm::Function* function = module_->getFunction(node.getName());
    if (function) {
        // This is a function identifier - return the function as a value
        // In LLVM, functions can be treated as values (function pointers)
        setCurrentValue(function);
        return;
    }
    
    // If not a function, try to load as a variable
    llvm::Value* value = loadVariable(node.getName(), node.getLocation());
    if (!value) {
        reportError("Undefined variable: " + node.getName(), node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
    } else {
        setCurrentValue(value);
    }
}

void LLVMCodeGen::visit(ThisExpression& node) {
    // In class methods, 'this' is the first parameter
    llvm::Function* currentFunction = builder_->GetInsertBlock()->getParent();
    
    if (currentFunction->arg_size() > 0) {
        // Get the first argument, which should be 'this'
        llvm::Argument* thisArg = currentFunction->arg_begin();
        setCurrentValue(thisArg);
    } else {
        // Not in a method context, create a null value
        reportError("'this' used outside of method context", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
    }
}

void LLVMCodeGen::visit(SuperExpression& node) {
    // 'super' refers to the parent class instance
    // For now, implement a placeholder that returns the 'this' pointer
    // In a full implementation, this would:
    // 1. Get the current 'this' pointer
    // 2. Cast it to the parent class type
    // 3. Return the parent class reference
    
    llvm::Function* currentFunction = builder_->GetInsertBlock()->getParent();
    
    if (currentFunction->arg_size() > 0) {
        // Get the first argument, which should be 'this'
        // Cast it to parent class type (placeholder implementation)
        llvm::Argument* thisArg = currentFunction->arg_begin();
        setCurrentValue(thisArg);
    } else {
        // Not in a method context, create a null value
        reportError("'super' used outside of method context", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
    }
}

void LLVMCodeGen::visit(NewExpression& node) {
    // For now, implement a basic object allocation
    // In a full implementation, this would:
    // 1. Allocate memory for the object
    // 2. Call the constructor with the allocated memory
    // 3. Return the constructed object
    
    if (auto identifier = dynamic_cast<Identifier*>(node.getConstructor())) {
        // Try to find the constructor function
        llvm::Function* constructor = module_->getFunction(identifier->getName());
        if (constructor) {
            // Allocate space for the object (simplified)
            llvm::Type* objectType = getAnyType(); // Placeholder
            llvm::Value* objectPtr = builder_->CreateAlloca(objectType, nullptr, "new_object");
            
            // Generate arguments for constructor call
            std::vector<llvm::Value*> args;
            args.push_back(objectPtr); // 'this' pointer
            
            for (const auto& arg : node.getArguments()) {
                arg->accept(*this);
                args.push_back(getCurrentValue());
            }
            
            // Call constructor
            if (args.size() <= constructor->arg_size()) {
                builder_->CreateCall(constructor, args);
            }
            
            // Return the object pointer
            setCurrentValue(objectPtr);
        } else {
            reportError("Constructor not found: " + identifier->getName(), node.getLocation());
            setCurrentValue(createNullValue(getAnyType()));
        }
    } else {
        // For complex constructor expressions, create a null value
        reportError("Complex constructor expressions not yet supported", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
    }
}

void LLVMCodeGen::visit(BinaryExpression& node) {
    // Generate left and right operands
    node.getLeft()->accept(*this);
    llvm::Value* left = getCurrentValue();
    
    node.getRight()->accept(*this);
    llvm::Value* right = getCurrentValue();
    
    if (!left || !right) {
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Generate binary operation
    llvm::Value* result = generateBinaryOp(node.getOperator(), left, right, 
                                          left->getType(), right->getType());
    setCurrentValue(result);
}

void LLVMCodeGen::visit(UnaryExpression& node) {
    // Generate operand
    node.getOperand()->accept(*this);
    llvm::Value* operand = getCurrentValue();
    
    if (!operand) {
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Generate unary operation
    llvm::Value* result = generateUnaryOp(static_cast<int>(node.getOperator()), operand, operand->getType());
    setCurrentValue(result);
}

void LLVMCodeGen::visit(AssignmentExpression& node) {
    // Generate right-hand side value
    node.getRight()->accept(*this);
    llvm::Value* value = getCurrentValue();
    
    // Handle left-hand side
    if (auto identifier = dynamic_cast<Identifier*>(node.getLeft())) {
        // Simple variable assignment
        storeVariable(identifier->getName(), value, node.getLocation());
        setCurrentValue(value); // Assignment returns the assigned value
    } else if (auto propertyAccess = dynamic_cast<PropertyAccess*>(node.getLeft())) {
        // Property assignment (e.g., this.value = ...)
        propertyAccess->getObject()->accept(*this);
        llvm::Value* objectPtr = getCurrentValue();
        
        if (objectPtr) {
            // For now, implement a simplified property assignment
            // In a full implementation, we'd need proper object layout and field offsets
            // This is a placeholder that allows compilation to proceed
            
            // For class properties, we need to store the value in the object's memory
            // This is a simplified implementation
            String propertyName = propertyAccess->getProperty();
            
            // Create a simple property store (this is a placeholder)
            // In a real implementation, we'd calculate field offsets
            setCurrentValue(value); // Assignment returns the assigned value
        } else {
            reportError("Failed to generate object for property assignment", node.getLocation());
            setCurrentValue(createNullValue(getAnyType()));
        }
    } else {
        reportError("Invalid assignment target", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
    }
}

void LLVMCodeGen::visit(CallExpression& node) {
    // Generate the callee (function to call)
    node.getCallee()->accept(*this);
    llvm::Value* calleeValue = getCurrentValue();
    
    // For now, assume the callee is an identifier that maps to a function
    // In a full implementation, we'd handle function pointers, method calls, etc.
    llvm::Function* function = nullptr;
    
    if (auto identifier = dynamic_cast<Identifier*>(node.getCallee())) {
        // Look up the function by name
        function = module_->getFunction(identifier->getName());
        if (!function) {
            reportError("Undefined function: " + identifier->getName(), node.getLocation());
            setCurrentValue(createNullValue(getAnyType()));
            return;
        }
    } else if (auto superExpr = dynamic_cast<SuperExpression*>(node.getCallee())) {
        // Handle super() constructor calls
        // For now, this is a simplified implementation
        // In a full implementation, we would:
        // 1. Find the parent class constructor
        // 2. Call it with the current 'this' pointer and arguments
        
        // For now, just create a no-op to allow compilation
        setCurrentValue(createNullValue(getAnyType()));
        return;
    } else {
        reportError("Complex function calls not yet supported", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Generate arguments
    std::vector<llvm::Value*> args;
    for (const auto& arg : node.getArguments()) {
        arg->accept(*this);
        llvm::Value* argValue = getCurrentValue();
        if (!argValue) {
            reportError("Failed to generate argument value", arg->getLocation());
            setCurrentValue(createNullValue(getAnyType()));
            return;
        }
        args.push_back(argValue);
    }
    
    // Generate the function call
    llvm::Value* callResult = builder_->CreateCall(function, args, "call_result");
    setCurrentValue(callResult);
}

void LLVMCodeGen::visit(ArrayLiteral& node) {
    const auto& elements = node.getElements();
    
    if (elements.empty()) {
        // Empty array - return null for now
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // For simplicity, create a stack-allocated array for small arrays
    // In a full implementation, we'd use heap allocation for dynamic arrays
    
    // Determine element type from first element
    elements[0]->accept(*this);
    llvm::Value* firstElement = getCurrentValue();
    if (!firstElement) {
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    llvm::Type* elementType = firstElement->getType();
    size_t arraySize = elements.size();
    
    // Create array type and allocate storage
    llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, arraySize);
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    
    if (!currentFunc) {
        reportError("Array literals not supported at global scope", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Allocate array on stack
    llvm::IRBuilder<> allocaBuilder(&currentFunc->getEntryBlock(), 
                                   currentFunc->getEntryBlock().begin());
    llvm::AllocaInst* arrayStorage = allocaBuilder.CreateAlloca(arrayType, nullptr, "array");
    
    // Initialize array elements
    for (size_t i = 0; i < elements.size(); ++i) {
        // Generate element value
        elements[i]->accept(*this);
        llvm::Value* elementValue = getCurrentValue();
        
        if (!elementValue) {
            reportError("Failed to generate array element", elements[i]->getLocation());
            setCurrentValue(createNullValue(getAnyType()));
            return;
        }
        
        // For now, require all elements to be the same type
        if (elementValue->getType() != elementType) {
            reportError("Array elements must have the same type", elements[i]->getLocation());
            setCurrentValue(createNullValue(getAnyType()));
            return;
        }
        
        // Create GEP to element location
        llvm::Value* indices[] = {
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0),  // Array base
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), i)   // Element index
        };
        llvm::Value* elementPtr = builder_->CreateGEP(arrayType, arrayStorage, indices, "element_ptr");
        
        // Store element
        builder_->CreateStore(elementValue, elementPtr);
    }
    
    // Return pointer to array (for now, return as 'any' type)
    setCurrentValue(arrayStorage);
}

void LLVMCodeGen::visit(IndexExpression& node) {
    // Generate object and index values
    node.getObject()->accept(*this);
    llvm::Value* objectValue = getCurrentValue();
    
    node.getIndex()->accept(*this);
    llvm::Value* indexValue = getCurrentValue();
    
    if (!objectValue || !indexValue) {
        reportError("Failed to generate array indexing", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // For now, implement simple array indexing
    // TODO: Add proper bounds checking and type conversion
    
    // Convert index to integer if needed
    if (indexValue->getType()->isDoubleTy()) {
        indexValue = builder_->CreateFPToSI(indexValue, llvm::Type::getInt32Ty(*context_), "index_int");
    }
    
    // Create GEP to access array element
    std::vector<llvm::Value*> indices = {
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0),
        indexValue
    };
    
    // Determine array and element types from the object
    llvm::Type* arrayType = nullptr;
    llvm::Type* elementType = getAnyType(); // Default fallback
    llvm::Value* arrayPtr = objectValue;
    
    // Try to get the array type from the alloca instruction
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(objectValue)) {
        arrayType = allocaInst->getAllocatedType();
        if (auto* arrType = llvm::dyn_cast<llvm::ArrayType>(arrayType)) {
            elementType = arrType->getElementType();
        }
    } else {
        // objectValue might be a loaded pointer to an array
        // For now, assume it's a pointer to a double array (simplified)
        // In a full implementation, we'd need better type tracking
        elementType = getNumberType(); // Assume double elements
        
        // For GEP, we need the array type, but we only have a pointer
        // This is a limitation of the current approach
        // For now, create a direct GEP with just the index
        llvm::Value* elementPtr = builder_->CreateGEP(elementType, arrayPtr, indexValue, "indexed_element");
        llvm::Value* elementValue = builder_->CreateLoad(elementType, elementPtr, "element_value");
        setCurrentValue(elementValue);
        return;
    }
    
    if (!arrayType) {
        reportError("Cannot determine array type for indexing", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    llvm::Value* elementPtr = builder_->CreateGEP(arrayType, arrayPtr, indices, "indexed_element");
    
    // Load the element value with the correct element type
    llvm::Value* elementValue = builder_->CreateLoad(elementType, elementPtr, "element_value");
    setCurrentValue(elementValue);
}

void LLVMCodeGen::visit(ObjectLiteral& node) {
    const auto& properties = node.getProperties();
    
    if (properties.empty()) {
        // Empty object - return null for now
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // For simplicity, create a basic object representation
    // In a full implementation, we'd create proper struct types
    // For now, store properties in a simple array-like structure
    
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("Object literals not supported at global scope", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Create a simple object as an array of numbers
    // HACK: For basic testing, assume all object properties are numbers
    // This is a very simplified approach
    size_t numProperties = properties.size();
    llvm::ArrayType* objectType = llvm::ArrayType::get(getNumberType(), numProperties);
    
    // Allocate object on stack
    llvm::IRBuilder<> allocaBuilder(&currentFunc->getEntryBlock(), 
                                   currentFunc->getEntryBlock().begin());
    llvm::AllocaInst* objectStorage = allocaBuilder.CreateAlloca(objectType, nullptr, "object");
    
    // Initialize object properties
    for (size_t i = 0; i < properties.size(); ++i) {
        // Generate property value
        properties[i].getValue()->accept(*this);
        llvm::Value* propertyValue = getCurrentValue();
        
        if (!propertyValue) {
            reportError("Failed to generate object property", properties[i].getValue()->getLocation());
            setCurrentValue(createNullValue(getAnyType()));
            return;
        }
        
        // For now, assume all properties are stored as "any" type
        // In a full implementation, we'd preserve type information
        
        // Create GEP to property location
        llvm::Value* indices[] = {
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0),  // Object base
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), i)   // Property index
        };
        llvm::Value* propertyPtr = builder_->CreateGEP(objectType, objectStorage, indices, "property_ptr");
        
        // Store property (simplified - we're losing type information)
        builder_->CreateStore(propertyValue, propertyPtr);
    }
    
    // Return pointer to object
    setCurrentValue(objectStorage);
}

void LLVMCodeGen::visit(PropertyAccess& node) {
    // Check if this is an enum member access first
    // For enum member access, we don't need the object value, we need the global constant
    if (auto* identifier = dynamic_cast<Identifier*>(node.getObject())) {
        // Look up the identifier to see if it's an enum type
        Symbol* symbol = symbolTable_->lookupSymbol(identifier->getName());
        if (symbol && symbol->getType()->getKind() == TypeKind::Enum) {
            // This is enum member access - return the global constant
            String memberName = node.getProperty();
            String globalName = identifier->getName() + "_" + memberName;
            
            // Look up the global constant
            llvm::GlobalVariable* globalVar = module_->getGlobalVariable(globalName);
            if (globalVar) {
                // Load the constant value
                llvm::Value* memberValue = builder_->CreateLoad(globalVar->getValueType(), globalVar, "enum_member");
                setCurrentValue(memberValue);
                return;
            } else {
                reportError("Enum member '" + globalName + "' not found", node.getLocation());
                setCurrentValue(createNullValue(getNumberType()));
                return;
            }
        }
    }
    
    // Generate the object for non-enum property access
    node.getObject()->accept(*this);
    llvm::Value* objectValue = getCurrentValue();
    
    if (!objectValue) {
        reportError("Failed to generate object for property access", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // For now, implement a very simplified property access
    // In a full implementation, we'd need:
    // 1. Property name -> index mapping
    // 2. Type information preservation
    // 3. Dynamic property lookup
    
    // This is a major limitation: we can't easily map property names to indices
    // without additional metadata. For now, assume properties are accessed by order:
    // - First property (index 0) for any property access
    // This is obviously incorrect but allows basic testing
    
    llvm::Type* objectType = nullptr;
    llvm::Type* propertyType = getAnyType(); // Default fallback
    
    // Try to get the object type from the alloca instruction
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(objectValue)) {
        objectType = allocaInst->getAllocatedType();
    } else {
        // objectValue might be a loaded pointer to an object
        // For now, assume it's a pointer to the first property (simplified)
        
        // HACK: For basic testing, assume all object properties are numbers
        // This is a major simplification but allows basic functionality
        propertyType = getNumberType(); // Use double type for numeric properties
        
        // Create a simple GEP to the first property (index 0)
        llvm::Value* propertyPtr = builder_->CreateGEP(propertyType, objectValue, 
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0), "property_ptr");
        llvm::Value* propertyValue = builder_->CreateLoad(propertyType, propertyPtr, "property_value");
        setCurrentValue(propertyValue);
        return;
    }
    
    if (!objectType || !llvm::isa<llvm::ArrayType>(objectType)) {
        reportError("Cannot determine object type for property access", node.getLocation());
        setCurrentValue(createNullValue(getAnyType()));
        return;
    }
    
    // Access the first property (index 0) - this is a major simplification
    llvm::Value* indices[] = {
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0),  // Object base
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0)   // First property
    };
    
    // HACK: For basic testing, assume all object properties are numbers
    propertyType = getNumberType(); // Use double type for numeric properties
    
    llvm::Value* propertyPtr = builder_->CreateGEP(objectType, objectValue, indices, "property_ptr");
    llvm::Value* propertyValue = builder_->CreateLoad(propertyType, propertyPtr, "property_value");
    setCurrentValue(propertyValue);
}

void LLVMCodeGen::visit(ArrowFunction& node) {
    // Arrow functions are similar to regular functions but are expressions
    // For now, implement a simplified version that creates an anonymous function
    
    // Generate function name (anonymous)
    static int arrowFunctionCounter = 0;
    String functionName = "arrow_function_" + std::to_string(arrowFunctionCounter++);
    
    // Create parameter types
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : node.getParameters()) {
        if (param.type) {
            paramTypes.push_back(mapTypeScriptTypeToLLVM(*param.type));
        } else {
            paramTypes.push_back(getAnyType());
        }
    }
    
    // Determine return type
    llvm::Type* returnType = getVoidType(); // Default
    if (node.getReturnType()) {
        returnType = mapTypeScriptTypeToLLVM(*node.getReturnType());
    } else {
        // For arrow functions, try to infer from body
        returnType = getAnyType(); // Simplified - assume any type
    }
    
    // Create function type
    llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
    
    // Create function
    llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::InternalLinkage, 
                                                     functionName, module_.get());
    
    // Save current insertion point
    llvm::BasicBlock* savedBlock = builder_->GetInsertBlock();
    llvm::Function* savedFunction = codeGenContext_->getCurrentFunction();
    
    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context_, "entry", function);
    builder_->SetInsertPoint(entryBlock);
    codeGenContext_->enterFunction(function);
    
    // Create parameters and add to symbol table
    auto paramIt = function->arg_begin();
    for (size_t i = 0; i < node.getParameters().size(); ++i, ++paramIt) {
        const auto& param = node.getParameters()[i];
        llvm::Argument* arg = &(*paramIt);
        arg->setName(param.name);
        
        // Allocate space for parameter
        llvm::Value* paramStorage = allocateVariable(param.name, arg->getType(), param.location);
        builder_->CreateStore(arg, paramStorage);
    }
    
    // Generate function body
    node.getBody()->accept(*this);
    
    // Ensure function has a return
    if (!builder_->GetInsertBlock()->getTerminator()) {
        if (returnType->isVoidTy()) {
            builder_->CreateRetVoid();
        } else {
            // Return default value for the type
            llvm::Value* defaultValue = createDefaultValue(returnType);
            builder_->CreateRet(defaultValue);
        }
    }
    
    // Restore insertion point
    if (savedBlock) {
        builder_->SetInsertPoint(savedBlock);
    }
    if (savedFunction) {
        codeGenContext_->enterFunction(savedFunction);
    } else {
        codeGenContext_->exitFunction();
    }
    
    // Return the function as a value (function pointer)
    setCurrentValue(function);
}

void LLVMCodeGen::visit(ExpressionStatement& node) {
    node.getExpression()->accept(*this);
    // Expression statement doesn't return a value
}

void LLVMCodeGen::visit(BlockStatement& node) {
    codeGenContext_->enterScope();
    
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
        if (hasErrors()) break;
    }
    
    codeGenContext_->exitScope();
}

void LLVMCodeGen::visit(ReturnStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("Return statement outside function", node.getLocation());
        return;
    }
    
    if (node.hasValue()) {
        // Generate code for return value
        node.getValue()->accept(*this);
        llvm::Value* returnValue = getCurrentValue();
        
        if (returnValue) {
            // Convert to appropriate return type if needed
            llvm::Type* returnType = currentFunc->getReturnType();
            if (returnValue->getType() != returnType) {
                // Perform type conversion
                returnValue = convertValueToType(returnValue, returnType);
            }
            builder_->CreateRet(returnValue);
        } else {
            reportError("Failed to generate return value", node.getLocation());
            builder_->CreateRetVoid();
        }
    } else {
        // Return void
        builder_->CreateRetVoid();
    }
}

void LLVMCodeGen::visit(IfStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("If statement outside function", node.getLocation());
        return;
    }
    
    // Generate condition
    node.getCondition()->accept(*this);
    llvm::Value* conditionValue = getCurrentValue();
    
    if (!conditionValue) {
        reportError("Failed to generate condition", node.getLocation());
        return;
    }
    
    // Convert condition to boolean (simplified for now)
    if (conditionValue->getType()->isDoubleTy()) {
        // Compare double to 0.0
        llvm::Value* zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_), 0.0);
        conditionValue = builder_->CreateFCmpONE(conditionValue, zero, "tobool");
    } else if (conditionValue->getType()->isIntegerTy(1)) {
        // Already boolean
    } else {
        // For other types, just use as-is (this is a simplification)
        // TODO: Add proper type conversion
    }
    
    // Create basic blocks
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(*context_, "if.then", currentFunc);
    llvm::BasicBlock* elseBlock = node.hasElse() ? 
        llvm::BasicBlock::Create(*context_, "if.else", currentFunc) : nullptr;
    llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(*context_, "if.end", currentFunc);
    
    // Create conditional branch
    if (node.hasElse()) {
        builder_->CreateCondBr(conditionValue, thenBlock, elseBlock);
    } else {
        builder_->CreateCondBr(conditionValue, thenBlock, endBlock);
    }
    
    // Generate then block
    builder_->SetInsertPoint(thenBlock);
    node.getThenStatement()->accept(*this);
    bool thenHasTerminator = builder_->GetInsertBlock()->getTerminator() != nullptr;
    if (!thenHasTerminator) {
        builder_->CreateBr(endBlock);
    }
    
    // Generate else block if present
    bool elseHasTerminator = false;
    if (node.hasElse()) {
        builder_->SetInsertPoint(elseBlock);
        node.getElseStatement()->accept(*this);
        elseHasTerminator = builder_->GetInsertBlock()->getTerminator() != nullptr;
        if (!elseHasTerminator) {
            builder_->CreateBr(endBlock);
        }
    }
    
    // Handle end block
    bool bothBranchesTerminate = thenHasTerminator && (!node.hasElse() || elseHasTerminator);
    
    if (bothBranchesTerminate) {
        // Both branches terminate (or only then branch exists and terminates)
        // End block is unreachable - remove it to avoid verification errors
        endBlock->eraseFromParent();
    } else {
        // At least one branch doesn't terminate - continue with end block
        builder_->SetInsertPoint(endBlock);
    }
}

void LLVMCodeGen::visit(WhileStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("While statement outside function", node.getLocation());
        return;
    }
    
    // Create basic blocks
    llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(*context_, "while.cond", currentFunc);
    llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(*context_, "while.body", currentFunc);
    llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(*context_, "while.end", currentFunc);
    
    // Jump to condition block
    builder_->CreateBr(conditionBlock);
    
    // Generate condition
    builder_->SetInsertPoint(conditionBlock);
    node.getCondition()->accept(*this);
    llvm::Value* conditionValue = getCurrentValue();
    
    if (!conditionValue) {
        reportError("Failed to generate while condition", node.getCondition()->getLocation());
        return;
    }
    
    // Convert condition to boolean (simplified for now)
    if (conditionValue->getType()->isDoubleTy()) {
        // Compare double to 0.0
        llvm::Value* zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_), 0.0);
        conditionValue = builder_->CreateFCmpONE(conditionValue, zero, "tobool");
    } else if (conditionValue->getType()->isIntegerTy(1)) {
        // Already boolean
    } else {
        // For other types, just use as-is (this is a simplification)
        // TODO: Add proper type conversion
    }
    
    // Create conditional branch
    builder_->CreateCondBr(conditionValue, bodyBlock, endBlock);
    
    // Generate body
    builder_->SetInsertPoint(bodyBlock);
    node.getBody()->accept(*this);
    if (!builder_->GetInsertBlock()->getTerminator()) {
        builder_->CreateBr(conditionBlock);
    }
    
    // Continue with end block
    builder_->SetInsertPoint(endBlock);
}

void LLVMCodeGen::visit(DoWhileStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("Do-while statement outside function", node.getLocation());
        return;
    }
    
    // Create basic blocks
    llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(*context_, "do.body", currentFunc);
    llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(*context_, "do.cond", currentFunc);
    llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(*context_, "do.end", currentFunc);
    
    // Jump to body block (do-while executes body at least once)
    builder_->CreateBr(bodyBlock);
    
    // Generate body
    builder_->SetInsertPoint(bodyBlock);
    node.getBody()->accept(*this);
    if (!builder_->GetInsertBlock()->getTerminator()) {
        builder_->CreateBr(conditionBlock);
    }
    
    // Generate condition
    builder_->SetInsertPoint(conditionBlock);
    node.getCondition()->accept(*this);
    llvm::Value* conditionValue = getCurrentValue();
    
    if (!conditionValue) {
        reportError("Failed to generate do-while condition", node.getCondition()->getLocation());
        return;
    }
    
    // Convert condition to boolean (simplified for now)
    if (conditionValue->getType()->isDoubleTy()) {
        // Compare double to 0.0
        llvm::Value* zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_), 0.0);
        conditionValue = builder_->CreateFCmpONE(conditionValue, zero, "tobool");
    } else if (conditionValue->getType()->isIntegerTy(1)) {
        // Already boolean
    } else {
        // For other types, just use as-is (this is a simplification)
        // TODO: Add proper type conversion
    }
    
    // Create conditional branch (continue if true, exit if false)
    builder_->CreateCondBr(conditionValue, bodyBlock, endBlock);
    
    // Continue with end block
    builder_->SetInsertPoint(endBlock);
}

void LLVMCodeGen::visit(ForStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("For statement outside function", node.getLocation());
        return;
    }
    
    // Generate init if present
    if (node.getInit()) {
        node.getInit()->accept(*this);
    }
    
    // Create basic blocks
    llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(*context_, "for.cond", currentFunc);
    llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(*context_, "for.body", currentFunc);
    llvm::BasicBlock* incrementBlock = llvm::BasicBlock::Create(*context_, "for.inc", currentFunc);
    llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(*context_, "for.end", currentFunc);
    
    // Jump to condition block
    builder_->CreateBr(conditionBlock);
    
    // Generate condition
    builder_->SetInsertPoint(conditionBlock);
    if (node.getCondition()) {
        node.getCondition()->accept(*this);
        llvm::Value* conditionValue = getCurrentValue();
        
        if (!conditionValue) {
            reportError("Failed to generate for condition", node.getCondition()->getLocation());
            return;
        }
        
        // Convert condition to boolean (simplified for now)
        if (conditionValue->getType()->isDoubleTy()) {
            // Compare double to 0.0
            llvm::Value* zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_), 0.0);
            conditionValue = builder_->CreateFCmpONE(conditionValue, zero, "tobool");
        } else if (conditionValue->getType()->isIntegerTy(1)) {
            // Already boolean
        } else {
            // For other types, just use as-is (this is a simplification)
            // TODO: Add proper type conversion
        }
        
        // Create conditional branch
        builder_->CreateCondBr(conditionValue, bodyBlock, endBlock);
    } else {
        // No condition means infinite loop (like for(;;))
        builder_->CreateBr(bodyBlock);
    }
    
    // Generate body
    builder_->SetInsertPoint(bodyBlock);
    node.getBody()->accept(*this);
    if (!builder_->GetInsertBlock()->getTerminator()) {
        builder_->CreateBr(incrementBlock);
    }
    
    // Generate increment
    builder_->SetInsertPoint(incrementBlock);
    if (node.getIncrement()) {
        node.getIncrement()->accept(*this);
    }
    builder_->CreateBr(conditionBlock);
    
    // Continue with end block
    builder_->SetInsertPoint(endBlock);
}

void LLVMCodeGen::visit(SwitchStatement& node) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("Switch statement outside function", node.getLocation());
        return;
    }
    
    // Generate discriminant
    node.getDiscriminant()->accept(*this);
    llvm::Value* discriminantValue = getCurrentValue();
    
    if (!discriminantValue) {
        reportError("Failed to generate switch discriminant", node.getDiscriminant()->getLocation());
        return;
    }
    
    // Convert discriminant to integer if it's a floating point
    if (discriminantValue->getType()->isDoubleTy()) {
        discriminantValue = builder_->CreateFPToSI(discriminantValue, 
            llvm::Type::getInt32Ty(*context_), "switch.discriminant.int");
    }
    
    // Create basic blocks
    llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(*context_, "switch.end", currentFunc);
    llvm::BasicBlock* defaultBlock = endBlock;  // Default to end block if no default case
    
    // Create blocks for each case
    std::vector<std::pair<llvm::ConstantInt*, llvm::BasicBlock*>> caseBlocks;
    
    for (size_t i = 0; i < node.getCases().size(); ++i) {
        const auto& caseClause = node.getCases()[i];
        
        if (caseClause->isDefault()) {
            defaultBlock = llvm::BasicBlock::Create(*context_, "switch.default", currentFunc);
        } else {
            llvm::BasicBlock* caseBlock = llvm::BasicBlock::Create(*context_, 
                "switch.case" + std::to_string(i), currentFunc);
            
            // For now, assume case values are integer constants
            // TODO: Add proper constant evaluation
            if (auto numLit = dynamic_cast<NumericLiteral*>(caseClause->getTest())) {
                llvm::ConstantInt* caseValue = llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(*context_), (int)numLit->getValue());
                caseBlocks.push_back({caseValue, caseBlock});
            }
        }
    }
    
    // Create switch instruction
    llvm::SwitchInst* switchInst = builder_->CreateSwitch(discriminantValue, defaultBlock, caseBlocks.size());
    
    // Add cases to switch instruction
    for (const auto& [caseValue, caseBlock] : caseBlocks) {
        switchInst->addCase(caseValue, caseBlock);
    }
    
    // Generate code for each case
    for (size_t i = 0; i < node.getCases().size(); ++i) {
        const auto& caseClause = node.getCases()[i];
        llvm::BasicBlock* caseBlock;
        
        if (caseClause->isDefault()) {
            caseBlock = defaultBlock;
        } else {
            // Find the corresponding case block
            auto it = std::find_if(caseBlocks.begin(), caseBlocks.end(),
                [i](const auto& pair) { return pair.second->getName().contains(std::to_string(i)); });
            if (it != caseBlocks.end()) {
                caseBlock = it->second;
            } else {
                continue;  // Skip if we couldn't create the case block
            }
        }
        
        builder_->SetInsertPoint(caseBlock);
        caseClause->accept(*this);
        
        // If no terminator was added (no break/return), fall through to next case
        if (!builder_->GetInsertBlock()->getTerminator()) {
            if (i + 1 < node.getCases().size()) {
                // Fall through to next case (simplified - should find next case block)
                builder_->CreateBr(endBlock);
            } else {
                builder_->CreateBr(endBlock);
            }
        }
    }
    
    // Continue with end block
    builder_->SetInsertPoint(endBlock);
}

void LLVMCodeGen::visit(CaseClause& node) {
    // Generate statements in this case
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
}

void LLVMCodeGen::visit(BreakStatement& node) {
    // TODO: Implement proper break handling with loop/switch context tracking
    // For now, just create an unreachable instruction as placeholder
    builder_->CreateUnreachable();
}

void LLVMCodeGen::visit(ContinueStatement& node) {
    // TODO: Implement proper continue handling with loop context tracking
    // For now, just create an unreachable instruction as placeholder
    builder_->CreateUnreachable();
}

void LLVMCodeGen::visit(TryStatement& node) {
    // Simplified try-catch-finally implementation
    // This doesn't implement proper exception handling but allows basic execution flow
    
    // Generate the try block
    node.getTryBlock()->accept(*this);
    
    // If there's a finally block, always execute it
    if (node.getFinallyBlock()) {
        node.getFinallyBlock()->accept(*this);
    }
    
    // Note: catch blocks are not implemented in this simplified version
    // In a full implementation, we would need LLVM exception handling with:
    // - Invoke instructions instead of Call instructions
    // - Landing pads for exception handling
    // - Personality functions and exception tables
}

void LLVMCodeGen::visit(CatchClause& node) {
    // Simplified catch clause implementation
    // In this simplified version, catch clauses are not executed
    // The catch body is not generated to avoid execution
    
    // Note: In a full implementation, this would:
    // - Handle the exception parameter binding
    // - Generate code for the catch body within exception handling context
    // - Set up proper exception type matching
}

void LLVMCodeGen::visit(ThrowStatement& node) {
    // TODO: Implement proper exception throwing with LLVM
    // For now, just generate the expression and create unreachable
    reportError("Throw statements not yet fully implemented in code generation", node.getLocation());
    
    // Generate the expression being thrown
    node.getExpression()->accept(*this);
    
    // Create unreachable to indicate this path doesn't continue
    builder_->CreateUnreachable();
}

void LLVMCodeGen::visit(VariableDeclaration& node) {
    // Generate initializer first to determine the type
    llvm::Value* initValue = nullptr;
    llvm::Type* llvmType = getAnyType(); // Default to any type
    
    if (node.getInitializer()) {
        node.getInitializer()->accept(*this);
        initValue = getCurrentValue();
        
        // Use the initializer's type if available
        if (initValue) {
            llvmType = initValue->getType();
        }
    }
    
    // Allocate storage for the variable
    llvm::Value* storage = allocateVariable(node.getName(), llvmType, node.getLocation());
    
    // Store the initializer if present
    if (initValue && storage) {
        llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
        if (currentFunc) {
            // Local variable - store normally
            builder_->CreateStore(initValue, storage);
        } else {
            // Global variable - set initial value if it's a global variable
            if (auto* globalVar = llvm::dyn_cast<llvm::GlobalVariable>(storage)) {
                if (auto* constant = llvm::dyn_cast<llvm::Constant>(initValue)) {
                    globalVar->setInitializer(constant);
                } else {
                    // For non-constant initializers, we need to defer initialization
                    // to a constructor function or main function
                    reportError("Global variable initializer must be constant", node.getLocation());
                }
            }
        }
    }
}

void LLVMCodeGen::visit(FunctionDeclaration& node) {
    // Generate function declaration
    llvm::Function* function = generateFunctionDeclaration(node);
    if (!function) {
        reportError("Failed to generate function: " + node.getName(), node.getLocation());
        return;
    }
    
    // Generate function body
    generateFunctionBody(function, node);
}

void LLVMCodeGen::visit(TypeParameter& node) {
    // Type parameters don't generate code - they're compile-time constructs
    // In a full implementation with runtime type information, we might generate
    // metadata or type information here
    // For now, this is a no-op
    setCurrentValue(llvm::Constant::getNullValue(getAnyType()));
}

void LLVMCodeGen::visit(Module& module) {
    // Set module name
    module_->setModuleIdentifier(module.getFilename());
    
    // Generate all statements in the module
    for (const auto& stmt : module.getStatements()) {
        stmt->accept(*this);
        if (hasErrors()) break;
    }
    
    // Create a main function if none exists
    if (!module_->getFunction("main")) {
        llvm::FunctionType* mainType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(*context_), false);
        llvm::Function* mainFunc = llvm::Function::Create(
            mainType, llvm::Function::ExternalLinkage, "main", module_.get());
        
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context_, "entry", mainFunc);
        builder_->SetInsertPoint(entry);
        
        // Return 0 from main
        builder_->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0));
    }
}

// Type mapping implementation
llvm::Type* LLVMCodeGen::mapTypeScriptTypeToLLVM(const Type& type) {
    switch (type.getKind()) {
        case TypeKind::Number:
            return getNumberType();
        case TypeKind::String:
            return getStringType();
        case TypeKind::Boolean:
            return getBooleanType();
        case TypeKind::Void:
            return getVoidType();
        case TypeKind::Any:
        default:
            return getAnyType();
    }
}

llvm::Type* LLVMCodeGen::getNumberType() const {
    return llvm::Type::getDoubleTy(*context_);
}

llvm::Type* LLVMCodeGen::getStringType() const {
    // Use i8* for strings (C-style strings for now)
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0);
}

llvm::Type* LLVMCodeGen::getBooleanType() const {
    return llvm::Type::getInt1Ty(*context_);
}

llvm::Type* LLVMCodeGen::getVoidType() const {
    return llvm::Type::getVoidTy(*context_);
}

llvm::Type* LLVMCodeGen::getAnyType() const {
    // Use i8* as a generic pointer type for 'any'
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0);
}

llvm::Type* LLVMCodeGen::convertTypeToLLVM(shared_ptr<Type> type) {
    if (!type) {
        return getAnyType();
    }
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::Value* LLVMCodeGen::convertValueToType(llvm::Value* value, llvm::Type* targetType) {
    if (!value || !targetType) {
        return value;
    }
    
    llvm::Type* sourceType = value->getType();
    if (sourceType == targetType) {
        return value; // No conversion needed
    }
    
    // Handle common type conversions
    if (targetType->isPointerTy() && sourceType->isDoubleTy()) {
        // Convert number to any (pointer) - box the value
        // For now, just cast to pointer (this is a simplification)
        return builder_->CreateIntToPtr(
            builder_->CreateFPToUI(value, llvm::Type::getInt64Ty(*context_)),
            targetType
        );
    } else if (targetType->isDoubleTy() && sourceType->isPointerTy()) {
        // Convert any (pointer) to number - unbox the value
        return builder_->CreateUIToFP(
            builder_->CreatePtrToInt(value, llvm::Type::getInt64Ty(*context_)),
            targetType
        );
    } else if (targetType->isPointerTy()) {
        // Convert to any type (pointer)
        if (sourceType->isIntegerTy()) {
            return builder_->CreateIntToPtr(value, targetType);
        } else if (sourceType->isDoubleTy()) {
            return builder_->CreateIntToPtr(
                builder_->CreateFPToUI(value, llvm::Type::getInt64Ty(*context_)),
                targetType
            );
        }
    }
    
    // If no specific conversion is available, return the original value
    // This might still cause LLVM verification errors, but it's better than crashing
    return value;
}

// Value creation implementation
llvm::Value* LLVMCodeGen::createNumberLiteral(double value) {
    return llvm::ConstantFP::get(getNumberType(), value);
}

llvm::Value* LLVMCodeGen::createStringLiteral(const String& value) {
    // Create a constant string array
    llvm::Constant* stringConstant = llvm::ConstantDataArray::getString(*context_, value, true);
    
    // Create a global variable to hold the string
    llvm::GlobalVariable* globalString = new llvm::GlobalVariable(
        *module_, stringConstant->getType(), true, llvm::GlobalValue::PrivateLinkage,
        stringConstant, "str");
    
    // Return a pointer to the string (cast to i8*)
    return llvm::ConstantExpr::getPointerCast(globalString, getStringType());
}

llvm::Value* LLVMCodeGen::createBooleanLiteral(bool value) {
    return llvm::ConstantInt::get(getBooleanType(), value ? 1 : 0);
}

llvm::Value* LLVMCodeGen::createNullValue(llvm::Type* type) {
    return llvm::Constant::getNullValue(type);
}

llvm::Value* LLVMCodeGen::createDefaultValue(llvm::Type* type) {
    if (type->isDoubleTy()) {
        return llvm::ConstantFP::get(type, 0.0);
    } else if (type->isIntegerTy()) {
        return llvm::ConstantInt::get(type, 0);
    } else if (type->isPointerTy()) {
        return llvm::Constant::getNullValue(type);
    } else {
        return llvm::Constant::getNullValue(type);
    }
}

// Binary operations implementation
llvm::Value* LLVMCodeGen::generateBinaryOp(BinaryExpression::Operator op, llvm::Value* left, 
                                           llvm::Value* right, llvm::Type* leftType, llvm::Type* rightType) {
    switch (op) {
        case BinaryExpression::Operator::Add:
            // Check if it's string concatenation
            if (leftType->isPointerTy() || rightType->isPointerTy()) {
                return generateStringConcat(left, right);
            }
            return generateArithmeticOp(op, left, right);
            
        case BinaryExpression::Operator::Subtract:
        case BinaryExpression::Operator::Multiply:
        case BinaryExpression::Operator::Divide:
            return generateArithmeticOp(op, left, right);
            
        case BinaryExpression::Operator::Equal:
        case BinaryExpression::Operator::NotEqual:
        case BinaryExpression::Operator::Less:
        case BinaryExpression::Operator::Greater:
        case BinaryExpression::Operator::LessEqual:
        case BinaryExpression::Operator::GreaterEqual:
            return generateComparisonOp(op, left, right);
            
        case BinaryExpression::Operator::LogicalAnd:
        case BinaryExpression::Operator::LogicalOr:
            return generateLogicalOp(op, left, right);
            
        default:
            reportError("Unsupported binary operator", SourceLocation());
            return createNullValue(getAnyType());
    }
}

llvm::Value* LLVMCodeGen::generateArithmeticOp(BinaryExpression::Operator op, 
                                              llvm::Value* left, llvm::Value* right) {
    // Convert operands to numbers if needed
    left = convertToNumber(left, left->getType());
    right = convertToNumber(right, right->getType());
    
    // Check if we can do constant folding
    if (auto* leftConst = llvm::dyn_cast<llvm::ConstantFP>(left)) {
        if (auto* rightConst = llvm::dyn_cast<llvm::ConstantFP>(right)) {
            double leftVal = leftConst->getValueAPF().convertToDouble();
            double rightVal = rightConst->getValueAPF().convertToDouble();
            double result;
            
            switch (op) {
                case BinaryExpression::Operator::Add:
                    result = leftVal + rightVal;
                    break;
                case BinaryExpression::Operator::Subtract:
                    result = leftVal - rightVal;
                    break;
                case BinaryExpression::Operator::Multiply:
                    result = leftVal * rightVal;
                    break;
                case BinaryExpression::Operator::Divide:
                    result = leftVal / rightVal;
                    break;
                default:
                    result = 0.0;
                    break;
            }
            
            return createNumberLiteral(result);
        }
    }
    
    // Check if we have a function context for runtime operations
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        reportError("Cannot perform runtime arithmetic operations in global scope", SourceLocation());
        return createNumberLiteral(0.0);
    }
    
    switch (op) {
        case BinaryExpression::Operator::Add:
            return builder_->CreateFAdd(left, right, "add");
        case BinaryExpression::Operator::Subtract:
            return builder_->CreateFSub(left, right, "sub");
        case BinaryExpression::Operator::Multiply:
            return builder_->CreateFMul(left, right, "mul");
        case BinaryExpression::Operator::Divide:
            return builder_->CreateFDiv(left, right, "div");
        default:
            return createNullValue(getNumberType());
    }
}

llvm::Value* LLVMCodeGen::generateComparisonOp(BinaryExpression::Operator op, 
                                              llvm::Value* left, llvm::Value* right) {
    // Convert operands to numbers for comparison
    left = convertToNumber(left, left->getType());
    right = convertToNumber(right, right->getType());
    
    switch (op) {
        case BinaryExpression::Operator::Equal:
            return builder_->CreateFCmpOEQ(left, right, "eq");
        case BinaryExpression::Operator::NotEqual:
            return builder_->CreateFCmpONE(left, right, "ne");
        case BinaryExpression::Operator::Less:
            return builder_->CreateFCmpOLT(left, right, "lt");
        case BinaryExpression::Operator::Greater:
            return builder_->CreateFCmpOGT(left, right, "gt");
        case BinaryExpression::Operator::LessEqual:
            return builder_->CreateFCmpOLE(left, right, "le");
        case BinaryExpression::Operator::GreaterEqual:
            return builder_->CreateFCmpOGE(left, right, "ge");
        default:
            return createBooleanLiteral(false);
    }
}

llvm::Value* LLVMCodeGen::generateLogicalOp(BinaryExpression::Operator op, 
                                           llvm::Value* left, llvm::Value* right) {
    // Convert operands to booleans
    left = convertToBoolean(left, left->getType());
    right = convertToBoolean(right, right->getType());
    
    switch (op) {
        case BinaryExpression::Operator::LogicalAnd:
            return builder_->CreateAnd(left, right, "and");
        case BinaryExpression::Operator::LogicalOr:
            return builder_->CreateOr(left, right, "or");
        default:
            return createBooleanLiteral(false);
    }
}

llvm::Value* LLVMCodeGen::generateStringConcat(llvm::Value* left, llvm::Value* right) {
    // For now, use a simple string concatenation function
    llvm::Function* concatFunc = getOrCreateStringConcatFunction();
    return builder_->CreateCall(concatFunc, {left, right}, "strcat");
}

// Unary operations implementation
llvm::Value* LLVMCodeGen::generateUnaryOp(int op, llvm::Value* operand, llvm::Type* operandType) {
    // Simplified unary operations for now
    if (op == 0) { // Plus
        return convertToNumber(operand, operandType);
    }
    if (op == 1) { // Minus
        operand = convertToNumber(operand, operandType);
        return builder_->CreateFNeg(operand, "neg");
    }
    if (op == 2) { // LogicalNot
        operand = convertToBoolean(operand, operandType);
        return builder_->CreateNot(operand, "not");
    }
    
    reportError("Unsupported unary operator", SourceLocation());
    return createNullValue(getAnyType());
}

// Type conversion implementation
llvm::Value* LLVMCodeGen::convertToNumber(llvm::Value* value, llvm::Type* fromType) {
    if (fromType->isDoubleTy()) {
        return value; // Already a number
    }
    if (fromType->isIntegerTy(1)) {
        // Boolean to number - check if we have a function context
        llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
        if (currentFunc) {
            return builder_->CreateUIToFP(value, getNumberType(), "bool_to_num");
        } else {
            // At global scope, can only handle constant booleans
            if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(value)) {
                return createNumberLiteral(constInt->isZero() ? 0.0 : 1.0);
            } else {
                reportError("Cannot convert non-constant boolean to number in global scope", SourceLocation());
                return createNumberLiteral(0.0);
            }
        }
    }
    // For other types, return 0.0 for now
    return createNumberLiteral(0.0);
}

llvm::Value* LLVMCodeGen::convertToBoolean(llvm::Value* value, llvm::Type* fromType) {
    if (fromType->isIntegerTy(1)) {
        return value; // Already a boolean
    }
    if (fromType->isDoubleTy()) {
        // Number to boolean (non-zero is true)
        llvm::Value* zero = createNumberLiteral(0.0);
        return builder_->CreateFCmpONE(value, zero, "num_to_bool");
    }
    // For other types, return false for now
    return createBooleanLiteral(false);
}

// Function generation implementation
llvm::Function* LLVMCodeGen::generateFunctionDeclaration(const FunctionDeclaration& funcDecl) {
    // Create parameter types
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : funcDecl.getParameters()) {
        if (param.type) {
            // Use the explicit parameter type if available
            paramTypes.push_back(mapTypeScriptTypeToLLVM(*param.type));
        } else {
            // Default to 'any' type if no explicit type
            paramTypes.push_back(getAnyType());
        }
    }
    
    // Determine return type from function declaration
    llvm::Type* returnType = getVoidType(); // Default to void
    if (funcDecl.getReturnType()) {
        returnType = mapTypeScriptTypeToLLVM(*funcDecl.getReturnType());
    } else {
        // If no explicit return type, analyze function body to infer return type
        if (hasReturnStatements(funcDecl)) {
            // Function has return statements, so it likely returns a value
            returnType = getAnyType();
        } else {
            // Function has no return statements, so it's likely void
            returnType = getVoidType();
        }
    }
    
    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, paramTypes, false);
    
    // Create function
    llvm::Function* function = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, funcDecl.getName(), module_.get());
    
    // Set parameter names
    auto paramIt = funcDecl.getParameters().begin();
    for (auto& arg : function->args()) {
        if (paramIt != funcDecl.getParameters().end()) {
            arg.setName(paramIt->name);
            ++paramIt;
        }
    }
    
    return function;
}

void LLVMCodeGen::generateFunctionBody(llvm::Function* function, const FunctionDeclaration& funcDecl) {
    // Create entry block
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context_, "entry", function);
    builder_->SetInsertPoint(entry);
    
    // Enter function context
    codeGenContext_->enterFunction(function);
    codeGenContext_->enterScope();
    
    // Add parameters to symbol table
    auto paramIt = funcDecl.getParameters().begin();
    for (auto& arg : function->args()) {
        if (paramIt != funcDecl.getParameters().end()) {
            // Allocate storage for parameter
            llvm::Value* storage = allocateVariable(paramIt->name, arg.getType(), SourceLocation());
            builder_->CreateStore(&arg, storage);
            ++paramIt;
        }
    }
    
    // Generate function body
    if (funcDecl.getBody()) {
        funcDecl.getBody()->accept(*this);
    }
    
    // Add return if not present
    if (!builder_->GetInsertBlock()->getTerminator()) {
        llvm::Type* returnType = function->getReturnType();
        if (returnType->isVoidTy()) {
            builder_->CreateRetVoid();
        } else {
            // Return a default value of the appropriate type
            // For functions without explicit return statements, return null/undefined
            llvm::Value* defaultValue = createDefaultValue(returnType);
            builder_->CreateRet(defaultValue);
        }
    }
    
    // Exit function context
    codeGenContext_->exitScope();
    codeGenContext_->exitFunction();
}

bool LLVMCodeGen::hasReturnStatements(const FunctionDeclaration& funcDecl) {
    // Simple visitor to check if function body contains return statements
    class ReturnStatementChecker : public ASTVisitor {
    private:
        bool found_ = false;
        
    public:
        bool hasReturnStatements() const { return found_; }
        
        void visit(ReturnStatement& node) override {
            found_ = true;
        }
        
        // Default implementations for all other node types (no-op)
        void visit(NumericLiteral& node) override {}
        void visit(StringLiteral& node) override {}
        void visit(BooleanLiteral& node) override {}
        void visit(NullLiteral& node) override {}
        void visit(Identifier& node) override {}
        void visit(BinaryExpression& node) override {
            node.getLeft()->accept(*this);
            if (!found_) node.getRight()->accept(*this);
        }
        void visit(UnaryExpression& node) override {
            node.getOperand()->accept(*this);
        }
        void visit(AssignmentExpression& node) override {
            node.getLeft()->accept(*this);
            if (!found_) node.getRight()->accept(*this);
        }
        void visit(CallExpression& node) override {
            node.getCallee()->accept(*this);
            if (!found_) {
                for (const auto& arg : node.getArguments()) {
                    arg->accept(*this);
                    if (found_) break;
                }
            }
        }
        void visit(PropertyAccess& node) override {
            node.getObject()->accept(*this);
        }
        void visit(IndexExpression& node) override {
            node.getObject()->accept(*this);
            if (!found_) node.getIndex()->accept(*this);
        }
        void visit(ArrayLiteral& node) override {
            for (const auto& element : node.getElements()) {
                element->accept(*this);
                if (found_) break;
            }
        }
        void visit(ObjectLiteral& node) override {
            for (const auto& property : node.getProperties()) {
                property.getValue()->accept(*this);
                if (found_) break;
            }
        }
        void visit(ThisExpression& node) override {}
        void visit(NewExpression& node) override {
            for (const auto& arg : node.getArguments()) {
                arg->accept(*this);
                if (found_) break;
            }
        }
        void visit(SuperExpression& node) override {}
        void visit(ExpressionStatement& node) override {
            node.getExpression()->accept(*this);
        }
        void visit(BlockStatement& node) override {
            for (const auto& stmt : node.getStatements()) {
                stmt->accept(*this);
                if (found_) break;
            }
        }
        void visit(IfStatement& node) override {
            node.getCondition()->accept(*this);
            if (!found_) node.getThenStatement()->accept(*this);
            if (!found_ && node.getElseStatement()) node.getElseStatement()->accept(*this);
        }
        void visit(WhileStatement& node) override {
            node.getCondition()->accept(*this);
            if (!found_) node.getBody()->accept(*this);
        }
        void visit(DoWhileStatement& node) override {
            node.getBody()->accept(*this);
            if (!found_) node.getCondition()->accept(*this);
        }
        void visit(ForStatement& node) override {
            if (node.getInit()) node.getInit()->accept(*this);
            if (!found_ && node.getCondition()) node.getCondition()->accept(*this);
            if (!found_ && node.getIncrement()) node.getIncrement()->accept(*this);
            if (!found_) node.getBody()->accept(*this);
        }
        void visit(SwitchStatement& node) override {
            node.getDiscriminant()->accept(*this);
            if (!found_) {
                for (const auto& clause : node.getCases()) {
                    clause->accept(*this);
                    if (found_) break;
                }
            }
        }
        void visit(CaseClause& node) override {
            if (node.getTest()) node.getTest()->accept(*this);
            if (!found_) {
                for (const auto& stmt : node.getStatements()) {
                    stmt->accept(*this);
                    if (found_) break;
                }
            }
        }
        void visit(BreakStatement& node) override {}
        void visit(ContinueStatement& node) override {}
        void visit(TryStatement& node) override {
            node.getTryBlock()->accept(*this);
            if (!found_ && node.getCatchClause()) node.getCatchClause()->accept(*this);
            if (!found_ && node.getFinallyBlock()) node.getFinallyBlock()->accept(*this);
        }
        void visit(CatchClause& node) override {
            node.getBody()->accept(*this);
        }
        void visit(ThrowStatement& node) override {
            if (node.getExpression()) node.getExpression()->accept(*this);
        }
        void visit(VariableDeclaration& node) override {
            if (node.getInitializer()) node.getInitializer()->accept(*this);
        }
        void visit(FunctionDeclaration& node) override {
            // Don't traverse into nested functions
        }
        void visit(TypeParameter& node) override {}
        void visit(ClassDeclaration& node) override {}
        void visit(PropertyDeclaration& node) override {}
        void visit(MethodDeclaration& node) override {}
        void visit(InterfaceDeclaration& node) override {}
        void visit(EnumDeclaration& node) override {}
        void visit(EnumMember& node) override {}
        void visit(TypeAliasDeclaration& node) override {}
        void visit(ArrowFunction& node) override {}
        void visit(Module& node) override {}
    };
    
    ReturnStatementChecker checker;
    if (funcDecl.getBody()) {
        funcDecl.getBody()->accept(checker);
    }
    return checker.hasReturnStatements();
}

// Memory management implementation
llvm::Value* LLVMCodeGen::allocateVariable(const String& name, llvm::Type* type, const SourceLocation& location) {
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (!currentFunc) {
        // Global variable
        llvm::GlobalVariable* global = new llvm::GlobalVariable(
            *module_, type, false, llvm::GlobalValue::PrivateLinkage,
            llvm::Constant::getNullValue(type), name);
        codeGenContext_->setSymbolValue(name, global);
        return global;
    } else {
        // Local variable - allocate on stack
        llvm::IRBuilder<> allocaBuilder(&currentFunc->getEntryBlock(), 
                                       currentFunc->getEntryBlock().begin());
        llvm::AllocaInst* alloca = allocaBuilder.CreateAlloca(type, nullptr, name);
        codeGenContext_->setSymbolValue(name, alloca);
        return alloca;
    }
}

llvm::Value* LLVMCodeGen::loadVariable(const String& name, const SourceLocation& location) {
    llvm::Value* storage = codeGenContext_->getSymbolValue(name);
    if (!storage) {
        return nullptr;
    }
    
    // Check if we're in a function context
    llvm::Function* currentFunc = codeGenContext_->getCurrentFunction();
    if (currentFunc) {
        // We're in a function - can use CreateLoad
        llvm::Type* elementType = getAnyType(); // Default fallback
        
        // Try to get the correct element type from the alloca instruction
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(storage)) {
            elementType = allocaInst->getAllocatedType();
        }
        
        return builder_->CreateLoad(elementType, storage, name + "_val");
    } else {
        // We're at global scope - can only reference constants
        if (auto* globalVar = llvm::dyn_cast<llvm::GlobalVariable>(storage)) {
            if (globalVar->hasInitializer()) {
                return globalVar->getInitializer();
            }
        }
        // Can't load non-constant values at global scope
        reportError("Cannot reference non-constant values in global initializers", location);
        return nullptr;
    }
}

void LLVMCodeGen::storeVariable(const String& name, llvm::Value* value, const SourceLocation& location) {
    llvm::Value* storage = codeGenContext_->getSymbolValue(name);
    if (!storage) {
        reportError("Undefined variable: " + name, location);
        return;
    }
    
    builder_->CreateStore(value, storage);
}

// Built-in functions implementation
void LLVMCodeGen::declareBuiltinFunctions() {
    getOrCreateStringConcatFunction();
}

llvm::Function* LLVMCodeGen::getOrCreateStringConcatFunction() {
    if (auto existing = module_->getFunction("string_concat")) {
        return existing;
    }
    
    llvm::FunctionType* concatType = llvm::FunctionType::get(
        getStringType(), {getStringType(), getStringType()}, false);
    return llvm::Function::Create(concatType, llvm::Function::ExternalLinkage, "string_concat", module_.get());
}

// Optimization implementation
void LLVMCodeGen::optimizeModule() {
    if (options_.optimizationLevel == CompilerOptions::OptLevel::O0) {
        return; // No optimization
    }
    
    // Basic optimization for now
    // Full optimization passes would be added here
}

// Target setup implementation
bool LLVMCodeGen::setupTargetMachine() {
    String targetTriple = getTargetTriple();
    module_->setTargetTriple(targetTriple);
    
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    
    if (!target) {
        reportError("Failed to lookup target: " + error, SourceLocation());
        return false;
    }
    
    llvm::TargetOptions targetOptions;
    targetMachine_ = std::unique_ptr<llvm::TargetMachine>(
        target->createTargetMachine(targetTriple, "generic", "", targetOptions,
                                   llvm::Reloc::PIC_));
    
    if (!targetMachine_) {
        reportError("Failed to create target machine", SourceLocation());
        return false;
    }
    
    module_->setDataLayout(targetMachine_->createDataLayout());
    return true;
}

String LLVMCodeGen::getTargetTriple() const {
    if (!options_.target.triple.empty()) {
        return options_.target.triple;
    }
    // Default target triple for x86_64 Linux
    return "x86_64-pc-linux-gnu";
}

// Error handling implementation
void LLVMCodeGen::reportError(const String& message, const SourceLocation& location) {
    codeGenContext_->reportError(message, location);
}

void LLVMCodeGen::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
}

// Class-related visitor implementations
void LLVMCodeGen::visit(PropertyDeclaration& node) {
    // For now, properties are handled as part of class layout
    // Individual property declarations don't generate standalone code
    // They are processed when the containing class is processed
    
    // If there's an initializer, we could generate code for it here
    // but for simplicity, we'll handle initialization in constructors
    if (node.getInitializer()) {
        node.getInitializer()->accept(*this);
        // Store the result for later use in constructor generation
    }
}

void LLVMCodeGen::visit(MethodDeclaration& node) {
    // Generate LLVM function for the method
    std::vector<llvm::Type*> paramTypes;
    
    // Add 'this' pointer as first parameter for non-static methods
    if (!node.isStatic()) {
        paramTypes.push_back(getAnyType()); // Simplified: use generic pointer for 'this'
    }
    
    // Add method parameters
    for (const auto& param : node.getParameters()) {
        llvm::Type* paramType = getAnyType(); // Simplified: use generic type
        if (param.type) {
            paramType = convertTypeToLLVM(param.type);
        }
        paramTypes.push_back(paramType);
    }
    
    // Determine return type
    llvm::Type* returnType = getVoidType(); // Default to void
    if (node.getReturnType()) {
        returnType = convertTypeToLLVM(node.getReturnType());
    }
    
    // Create function type
    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, paramTypes, false);
    
    // Create function with mangled name (simplified: just use method name for now)
    String functionName = node.getName();
    if (node.getName() == "constructor") {
        functionName = "constructor"; // Special handling for constructors
    }
    
    llvm::Function* function = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, functionName, module_.get()
    );
    
    // Generate function body if present
    if (node.getBody()) {
        llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context_, "entry", function);
        builder_->SetInsertPoint(entryBlock);
        
        // Save current function context
        codeGenContext_->enterFunction(function);
        
        // Set up parameters
        auto paramIt = function->arg_begin();
        if (!node.isStatic()) {
            // Skip 'this' parameter for now
            ++paramIt;
        }
        
        for (size_t i = 0; i < node.getParameters().size(); ++i, ++paramIt) {
            const auto& param = node.getParameters()[i];
            llvm::Type* paramType = paramTypes[node.isStatic() ? i : i + 1];
            llvm::Value* paramStorage = allocateVariable(param.name, paramType, node.getLocation());
            builder_->CreateStore(&*paramIt, paramStorage);
        }
        
        // Generate method body
        node.getBody()->accept(*this);
        
        // Ensure function has a return
        if (!builder_->GetInsertBlock()->getTerminator()) {
            if (returnType->isVoidTy()) {
                builder_->CreateRetVoid();
            } else {
                builder_->CreateRet(createDefaultValue(returnType));
            }
        }
        
        // Restore previous function context
        codeGenContext_->exitFunction();
    }
    
    setCurrentValue(function);
}

void LLVMCodeGen::visit(ClassDeclaration& node) {
    // For now, implement classes as simple structs
    // In a full implementation, we'd need vtables for virtual methods
    
    std::vector<llvm::Type*> memberTypes;
    
    // Add properties to struct layout
    for (const auto& property : node.getProperties()) {
        llvm::Type* propertyType = getAnyType(); // Simplified: use generic type
        if (property->getType()) {
            propertyType = convertTypeToLLVM(property->getType());
        }
        memberTypes.push_back(propertyType);
    }
    
    // Create struct type for the class
    llvm::StructType* classStruct = llvm::StructType::create(*context_, memberTypes, node.getName());
    
    // Generate constructor if present
    if (node.getConstructor()) {
        node.getConstructor()->accept(*this);
    }
    
    // Generate methods
    for (const auto& method : node.getMethods()) {
        method->accept(*this);
    }
    
    // Store class type information (simplified)
    // In a full implementation, we'd store this in a class registry
    setCurrentValue(llvm::Constant::getNullValue(llvm::PointerType::get(classStruct, 0)));
}

void LLVMCodeGen::visit(InterfaceDeclaration& node) {
    // Interfaces don't generate direct code in LLVM
    // They are used for type checking during semantic analysis
    // The actual implementation will be in classes that implement the interface
    
    // For now, we'll just ignore interfaces in code generation
    // In a full implementation, we might:
    // 1. Generate type information for runtime type checking
    // 2. Create vtable layouts for interface methods
    // 3. Generate interface dispatch code
    
    // No-op for now
}

void LLVMCodeGen::visit(EnumMember& node) {
    // Enum members don't generate code directly
    // They are handled as part of the containing enum
    // No-op for now
}

void LLVMCodeGen::visit(EnumDeclaration& node) {
    // Generate constants for enum members
    // In TypeScript/JavaScript, enums are typically compiled to objects or constants
    
    int currentValue = 0;
    
    for (const auto& member : node.getMembers()) {
        llvm::Value* memberValue = nullptr;
        
        if (member->hasValue()) {
            // Member has explicit value - evaluate it
            member->getValue()->accept(*this);
            memberValue = getCurrentValue();
        } else {
            // Auto-increment numeric value
            memberValue = llvm::ConstantFP::get(getNumberType(), static_cast<double>(currentValue));
        }
        
        // Create a global constant for the enum member
        String globalName = node.getName() + "_" + member->getName();
        
        // Ensure we have a constant value
        llvm::Constant* constantValue = nullptr;
        if (llvm::Constant* constVal = llvm::dyn_cast<llvm::Constant>(memberValue)) {
            constantValue = constVal;
        } else {
            // Fallback to the current auto-increment value
            constantValue = llvm::ConstantFP::get(getNumberType(), static_cast<double>(currentValue));
        }
        
        llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
            *module_,
            getNumberType(),
            true, // isConstant
            llvm::GlobalValue::ExternalLinkage,
            constantValue,
            globalName
        );
        
        // Store the global variable for later reference
        // In a full implementation, we'd have a more sophisticated enum value tracking system
        
        currentValue++;
    }
    
    // Enum declarations don't produce a value themselves
    setCurrentValue(llvm::Constant::getNullValue(getAnyType()));
}

void LLVMCodeGen::visit(TypeAliasDeclaration& node) {
    // Type aliases don't generate direct code in LLVM
    // They are used during semantic analysis for type checking
    // The aliased type information is stored in the type system
    
    // For now, we'll just ignore type aliases in code generation
    // In a full implementation, we might:
    // 1. Generate type information for runtime type checking
    // 2. Create debug metadata for better debugging support
    
    // No-op for now
    setCurrentValue(llvm::Constant::getNullValue(getAnyType()));
}

// Factory function
unique_ptr<LLVMCodeGen> createLLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options) {
    return std::make_unique<LLVMCodeGen>(diagnostics, options);
}

} // namespace tsc
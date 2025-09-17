#include "tsc/codegen/functions/FunctionGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/ast/declarations/DeclarationNodes.h"

namespace tsc {
namespace codegen {

FunctionGenerator::FunctionGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen) {
}

llvm::Function* FunctionGenerator::generateFunctionDeclaration(const ast::declarations::FunctionDeclaration& funcDecl) {
    // Create parameter types
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : funcDecl.getParameters()) {
        if (param.type) {
            // Use the explicit parameter type if available
            paramTypes.push_back(codeGen_->getTypeGenerator()->mapTypeScriptTypeToLLVM(*param.type));
        } else {
            // Default to 'any' type if no explicit type
            paramTypes.push_back(codeGen_->getTypeGenerator()->getAnyType());
        }
    }
    
    // Determine return type from function declaration
    llvm::Type* returnType = codeGen_->getTypeGenerator()->getVoidType(); // Default to void
    if (funcDecl.getReturnType()) {
        returnType = codeGen_->getTypeGenerator()->mapTypeScriptTypeToLLVM(*funcDecl.getReturnType());
    } else {
        // If no explicit return type, analyze function body to infer return type
        if (hasReturnStatements(funcDecl)) {
            // Check if any return statements have values
            if (hasReturnStatementsWithValues(funcDecl)) {
                // Function has return statements with values, so it returns a value
                returnType = codeGen_->getTypeGenerator()->getAnyType();
            } else {
                // Function has return statements but no values, so it's void
                returnType = codeGen_->getTypeGenerator()->getVoidType();
            }
        } else {
            // Function has no return statements, so it's likely void
            returnType = codeGen_->getTypeGenerator()->getVoidType();
        }
    }
    
    // Special case: main function should return int, not double
    if (funcDecl.getName() == "main") {
        returnType = llvm::Type::getInt32Ty(*codeGen_->getLLVMContext());
    }
    
    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, paramTypes, false);
    
    // Create function with appropriate linkage
    // Main function needs external linkage so the linker can find it
    llvm::Function::LinkageTypes linkage = (funcDecl.getName() == "main")
                                              ? llvm::Function::ExternalLinkage
                                              : llvm::Function::InternalLinkage;
    
    llvm::Function* function = llvm::Function::Create(
        functionType, linkage, funcDecl.getName(), codeGen_->getModule());
    
    // Debug: Check basic blocks immediately after function creation
    std::cout << "DEBUG: Basic blocks after function creation:" << std::endl;
    for (auto& block : *function) {
        std::cout << "DEBUG: Block " << &block << " has terminator: " << (block.getTerminator() ? "YES" : "NO") <<
                std::endl;
    }
    
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

void FunctionGenerator::generateFunctionBody(llvm::Function* function, const ast::declarations::FunctionDeclaration& funcDecl) {
    // Create entry block
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "entry", function);
    codeGen_->getBuilder()->SetInsertPoint(entry);
    
    // Enter function context
    codeGen_->getCodeGenContext()->enterFunction(function);
    codeGen_->getCodeGenContext()->enterScope();
    
    // Navigate to the function scope for proper symbol lookup
    Scope* functionScope = codeGen_->getSymbolTable()->findScopeByName(funcDecl.getName());
    if (functionScope) {
        codeGen_->getSymbolTable()->enterScope(functionScope);
    }
    
    // Store parameters in symbol table for access within function body
    auto paramIt = funcDecl.getParameters().begin();
    for (auto& arg : function->args()) {
        if (paramIt != funcDecl.getParameters().end()) {
            // Create alloca for parameter
            llvm::AllocaInst* paramAlloca = codeGen_->getBuilder()->CreateAlloca(
                arg.getType(), nullptr, paramIt->name);
            
            // Store parameter value
            codeGen_->getBuilder()->CreateStore(&arg, paramAlloca);
            
            // Add to symbol table
            codeGen_->getCodeGenContext()->setSymbolValue(paramIt->name, paramAlloca);
            
            ++paramIt;
        }
    }
    
    // Generate function body
    if (funcDecl.getBody()) {
        std::cout << "DEBUG: Processing function body in generateFunctionBody" << std::endl;
        codeGen_->generateStatement(*funcDecl.getBody());
        std::cout << "DEBUG: Finished processing function body in generateFunctionBody" << std::endl;
    }
    
    // Add return statement if function doesn't end with one
    llvm::BasicBlock* currentBlock = codeGen_->getBuilder()->GetInsertBlock();
    if (currentBlock && !currentBlock->getTerminator()) {
        if (function->getReturnType()->isVoidTy()) {
            codeGen_->getBuilder()->CreateRetVoid();
        } else {
            // Create a default return value
            llvm::Value* defaultValue = createDefaultReturnValue(function->getReturnType());
            codeGen_->getBuilder()->CreateRet(defaultValue);
        }
    }
    
    std::cout << "DEBUG: About to check for terminator in generateFunctionBody" << std::endl;
    
    // Ensure all blocks have terminators
    ensureBlockTerminators(function);
    
    // Exit function context
    codeGen_->getCodeGenContext()->exitScope();
    codeGen_->getCodeGenContext()->exitFunction();
    
    // Exit function scope
    if (functionScope) {
        codeGen_->getSymbolTable()->exitScope();
    }
}

llvm::Value* FunctionGenerator::generateCallExpression(const ast::expressions::CallExpression& callExpr) {
    // Generate callee expression
    llvm::Value* callee = codeGen_->generateExpression(*callExpr.getCallee());
    
    if (!callee) {
        codeGen_->reportError("Failed to generate callee for function call", callExpr.getLocation());
        return nullptr;
    }
    
    // Handle different types of callees
    if (llvm::Function* function = llvm::dyn_cast<llvm::Function>(callee)) {
        return generateDirectFunctionCall(function, callExpr);
    } else {
        return generateIndirectFunctionCall(callee, callExpr);
    }
}

llvm::Value* FunctionGenerator::generateDirectFunctionCall(llvm::Function* function, const ast::expressions::CallExpression& callExpr) {
    // Generate arguments
    std::vector<llvm::Value*> args;
    for (const auto& argExpr : callExpr.getArguments()) {
        llvm::Value* argValue = codeGen_->generateExpression(*argExpr);
        if (!argValue) {
            codeGen_->reportError("Failed to generate argument for function call", argExpr->getLocation());
            return nullptr;
        }
        args.push_back(argValue);
    }
    
    // Check argument count
    if (args.size() != function->arg_size()) {
        codeGen_->reportError("Argument count mismatch in function call", callExpr.getLocation());
        return nullptr;
    }
    
    // Perform type conversions if necessary
    auto paramIt = function->arg_begin();
    for (size_t i = 0; i < args.size(); ++i, ++paramIt) {
        llvm::Type* expectedType = paramIt->getType();
        if (args[i]->getType() != expectedType) {
            args[i] = codeGen_->getTypeGenerator()->convertValue(args[i], args[i]->getType(), expectedType);
        }
    }
    
    // Generate the call
    return codeGen_->getBuilder()->CreateCall(function, args, "call");
}

llvm::Value* FunctionGenerator::generateIndirectFunctionCall(llvm::Value* callee, const ast::expressions::CallExpression& callExpr) {
    // For indirect calls, we need to handle function pointers
    // This is a simplified implementation
    
    // Generate arguments
    std::vector<llvm::Value*> args;
    for (const auto& argExpr : callExpr.getArguments()) {
        llvm::Value* argValue = codeGen_->generateExpression(*argExpr);
        if (!argValue) {
            codeGen_->reportError("Failed to generate argument for indirect function call", argExpr->getLocation());
            return nullptr;
        }
        args.push_back(argValue);
    }
    
    // For now, assume callee is a function pointer
    if (callee->getType()->isPointerTy()) {
        llvm::Type* pointeeType = callee->getType()->getPointerElementType();
        if (llvm::FunctionType* funcType = llvm::dyn_cast<llvm::FunctionType>(pointeeType)) {
            return codeGen_->getBuilder()->CreateCall(funcType, callee, args, "indirect_call");
        }
    }
    
    codeGen_->reportError("Invalid callee type for function call", callExpr.getLocation());
    return nullptr;
}

llvm::Function* FunctionGenerator::generateArrowFunction(const ast::expressions::ArrowFunction& arrowFunc) {
    // Generate a unique name for the arrow function
    static int arrowFuncCounter = 0;
    String funcName = "__arrow_func_" + std::to_string(arrowFuncCounter++);
    
    // Create parameter types
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : arrowFunc.getParameters()) {
        if (param.type) {
            paramTypes.push_back(codeGen_->getTypeGenerator()->mapTypeScriptTypeToLLVM(*param.type));
        } else {
            paramTypes.push_back(codeGen_->getTypeGenerator()->getAnyType());
        }
    }
    
    // Determine return type
    llvm::Type* returnType = codeGen_->getTypeGenerator()->getAnyType(); // Default to any
    if (arrowFunc.getReturnType()) {
        returnType = codeGen_->getTypeGenerator()->mapTypeScriptTypeToLLVM(*arrowFunc.getReturnType());
    }
    
    // Create function type and function
    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, paramTypes, false);
    llvm::Function* function = llvm::Function::Create(
        functionType, llvm::Function::InternalLinkage, funcName, codeGen_->getModule());
    
    // Set parameter names
    auto paramIt = arrowFunc.getParameters().begin();
    for (auto& arg : function->args()) {
        if (paramIt != arrowFunc.getParameters().end()) {
            arg.setName(paramIt->name);
            ++paramIt;
        }
    }
    
    // Generate function body
    generateArrowFunctionBody(function, arrowFunc);
    
    return function;
}

void FunctionGenerator::generateArrowFunctionBody(llvm::Function* function, const ast::expressions::ArrowFunction& arrowFunc) {
    // Create entry block
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "entry", function);
    codeGen_->getBuilder()->SetInsertPoint(entry);
    
    // Enter function context
    codeGen_->getCodeGenContext()->enterFunction(function);
    codeGen_->getCodeGenContext()->enterScope();
    
    // Store parameters in symbol table
    auto paramIt = arrowFunc.getParameters().begin();
    for (auto& arg : function->args()) {
        if (paramIt != arrowFunc.getParameters().end()) {
            // Create alloca for parameter
            llvm::AllocaInst* paramAlloca = codeGen_->getBuilder()->CreateAlloca(
                arg.getType(), nullptr, paramIt->name);
            
            // Store parameter value
            codeGen_->getBuilder()->CreateStore(&arg, paramAlloca);
            
            // Add to symbol table
            codeGen_->getCodeGenContext()->setSymbolValue(paramIt->name, paramAlloca);
            
            ++paramIt;
        }
    }
    
    // Generate body
    if (arrowFunc.getBody()) {
        if (arrowFunc.isExpressionBody()) {
            // Expression body - return the expression result
            llvm::Value* result = codeGen_->generateExpression(*arrowFunc.getBody());
            if (result) {
                // Convert result to return type if necessary
                if (result->getType() != function->getReturnType()) {
                    result = codeGen_->getTypeGenerator()->convertValue(result, result->getType(), function->getReturnType());
                }
                codeGen_->getBuilder()->CreateRet(result);
            } else {
                // Return default value if expression generation failed
                llvm::Value* defaultValue = createDefaultReturnValue(function->getReturnType());
                codeGen_->getBuilder()->CreateRet(defaultValue);
            }
        } else {
            // Statement body
            codeGen_->generateStatement(*arrowFunc.getBody());
            
            // Add return if needed
            llvm::BasicBlock* currentBlock = codeGen_->getBuilder()->GetInsertBlock();
            if (currentBlock && !currentBlock->getTerminator()) {
                if (function->getReturnType()->isVoidTy()) {
                    codeGen_->getBuilder()->CreateRetVoid();
                } else {
                    llvm::Value* defaultValue = createDefaultReturnValue(function->getReturnType());
                    codeGen_->getBuilder()->CreateRet(defaultValue);
                }
            }
        }
    }
    
    // Ensure all blocks have terminators
    ensureBlockTerminators(function);
    
    // Exit function context
    codeGen_->getCodeGenContext()->exitScope();
    codeGen_->getCodeGenContext()->exitFunction();
}

bool FunctionGenerator::hasReturnStatements(const ast::declarations::FunctionDeclaration& funcDecl) const {
    // This is a simplified implementation - would need a proper AST visitor
    // For now, assume functions have return statements if they have a body
    return funcDecl.getBody() != nullptr;
}

bool FunctionGenerator::hasReturnStatementsWithValues(const ast::declarations::FunctionDeclaration& funcDecl) const {
    // This is a simplified implementation
    // In a full implementation, we would traverse the AST to find return statements
    return funcDecl.getReturnType() != nullptr;
}

llvm::Value* FunctionGenerator::createDefaultReturnValue(llvm::Type* returnType) {
    if (returnType->isVoidTy()) {
        return nullptr; // Void functions don't return values
    } else if (returnType->isIntegerTy()) {
        return llvm::ConstantInt::get(returnType, 0);
    } else if (returnType->isFloatingPointTy()) {
        return llvm::ConstantFP::get(returnType, 0.0);
    } else if (returnType->isPointerTy()) {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(returnType));
    } else {
        // For other types, create an undef value
        return llvm::UndefValue::get(returnType);
    }
}

void FunctionGenerator::ensureBlockTerminators(llvm::Function* function) {
    for (auto& block : *function) {
        if (!block.getTerminator()) {
            // Add unreachable terminator to blocks without terminators
            llvm::IRBuilder<> builder(&block);
            if (function->getReturnType()->isVoidTy()) {
                builder.CreateRetVoid();
            } else {
                llvm::Value* defaultValue = createDefaultReturnValue(function->getReturnType());
                builder.CreateRet(defaultValue);
            }
        }
    }
}

} // namespace codegen
} // namespace tsc
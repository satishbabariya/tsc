#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Class generation utilities for converting class declarations to LLVM IR
class ClassGenerator {
public:
    explicit ClassGenerator(LLVMCodeGen* codeGen);
    
    // Class declaration generation
    void generateClassDeclaration(const ClassDeclaration& node);
    void generatePropertyDeclaration(const PropertyDeclaration& node);
    void generateMethodDeclaration(const MethodDeclaration& node);
    void generateDestructorDeclaration(const DestructorDeclaration& node);
    
    // Interface and enum generation
    void generateInterfaceDeclaration(const InterfaceDeclaration& node);
    void generateEnumDeclaration(const EnumDeclaration& node);
    void generateEnumMember(const EnumMember& node);
    void generateTypeAliasDeclaration(const TypeAliasDeclaration& node);
    
    // Generic method lookup for constrained types
    llvm::Function* genericMethodLookup(const String& methodName, shared_ptr<Type> objectType, const SourceLocation& location);
    llvm::Function* createBuiltinMethodFunction(const String& methodName, shared_ptr<Type> objectType, const SourceLocation& location);
    
    // Generic type monomorphization
    void generateMonomorphizedMethods(const GenericType& genericType, Symbol* classSymbol);
    void generateMonomorphizedMethod(const MethodDeclaration& method, const GenericType& genericType, const String& mangledName);

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc
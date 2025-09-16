#pragma once

#include "tsc/Common.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Allocator.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Memory management utilities for LLVM IR generation
class MemoryManager {
public:
    explicit MemoryManager(LLVMCodeGen* codeGen);
    
    // Memory allocation
    llvm::Value* allocateVariable(const String& name, llvm::Type* type, const SourceLocation& location);
    llvm::Value* loadVariable(const String& name, const SourceLocation& location);
    void storeVariable(const String& name, llvm::Value* value, const SourceLocation& location);
    
    // ARC helper functions
    bool isARCManagedType(shared_ptr<Type> type) const;
    void generateAutomaticCleanup(const String& className);
    
    // Memory usage reporting
    void reportMemoryUsage() const;
    
    // IR Allocator for efficient memory management during code generation
    class IRAllocator {
    private:
        llvm::BumpPtrAllocator arena_;
        size_t totalAllocated_ = 0;
        size_t allocationCount_ = 0;
        
    public:
        template<typename T, typename... Args>
        T* allocate(Args&&... args) {
            void* ptr = arena_.Allocate(sizeof(T), alignof(T));
            if (!ptr) {
                throw std::bad_alloc();
            }
            
            totalAllocated_ += sizeof(T);
            allocationCount_++;
            
            return new(ptr) T(std::forward<Args>(args)...);
        }
        
        template<typename T>
        void deallocate(T* ptr) {
            if (ptr) {
                ptr->~T();
                // Note: BumpPtrAllocator doesn't support individual deallocation
                // Memory is freed when the allocator is destroyed
            }
        }
        
        void reset() {
            arena_.Reset();
            totalAllocated_ = 0;
            allocationCount_ = 0;
        }
        
        size_t getTotalAllocated() const { return totalAllocated_; }
        size_t getAllocationCount() const { return allocationCount_; }
        
        // Get memory statistics
        struct MemoryStats {
            size_t totalAllocated;
            size_t allocationCount;
            double averageAllocationSize;
            size_t peakMemoryUsage;
        };
        
        MemoryStats getStats() const {
            MemoryStats stats;
            stats.totalAllocated = totalAllocated_;
            stats.allocationCount = allocationCount_;
            stats.averageAllocationSize = allocationCount_ > 0 ? 
                static_cast<double>(totalAllocated_) / allocationCount_ : 0.0;
            stats.peakMemoryUsage = arena_.getBytesAllocated();
            return stats;
        }
        
        // Memory usage reporting
        void reportMemoryUsage() const;
    };
    
    // Memory management
    void reportMemoryUsage() const;
    IRAllocator::MemoryStats getMemoryStats() const;

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc
#pragma once

#include <llvm/Support/Allocator.h>
#include <memory>

namespace tsc::utils {

// AST allocator using LLVM's BumpPtrAllocator for efficient memory management
class ASTAllocator {
private:
    llvm::BumpPtrAllocator allocator_;
    
public:
    // Allocate memory for AST nodes
    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        return new (allocator_.Allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
    }
    
    // Allocate array of objects
    template<typename T>
    T* allocateArray(size_t count) {
        return static_cast<T*>(allocator_.Allocate(sizeof(T) * count, alignof(T)));
    }
    
    // Allocate raw memory
    void* allocateBytes(size_t size, size_t alignment = 8) {
        return allocator_.Allocate(size, alignment);
    }
    
    // Reset the allocator (frees all memory)
    void reset() {
        allocator_.Reset();
    }
    
    // Get total allocated size
    size_t getTotalAllocated() const {
        return allocator_.getTotalMemory();
    }
    
    // Get bytes used
    size_t getBytesUsed() const {
        return allocator_.getBytesAllocated();
    }
};

// RAII wrapper for AST allocator scopes
class ASTAllocatorScope {
private:
    ASTAllocator* allocator_;
    size_t initialBytesUsed_;
    
public:
    explicit ASTAllocatorScope(ASTAllocator& allocator) 
        : allocator_(&allocator), initialBytesUsed_(allocator.getBytesUsed()) {}
    
    ~ASTAllocatorScope() {
        // Note: We don't reset here as the allocator is shared
        // Individual scopes can track their usage for debugging
    }
    
    // Get bytes allocated in this scope
    size_t getBytesAllocatedInScope() const {
        return allocator_->getBytesUsed() - initialBytesUsed_;
    }
    
    // Prevent copying
    ASTAllocatorScope(const ASTAllocatorScope&) = delete;
    ASTAllocatorScope& operator=(const ASTAllocatorScope&) = delete;
};

} // namespace tsc::utils
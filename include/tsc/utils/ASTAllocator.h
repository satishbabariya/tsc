#pragma once

#include "tsc/Common.h"
#include <llvm/Support/Allocator.h>

namespace tsc {

class ASTAllocator {
public:
    ASTAllocator() = default;
    ~ASTAllocator() = default;
    
    // Non-copyable
    ASTAllocator(const ASTAllocator&) = delete;
    ASTAllocator& operator=(const ASTAllocator&) = delete;
    
    // Allocate AST nodes
    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        void* ptr = arena_.allocate(sizeof(T), alignof(T));
        return new(ptr) T(std::forward<Args>(args)...);
    }
    
    // Allocate arrays
    template<typename T>
    T* allocateArray(size_t count) {
        return static_cast<T*>(arena_.allocate(sizeof(T) * count, alignof(T)));
    }
    
    // Allocate strings
    String allocateString(const String& str) {
        char* ptr = static_cast<char*>(arena_.allocate(str.length() + 1, 1));
        std::strcpy(ptr, str.c_str());
        return String(ptr);
    }
    
    // Get memory usage statistics
    size_t getTotalBytesAllocated() const {
        return arena_.getBytesAllocated();
    }
    
    // Reset allocator (for reuse)
    void reset() {
        arena_.Reset();
    }

private:
    llvm::BumpPtrAllocator arena_;
};

// RAII wrapper for AST allocator
class ASTAllocatorScope {
public:
    explicit ASTAllocatorScope(ASTAllocator& allocator) : allocator_(allocator) {}
    ~ASTAllocatorScope() = default;
    
    ASTAllocator& getAllocator() { return allocator_; }

private:
    ASTAllocator& allocator_;
};

} // namespace tsc
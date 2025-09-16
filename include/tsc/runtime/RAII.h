#ifndef TSC_RAII_H
#define TSC_RAII_H

#include <memory>
#include <functional>
#include <utility>
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <iostream>

// Forward declarations for C runtime functions
extern "C" {
    void* __tsc_alloc(size_t size, void (*destructor)(void*), void* type_info);
    void __tsc_dealloc(void* obj);
    void* __tsc_retain(void* obj);
    void __tsc_release(void* obj);
    void __tsc_report_memory_leaks(void);
    void __tsc_cleanup_malloc_tracker(void);
}

namespace tsc {
namespace runtime {

// RAII wrapper for unique ownership
template<typename T>
class UniquePtr {
private:
    T* ptr_;
    std::function<void(T*)> deleter_;

public:
    // Constructors
    explicit UniquePtr(T* ptr = nullptr, std::function<void(T*)> deleter = nullptr) 
        : ptr_(ptr), deleter_(deleter ? deleter : [](T* p) { delete p; }) {}
    
    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept 
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }
    
    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    // Delete copy constructor and assignment
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    // Destructor
    ~UniquePtr() {
        reset();
    }
    
    // Access operators
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    // Reset
    void reset(T* new_ptr = nullptr) {
        if (ptr_) {
            deleter_(ptr_);
        }
        ptr_ = new_ptr;
    }
    
    // Release ownership
    T* release() {
        T* old_ptr = ptr_;
        ptr_ = nullptr;
        return old_ptr;
    }
    
    // Check if valid
    explicit operator bool() const { return ptr_ != nullptr; }
    
    // Swap
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }
};

// RAII wrapper for shared ownership with reference counting
template<typename T>
class SharedPtr {
private:
    T* ptr_;
    std::atomic<size_t>* ref_count_;
    std::function<void(T*)> deleter_;
    
    void increment_ref() {
        if (ref_count_) {
            ref_count_->fetch_add(1);
        }
    }
    
    void decrement_ref() {
        if (ref_count_) {
            if (ref_count_->fetch_sub(1) == 1) {
                if (ptr_) {
                    deleter_(ptr_);
                }
                delete ref_count_;
                ref_count_ = nullptr;
            }
        }
    }

public:
    // Constructors
    explicit SharedPtr(T* ptr = nullptr, std::function<void(T*)> deleter = nullptr)
        : ptr_(ptr), ref_count_(ptr ? new std::atomic<size_t>(1) : nullptr), 
          deleter_(deleter ? deleter : [](T* p) { delete p; }) {}
    
    // Copy constructor
    SharedPtr(const SharedPtr& other) 
        : ptr_(other.ptr_), ref_count_(other.ref_count_), deleter_(other.deleter_) {
        increment_ref();
    }
    
    // Move constructor
    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), ref_count_(other.ref_count_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }
    
    // Copy assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            decrement_ref();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            deleter_ = other.deleter_;
            increment_ref();
        }
        return *this;
    }
    
    // Move assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            decrement_ref();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }
    
    // Destructor
    ~SharedPtr() {
        decrement_ref();
    }
    
    // Access operators
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    // Reset
    void reset(T* new_ptr = nullptr) {
        decrement_ref();
        ptr_ = new_ptr;
        ref_count_ = new_ptr ? new std::atomic<size_t>(1) : nullptr;
    }
    
    // Check if valid
    explicit operator bool() const { return ptr_ != nullptr; }
    
    // Reference count
    size_t use_count() const { 
        return ref_count_ ? ref_count_->load() : 0; 
    }
    
    // Check if unique
    bool unique() const { 
        return use_count() == 1; 
    }
    
    // Swap
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count_, other.ref_count_);
        std::swap(deleter_, other.deleter_);
    }
};

// RAII wrapper for ARC-managed objects
template<typename T>
class ARCPtr {
private:
    T* ptr_;
    
public:
    // Constructors
    explicit ARCPtr(T* ptr = nullptr) : ptr_(ptr) {
        if (ptr_) {
            __tsc_retain(ptr_);
        }
    }
    
    // Copy constructor
    ARCPtr(const ARCPtr& other) : ptr_(other.ptr_) {
        if (ptr_) {
            __tsc_retain(ptr_);
        }
    }
    
    // Move constructor
    ARCPtr(ARCPtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    
    // Copy assignment
    ARCPtr& operator=(const ARCPtr& other) {
        if (this != &other) {
            if (ptr_) {
                __tsc_release(ptr_);
            }
            ptr_ = other.ptr_;
            if (ptr_) {
                __tsc_retain(ptr_);
            }
        }
        return *this;
    }
    
    // Move assignment
    ARCPtr& operator=(ARCPtr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                __tsc_release(ptr_);
            }
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    // Destructor
    ~ARCPtr() {
        if (ptr_) {
            __tsc_release(ptr_);
        }
    }
    
    // Access operators
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    // Reset
    void reset(T* new_ptr = nullptr) {
        if (ptr_) {
            __tsc_release(ptr_);
        }
        ptr_ = new_ptr;
        if (ptr_) {
            __tsc_retain(ptr_);
        }
    }
    
    // Check if valid
    explicit operator bool() const { return ptr_ != nullptr; }
    
    // Swap
    void swap(ARCPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
    }
};

// Utility functions for creating smart pointers
template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
ARCPtr<T> make_arc(T* ptr) {
    return ARCPtr<T>(ptr);
}

// RAII wrapper for malloc/free with debug tracking
class MallocPtr {
private:
    void* ptr_;
    size_t size_;
    
public:
    explicit MallocPtr(size_t size) : ptr_(malloc(size)), size_(size) {}
    
    MallocPtr(const MallocPtr&) = delete;
    MallocPtr& operator=(const MallocPtr&) = delete;
    
    MallocPtr(MallocPtr&& other) noexcept : ptr_(other.ptr_), size_(other.size_) {
        other.ptr_ = nullptr;
        other.size_ = 0;
    }
    
    MallocPtr& operator=(MallocPtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~MallocPtr() {
        reset();
    }
    
    void* get() const { return ptr_; }
    size_t size() const { return size_; }
    
    void reset() {
        if (ptr_) {
            free(ptr_);
            ptr_ = nullptr;
            size_ = 0;
        }
    }
    
    explicit operator bool() const { return ptr_ != nullptr; }
};

// Memory management utilities
class MemoryManager {
public:
    // Report memory leaks (debug builds only)
    static void reportLeaks() {
        __tsc_report_memory_leaks();
    }
    
    // Cleanup memory tracking
    static void cleanup() {
        __tsc_cleanup_malloc_tracker();
    }
    
    // Force garbage collection (if applicable)
    static void collectGarbage() {
        // For now, this is a no-op since we use manual memory management
        // In the future, this could trigger ARC cleanup
    }
};

} // namespace runtime
} // namespace tsc

#endif // TSC_RAII_H
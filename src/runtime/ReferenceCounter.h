#pragma once

#include <atomic>
#include <memory>
#include <functional>
#include <unordered_set>
#include <mutex>

namespace tsc::runtime {

/**
 * Thread-safe reference counter for automatic reference counting (ARC)
 * Manages object lifetime through reference counting
 */
class ReferenceCounter {
public:
    ReferenceCounter();
    ~ReferenceCounter();

    // Reference counting operations
    void addRef();
    void releaseRef();
    
    // Get current reference count (for debugging)
    size_t getRefCount() const;
    
    // Check if object should be deallocated
    bool shouldDeallocate() const;
    
    // Set cleanup callback for when object is deallocated
    void setCleanupCallback(std::function<void()> callback);

private:
    std::atomic<size_t> refCount_;
    std::function<void()> cleanupCallback_;
    mutable std::mutex mutex_;
};

/**
 * RAII wrapper for TypeScript objects with automatic reference counting
 * Ensures proper cleanup when object goes out of scope
 */
template<typename T>
class ArcPtr {
public:
    // Constructors
    ArcPtr() : ptr_(nullptr), refCounter_(nullptr) {}
    
    explicit ArcPtr(T* ptr) : ptr_(ptr), refCounter_(ptr ? new ReferenceCounter() : nullptr) {
        if (refCounter_) {
            refCounter_->addRef();
        }
    }
    
    // Copy constructor - increments reference count
    ArcPtr(const ArcPtr& other) : ptr_(other.ptr_), refCounter_(other.refCounter_) {
        if (refCounter_) {
            refCounter_->addRef();
        }
    }
    
    // Move constructor - transfers ownership
    ArcPtr(ArcPtr&& other) noexcept : ptr_(other.ptr_), refCounter_(other.refCounter_) {
        other.ptr_ = nullptr;
        other.refCounter_ = nullptr;
    }
    
    // Destructor - decrements reference count and deallocates if needed
    ~ArcPtr() {
        release();
    }
    
    // Assignment operators
    ArcPtr& operator=(const ArcPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            refCounter_ = other.refCounter_;
            if (refCounter_) {
                refCounter_->addRef();
            }
        }
        return *this;
    }
    
    ArcPtr& operator=(ArcPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            refCounter_ = other.refCounter_;
            other.ptr_ = nullptr;
            other.refCounter_ = nullptr;
        }
        return *this;
    }
    
    // Access operators
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    // Boolean conversion
    explicit operator bool() const { return ptr_ != nullptr; }
    
    // Reset to null
    void reset() {
        release();
        ptr_ = nullptr;
        refCounter_ = nullptr;
    }
    
    // Get reference count
    size_t getRefCount() const {
        return refCounter_ ? refCounter_->getRefCount() : 0;
    }

private:
    void release() {
        if (refCounter_) {
            refCounter_->releaseRef();
            if (refCounter_->shouldDeallocate()) {
                delete ptr_;
                delete refCounter_;
            }
        }
    }
    
    T* ptr_;
    ReferenceCounter* refCounter_;
};

/**
 * Factory function to create ArcPtr instances
 */
template<typename T, typename... Args>
ArcPtr<T> makeArc(Args&&... args) {
    return ArcPtr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Weak reference pointer that doesn't keep object alive
 * Useful for breaking reference cycles
 */
template<typename T>
class WeakPtr {
public:
    WeakPtr() : ptr_(nullptr), refCounter_(nullptr) {}
    
    explicit WeakPtr(const ArcPtr<T>& arcPtr) 
        : ptr_(arcPtr.get()), refCounter_(arcPtr.refCounter_) {}
    
    // Check if object is still alive
    bool expired() const {
        return !refCounter_ || refCounter_->getRefCount() == 0;
    }
    
    // Try to get strong reference
    ArcPtr<T> lock() const {
        if (!expired()) {
            return ArcPtr<T>(ptr_);
        }
        return ArcPtr<T>();
    }
    
    T* get() const { return ptr_; }

private:
    T* ptr_;
    ReferenceCounter* refCounter_;
};

} // namespace tsc::runtime
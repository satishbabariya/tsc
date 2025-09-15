#pragma once

#include "TypeScriptObject.h"
#include <unordered_set>
#include <vector>
#include <functional>
#include <memory>

namespace tsc::runtime {

/**
 * Garbage Collector interface for managing object lifecycle
 * Implements mark-and-sweep algorithm with ARC
 */
class GarbageCollector {
public:
    static GarbageCollector& getInstance();
    
    // Register an object for garbage collection
    void registerObject(TypeScriptObject* obj);
    
    // Unregister an object (when manually deallocated)
    void unregisterObject(TypeScriptObject* obj);
    
    // Perform garbage collection
    void collect();
    
    // Set root objects (globally reachable)
    void addRoot(TypeScriptObject* obj);
    void removeRoot(TypeScriptObject* obj);
    
    // Get statistics
    size_t getObjectCount() const { return objects_.size(); }
    size_t getRootCount() const { return roots_.size(); }
    
    // Force cleanup of all objects (for shutdown)
    void cleanup();

private:
    GarbageCollector() = default;
    ~GarbageCollector();
    
    // Mark phase - mark all reachable objects
    void markReachable();
    
    // Sweep phase - deallocate unreachable objects
    void sweep();
    
    // Helper to mark object and its references
    void markObject(TypeScriptObject* obj, std::unordered_set<TypeScriptObject*>& reachable);
    
    std::unordered_set<TypeScriptObject*> objects_;
    std::unordered_set<TypeScriptObject*> roots_;
    mutable std::mutex mutex_;
};

/**
 * RAII wrapper for automatic garbage collection
 */
class GCObject {
public:
    template<typename T, typename... Args>
    static TypeScriptObjectPtr create(Args&&... args) {
        auto obj = TypeScriptObjectPtr(new T(std::forward<Args>(args)...));
        GarbageCollector::getInstance().registerObject(obj.get());
        return obj;
    }
    
    static void destroy(TypeScriptObject* obj) {
        GarbageCollector::getInstance().unregisterObject(obj);
        delete obj;
    }
};

/**
 * Scoped root object - automatically manages root registration
 */
class ScopedRoot {
public:
    explicit ScopedRoot(TypeScriptObject* obj) : obj_(obj) {
        if (obj_) {
            GarbageCollector::getInstance().addRoot(obj_);
        }
    }
    
    ~ScopedRoot() {
        if (obj_) {
            GarbageCollector::getInstance().removeRoot(obj_);
        }
    }
    
    // Non-copyable
    ScopedRoot(const ScopedRoot&) = delete;
    ScopedRoot& operator=(const ScopedRoot&) = delete;
    
    TypeScriptObject* get() const { return obj_; }

private:
    TypeScriptObject* obj_;
};

/**
 * Utility macro for creating scoped roots
 */
#define SCOPE_ROOT(obj) ScopedRoot _root(obj)

/**
 * Memory pool for efficient object allocation
 */
template<typename T>
class ObjectPool {
public:
    ObjectPool(size_t initialSize = 100) {
        for (size_t i = 0; i < initialSize; ++i) {
            pool_.push_back(std::make_unique<T>());
        }
    }
    
    ~ObjectPool() = default;
    
    // Get object from pool
    std::unique_ptr<T> acquire() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pool_.empty()) {
            return std::make_unique<T>();
        }
        auto obj = std::move(pool_.back());
        pool_.pop_back();
        return obj;
    }
    
    // Return object to pool
    void release(std::unique_ptr<T> obj) {
        if (!obj) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        if (pool_.size() < maxSize_) {
            pool_.push_back(std::move(obj));
        }
    }
    
    void setMaxSize(size_t maxSize) { maxSize_ = maxSize; }
    size_t getPoolSize() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return pool_.size();
    }

private:
    std::vector<std::unique_ptr<T>> pool_;
    size_t maxSize_ = 1000;
    mutable std::mutex mutex_;
};

} // namespace tsc::runtime
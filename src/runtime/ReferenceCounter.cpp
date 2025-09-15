#include "ReferenceCounter.h"
#include <iostream>

namespace tsc::runtime {

ReferenceCounter::ReferenceCounter() 
    : refCount_(1), cleanupCallback_(nullptr) {
}

ReferenceCounter::~ReferenceCounter() {
    // Cleanup callback should be called before destruction
    if (cleanupCallback_) {
        cleanupCallback_();
    }
}

void ReferenceCounter::addRef() {
    std::lock_guard<std::mutex> lock(mutex_);
    refCount_.fetch_add(1, std::memory_order_relaxed);
}

void ReferenceCounter::releaseRef() {
    std::lock_guard<std::mutex> lock(mutex_);
    size_t oldCount = refCount_.fetch_sub(1, std::memory_order_relaxed);
    
    if (oldCount == 1) {
        // Last reference released
        if (cleanupCallback_) {
            cleanupCallback_();
        }
    }
}

size_t ReferenceCounter::getRefCount() const {
    return refCount_.load(std::memory_order_relaxed);
}

bool ReferenceCounter::shouldDeallocate() const {
    return refCount_.load(std::memory_order_relaxed) == 0;
}

void ReferenceCounter::setCleanupCallback(std::function<void()> callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    cleanupCallback_ = std::move(callback);
}

} // namespace tsc::runtime
#include "GarbageCollector.h"
#include <iostream>
#include <algorithm>

namespace tsc::runtime {

GarbageCollector& GarbageCollector::getInstance() {
    static GarbageCollector instance;
    return instance;
}

GarbageCollector::~GarbageCollector() {
    cleanup();
}

void GarbageCollector::registerObject(TypeScriptObject* obj) {
    if (!obj) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    objects_.insert(obj);
}

void GarbageCollector::unregisterObject(TypeScriptObject* obj) {
    if (!obj) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    objects_.erase(obj);
    roots_.erase(obj);
}

void GarbageCollector::collect() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::cout << "Garbage Collection: Starting collection of " << objects_.size() << " objects" << std::endl;
    
    // Mark phase
    markReachable();
    
    // Sweep phase
    sweep();
    
    std::cout << "Garbage Collection: Completed, " << objects_.size() << " objects remaining" << std::endl;
}

void GarbageCollector::addRoot(TypeScriptObject* obj) {
    if (!obj) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    roots_.insert(obj);
}

void GarbageCollector::removeRoot(TypeScriptObject* obj) {
    if (!obj) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    roots_.erase(obj);
}

void GarbageCollector::cleanup() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Deallocate all objects
    for (auto* obj : objects_) {
        obj->cleanup();
        delete obj;
    }
    
    objects_.clear();
    roots_.clear();
}

void GarbageCollector::markReachable() {
    std::unordered_set<TypeScriptObject*> reachable;
    
    // Start from root objects
    for (auto* root : roots_) {
        markObject(root, reachable);
    }
    
    // Remove unreachable objects
    auto it = objects_.begin();
    while (it != objects_.end()) {
        if (reachable.find(*it) == reachable.end()) {
            // Object is unreachable, schedule for deletion
            (*it)->cleanup();
            delete *it;
            it = objects_.erase(it);
        } else {
            ++it;
        }
    }
}

void GarbageCollector::sweep() {
    // Mark phase already handled the sweeping
    // This method is kept for future extensions
}

void GarbageCollector::markObject(TypeScriptObject* obj, std::unordered_set<TypeScriptObject*>& reachable) {
    if (!obj || reachable.find(obj) != reachable.end()) {
        return; // Already marked or null
    }
    
    reachable.insert(obj);
    
    // Mark all objects referenced by this object
    obj->markReachable(reachable);
}

} // namespace tsc::runtime
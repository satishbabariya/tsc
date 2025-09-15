#pragma once

#include "ReferenceCounter.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <any>

namespace tsc::runtime {

/**
 * Base class for all TypeScript objects with ARC support
 */
class TypeScriptObject {
public:
    TypeScriptObject() = default;
    virtual ~TypeScriptObject() = default;
    
            // Pure virtual methods for object behavior
            virtual std::string getTypeName() const = 0;
            virtual void cleanup() {}
    
    // Get reference count for debugging
    size_t getRefCount() const { return refCount_.getRefCount(); }
    
    // Add/remove references
    void addRef() { refCount_.addRef(); }
    void releaseRef() { refCount_.releaseRef(); }
    
    // Check if should deallocate
    bool shouldDeallocate() const { return refCount_.shouldDeallocate(); }

protected:
    ReferenceCounter refCount_;
};

/**
 * RAII wrapper for TypeScript objects
 */
class TypeScriptObjectPtr {
public:
    TypeScriptObjectPtr() : obj_(nullptr) {}
    
    explicit TypeScriptObjectPtr(TypeScriptObject* obj) : obj_(obj) {
        if (obj_) {
            obj_->addRef();
        }
    }
    
    TypeScriptObjectPtr(const TypeScriptObjectPtr& other) : obj_(other.obj_) {
        if (obj_) {
            obj_->addRef();
        }
    }
    
    TypeScriptObjectPtr(TypeScriptObjectPtr&& other) noexcept : obj_(other.obj_) {
        other.obj_ = nullptr;
    }
    
    ~TypeScriptObjectPtr() {
        release();
    }
    
    TypeScriptObjectPtr& operator=(const TypeScriptObjectPtr& other) {
        if (this != &other) {
            release();
            obj_ = other.obj_;
            if (obj_) {
                obj_->addRef();
            }
        }
        return *this;
    }
    
    TypeScriptObjectPtr& operator=(TypeScriptObjectPtr&& other) noexcept {
        if (this != &other) {
            release();
            obj_ = other.obj_;
            other.obj_ = nullptr;
        }
        return *this;
    }
    
    TypeScriptObject* get() const { return obj_; }
    TypeScriptObject* operator->() const { return obj_; }
    TypeScriptObject& operator*() const { return *obj_; }
    
    explicit operator bool() const { return obj_ != nullptr; }
    
    void reset() {
        release();
        obj_ = nullptr;
    }
    
    size_t getRefCount() const {
        return obj_ ? obj_->getRefCount() : 0;
    }

private:
    void release() {
        if (obj_) {
            obj_->releaseRef();
            if (obj_->shouldDeallocate()) {
                obj_->cleanup();
                delete obj_;
            }
        }
    }
    
    TypeScriptObject* obj_;
};

/**
 * TypeScript Array object with ARC support
 */
class TypeScriptArray : public TypeScriptObject {
public:
    TypeScriptArray() = default;
    ~TypeScriptArray() override = default;
    
    std::string getTypeName() const override { return "Array"; }
    
    
    // Array operations
    void push(const std::any& value) {
        elements_.push_back(value);
    }
    
    std::any pop() {
        if (elements_.empty()) {
            return std::any{};
        }
        auto value = elements_.back();
        elements_.pop_back();
        return value;
    }
    
    size_t length() const {
        return elements_.size();
    }
    
    std::any get(size_t index) const {
        if (index >= elements_.size()) {
            return std::any{};
        }
        return elements_[index];
    }
    
    void set(size_t index, const std::any& value) {
        if (index < elements_.size()) {
            elements_[index] = value;
        }
    }

private:
    std::vector<std::any> elements_;
};

/**
 * TypeScript Object with ARC support
 */
class TypeScriptObjectLiteral : public TypeScriptObject {
public:
    TypeScriptObjectLiteral() = default;
    ~TypeScriptObjectLiteral() override = default;
    
    std::string getTypeName() const override { return "Object"; }
    
    
    // Object operations
    void setProperty(const std::string& key, const std::any& value) {
        properties_[key] = value;
    }
    
    std::any getProperty(const std::string& key) const {
        auto it = properties_.find(key);
        if (it != properties_.end()) {
            return it->second;
        }
        return std::any{};
    }
    
    bool hasProperty(const std::string& key) const {
        return properties_.find(key) != properties_.end();
    }
    
    void deleteProperty(const std::string& key) {
        properties_.erase(key);
    }
    
    std::vector<std::string> getKeys() const {
        std::vector<std::string> keys;
        for (const auto& [key, value] : properties_) {
            keys.push_back(key);
        }
        return keys;
    }

private:
    std::unordered_map<std::string, std::any> properties_;
};

/**
 * Factory functions for creating TypeScript objects
 */
inline TypeScriptObjectPtr makeArray() {
    return TypeScriptObjectPtr(new TypeScriptArray());
}

inline TypeScriptObjectPtr makeObject() {
    return TypeScriptObjectPtr(new TypeScriptObjectLiteral());
}

} // namespace tsc::runtime
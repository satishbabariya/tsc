#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>
#include <cstdlib>
#include <cstring>

// Include our RAII wrappers
#include "tsc/runtime/RAII.h"

using namespace tsc::runtime;

// Test class for RAII testing
class TestObject {
private:
    int value_;
    bool* destroyed_;

public:
    TestObject(int value, bool* destroyed = nullptr) 
        : value_(value), destroyed_(destroyed) {
        if (destroyed_) *destroyed_ = false;
    }
    
    ~TestObject() {
        if (destroyed_) *destroyed_ = true;
    }
    
    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }
};

// Test destructor function for ARC
void test_destructor(void* obj) {
    TestObject* test_obj = static_cast<TestObject*>(obj);
    delete test_obj;
}

// Test functions
bool testUniquePtrBasic() {
    std::cout << "Testing UniquePtr basic functionality... ";
    
    bool destroyed = false;
    {
        UniquePtr<TestObject> ptr(new TestObject(42, &destroyed));
        
        if (!ptr) {
            std::cout << "FAILED: UniquePtr should be valid\n";
            return false;
        }
        
        if (ptr->getValue() != 42) {
            std::cout << "FAILED: Wrong value\n";
            return false;
        }
        
        ptr->setValue(100);
        if (ptr->getValue() != 100) {
            std::cout << "FAILED: Value not updated\n";
            return false;
        }
    }
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testUniquePtrMove() {
    std::cout << "Testing UniquePtr move semantics... ";
    
    bool destroyed = false;
    UniquePtr<TestObject> ptr1(new TestObject(42, &destroyed));
    
    if (!ptr1) {
        std::cout << "FAILED: Initial UniquePtr should be valid\n";
        return false;
    }
    
    UniquePtr<TestObject> ptr2 = std::move(ptr1);
    
    if (ptr1) {
        std::cout << "FAILED: Moved-from UniquePtr should be invalid\n";
        return false;
    }
    
    if (!ptr2) {
        std::cout << "FAILED: Moved-to UniquePtr should be valid\n";
        return false;
    }
    
    if (ptr2->getValue() != 42) {
        std::cout << "FAILED: Wrong value after move\n";
        return false;
    }
    
    ptr2.reset();
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed after reset\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testSharedPtrBasic() {
    std::cout << "Testing SharedPtr basic functionality... ";
    
    bool destroyed = false;
    SharedPtr<TestObject> ptr1(new TestObject(42, &destroyed));
    
    if (!ptr1) {
        std::cout << "FAILED: SharedPtr should be valid\n";
        return false;
    }
    
    if (ptr1->getValue() != 42) {
        std::cout << "FAILED: Wrong value\n";
        return false;
    }
    
    if (ptr1.use_count() != 1) {
        std::cout << "FAILED: Wrong reference count\n";
        return false;
    }
    
    {
        SharedPtr<TestObject> ptr2 = ptr1;
        
        if (ptr1.use_count() != 2) {
            std::cout << "FAILED: Reference count not incremented\n";
            return false;
        }
        
        if (ptr2.use_count() != 2) {
            std::cout << "FAILED: Reference count not incremented\n";
            return false;
        }
        
        if (ptr1->getValue() != ptr2->getValue()) {
            std::cout << "FAILED: Values should be equal\n";
            return false;
        }
    }
    
    if (ptr1.use_count() != 1) {
        std::cout << "FAILED: Reference count not decremented\n";
        return false;
    }
    
    ptr1.reset();
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testARCPtrBasic() {
    std::cout << "Testing ARCPtr basic functionality... ";
    
    // Create ARC-managed object
    TestObject* raw_obj = new TestObject(42);
    void* arc_obj = __tsc_alloc(sizeof(TestObject), test_destructor, nullptr);
    
    if (!arc_obj) {
        std::cout << "FAILED: ARC allocation failed\n";
        return false;
    }
    
    // Copy object to ARC-managed memory
    memcpy(arc_obj, raw_obj, sizeof(TestObject));
    delete raw_obj;
    
    {
        ARCPtr<TestObject> ptr1(static_cast<TestObject*>(arc_obj));
        
        if (!ptr1) {
            std::cout << "FAILED: ARCPtr should be valid\n";
            return false;
        }
        
        if (ptr1->getValue() != 42) {
            std::cout << "FAILED: Wrong value\n";
            return false;
        }
        
        {
            ARCPtr<TestObject> ptr2 = ptr1;
            
            if (ptr1->getValue() != ptr2->getValue()) {
                std::cout << "FAILED: Values should be equal\n";
                return false;
            }
        }
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testMallocPtrBasic() {
    std::cout << "Testing MallocPtr basic functionality... ";
    
    {
        MallocPtr ptr(1024);
        
        if (!ptr) {
            std::cout << "FAILED: MallocPtr should be valid\n";
            return false;
        }
        
        if (ptr.size() != 1024) {
            std::cout << "FAILED: Wrong size\n";
            return false;
        }
        
        // Test that we can write to the memory
        memset(ptr.get(), 0x42, 1024);
        
        // Verify the data
        char* data = static_cast<char*>(ptr.get());
        for (size_t i = 0; i < 1024; i++) {
            if (data[i] != 0x42) {
                std::cout << "FAILED: Data not written correctly\n";
                return false;
            }
        }
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testMakeUnique() {
    std::cout << "Testing make_unique... ";
    
    bool destroyed = false;
    auto ptr = make_unique<TestObject>(42, &destroyed);
    
    if (!ptr) {
        std::cout << "FAILED: make_unique should return valid pointer\n";
        return false;
    }
    
    if (ptr->getValue() != 42) {
        std::cout << "FAILED: Wrong value\n";
        return false;
    }
    
    ptr.reset();
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testMakeShared() {
    std::cout << "Testing make_shared... ";
    
    bool destroyed = false;
    auto ptr = make_shared<TestObject>(42, &destroyed);
    
    if (!ptr) {
        std::cout << "FAILED: make_shared should return valid pointer\n";
        return false;
    }
    
    if (ptr->getValue() != 42) {
        std::cout << "FAILED: Wrong value\n";
        return false;
    }
    
    if (ptr.use_count() != 1) {
        std::cout << "FAILED: Wrong reference count\n";
        return false;
    }
    
    ptr.reset();
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testMemoryLeakDetection() {
    std::cout << "Testing memory leak detection... ";
    
    // This test is only meaningful in debug builds with TSC_DEBUG_MEMORY
    // For now, just test that the functions exist and don't crash
    MemoryManager::reportLeaks();
    MemoryManager::cleanup();
    
    std::cout << "PASSED\n";
    return true;
}

bool testConcurrentAccess() {
    std::cout << "Testing concurrent access... ";
    
    bool destroyed = false;
    SharedPtr<TestObject> shared_ptr(new TestObject(42, &destroyed));
    
    std::vector<std::thread> threads;
    const int num_threads = 10;
    const int iterations = 100;
    
    // Create threads that copy and destroy SharedPtr
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&shared_ptr]() {
            for (int j = 0; j < iterations; j++) {
                SharedPtr<TestObject> local_ptr = shared_ptr;
                // Do some work
                local_ptr->getValue();
            }
        });
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Now destroy the shared_ptr
    shared_ptr.reset();
    
    if (!destroyed) {
        std::cout << "FAILED: Object not destroyed\n";
        return false;
    }
    
    std::cout << "PASSED\n";
    return true;
}

bool testPerformance() {
    std::cout << "Testing performance... ";
    
    const int iterations = 10000;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Test UniquePtr performance
    for (int i = 0; i < iterations; i++) {
        auto ptr = make_unique<TestObject>(i);
        ptr->getValue();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double avg_time = duration.count() / (double)iterations;
    
    // Check that it's reasonably fast (less than 10 microseconds per operation)
    if (avg_time > 10.0) {
        std::cout << "FAILED: Too slow (" << avg_time << " microseconds per operation)\n";
        return false;
    }
    
    std::cout << "PASSED (" << avg_time << " microseconds per operation)\n";
    return true;
}

int main() {
    std::cout << "=== Memory Management Tests ===\n";
    
    bool all_passed = true;
    
    all_passed &= testUniquePtrBasic();
    all_passed &= testUniquePtrMove();
    all_passed &= testSharedPtrBasic();
    all_passed &= testARCPtrBasic();
    all_passed &= testMallocPtrBasic();
    all_passed &= testMakeUnique();
    all_passed &= testMakeShared();
    all_passed &= testMemoryLeakDetection();
    all_passed &= testConcurrentAccess();
    all_passed &= testPerformance();
    
    std::cout << "\n=== Test Summary ===\n";
    if (all_passed) {
        std::cout << "All tests passed! ✅\n";
        return 0;
    } else {
        std::cout << "Some tests failed! ❌\n";
        return 1;
    }
}
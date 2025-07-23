#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <new>
using namespace std;

class CustomAllocator {
private:
    static size_t totalAllocated;
    static size_t allocationCount;
    
public:
    static void* allocate(size_t size) {
        void* ptr = malloc(size);
        if (ptr) {
            totalAllocated += size;
            allocationCount++;
            cout << "Allocated " << size << " bytes at " << ptr << endl;
        }
        return ptr;
    }
    
    static void deallocate(void* ptr, size_t size) {
        if (ptr) {
            totalAllocated -= size;
            allocationCount--;
            cout << "Deallocated " << size << " bytes at " << ptr << endl;
            free(ptr);
        }
    }
    
    static void printStats() {
        cout << "Memory Stats - Total: " << totalAllocated 
             << " bytes, Active allocations: " << allocationCount << endl;
    }
};

size_t CustomAllocator::totalAllocated = 0;
size_t CustomAllocator::allocationCount = 0;

class ManagedResource {
private:
    int* data;
    size_t size;
    string name;
    
public:
    ManagedResource(const string& n, size_t s) : name(n), size(s) {
        data = static_cast<int*>(CustomAllocator::allocate(size * sizeof(int)));
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i * i);
        }
        cout << "Created " << name << " with " << size << " elements" << endl;
    }
    
    ~ManagedResource() {
        if (data) {
            CustomAllocator::deallocate(data, size * sizeof(int));
            cout << "Destroyed " << name << endl;
        }
    }
    
    ManagedResource(const ManagedResource&) = delete;
    ManagedResource& operator=(const ManagedResource&) = delete;
    
    ManagedResource(ManagedResource&& other) noexcept 
        : data(other.data), size(other.size), name(move(other.name)) {
        other.data = nullptr;
        other.size = 0;
        cout << "Moved " << name << endl;
    }
    
    void display() const {
        cout << name << " (size " << size << "): ";
        if (data && size > 0) {
            for (size_t i = 0; i < min(size, size_t(5)); ++i) {
                cout << data[i] << " ";
            }
            if (size > 5) cout << "...";
        }
        cout << endl;
    }
};

class CircularReference {
public:
    shared_ptr<CircularReference> next;
    weak_ptr<CircularReference> parent;
    string name;
    
    CircularReference(const string& n) : name(n) {
        cout << "Created CircularReference: " << name << endl;
    }
    
    ~CircularReference() {
        cout << "Destroyed CircularReference: " << name << endl;
    }
    
    void setNext(shared_ptr<CircularReference> n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();
        }
    }
};

class CircularReferenceDemo : public enable_shared_from_this<CircularReference> {
public:
    shared_ptr<CircularReferenceDemo> next;
    weak_ptr<CircularReferenceDemo> parent;
    string name;
    
    CircularReferenceDemo(const string& n) : name(n) {
        cout << "Created CircularReferenceDemo: " << name << endl;
    }
    
    ~CircularReferenceDemo() {
        cout << "Destroyed CircularReferenceDemo: " << name << endl;
    }
    
    void setNext(shared_ptr<CircularReferenceDemo> n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();
        }
    }
};

void demonstrateSmartPointers() {
    cout << "\n=== Smart Pointers Demo ===" << endl;
    
    cout << "\n1. unique_ptr:" << endl;
    {
        auto uniqueRes = make_unique<ManagedResource>("UniqueResource", 3);
        uniqueRes->display();
        
        auto movedRes = move(uniqueRes);
        cout << "After move, original unique_ptr is: " 
             << (uniqueRes ? "valid" : "null") << endl;
        movedRes->display();
    }
    
    cout << "\n2. shared_ptr:" << endl;
    {
        auto sharedRes1 = make_shared<ManagedResource>("SharedResource", 4);
        cout << "Reference count: " << sharedRes1.use_count() << endl;
        
        {
            auto sharedRes2 = sharedRes1;
            cout << "Reference count after copy: " << sharedRes1.use_count() << endl;
            sharedRes2->display();
        }
        
        cout << "Reference count after scope exit: " << sharedRes1.use_count() << endl;
    }
    
    cout << "\n3. weak_ptr and circular reference prevention:" << endl;
    {
        auto node1 = make_shared<CircularReferenceDemo>("Node1");
        auto node2 = make_shared<CircularReferenceDemo>("Node2");
        auto node3 = make_shared<CircularReferenceDemo>("Node3");
        
        node1->setNext(node2);
        node2->setNext(node3);
        
        cout << "Node1 ref count: " << node1.use_count() << endl;
        cout << "Node2 ref count: " << node2.use_count() << endl;
        cout << "Node3 ref count: " << node3.use_count() << endl;
        
        if (auto parent = node3->parent.lock()) {
            cout << "Node3's parent: " << parent->name << endl;
        }
    }
}

void memoryLeakDemo() {
    cout << "\n=== Memory Leak Prevention ===" << endl;
    
    cout << "\nCreating resources with proper RAII:" << endl;
    {
        vector<unique_ptr<ManagedResource>> resources;
        resources.push_back(make_unique<ManagedResource>("Resource1", 5));
        resources.push_back(make_unique<ManagedResource>("Resource2", 3));
        resources.push_back(make_unique<ManagedResource>("Resource3", 7));
        
        for (const auto& res : resources) {
            res->display();
        }
        
        CustomAllocator::printStats();
    }
    
    cout << "\nAfter scope exit:" << endl;
    CustomAllocator::printStats();
}

void performanceComparison() {
    cout << "\n=== Performance Comparison ===" << endl;
    
    const size_t iterations = 1000000;
    
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        int* rawPtr = new int(42);
        delete rawPtr;
    }
    
    auto mid = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        auto smartPtr = make_unique<int>(42);
    }
    
    auto end = chrono::high_resolution_clock::now();
    
    auto rawTime = chrono::duration_cast<chrono::microseconds>(mid - start);
    auto smartTime = chrono::duration_cast<chrono::microseconds>(end - mid);
    
    cout << "Raw pointer allocation/deallocation: " << rawTime.count() << " μs" << endl;
    cout << "Smart pointer allocation/deallocation: " << smartTime.count() << " μs" << endl;
    cout << "Overhead: " << (smartTime.count() - rawTime.count()) << " μs" << endl;
}

int main() {
    cout << "Advanced Memory Management Demo" << endl;
    cout << "===============================" << endl;
    
    demonstrateSmartPointers();
    memoryLeakDemo();
    performanceComparison();
    
    cout << "\nFinal memory statistics:" << endl;
    CustomAllocator::printStats();
    
    return 0;
}
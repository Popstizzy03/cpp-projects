#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cassert>
using namespace std;

template<typename T, size_t PoolSize = 1024>
class MemoryPool {
private:
    union Block {
        T data;
        Block* next;
    };
    
    Block* freeList;
    vector<unique_ptr<Block[]>> chunks;
    size_t blocksPerChunk;
    size_t totalBlocks;
    size_t usedBlocks;
    
    void allocateNewChunk() {
        auto newChunk = make_unique<Block[]>(blocksPerChunk);
        
        for (size_t i = 0; i < blocksPerChunk - 1; ++i) {
            newChunk[i].next = &newChunk[i + 1];
        }
        newChunk[blocksPerChunk - 1].next = freeList;
        
        freeList = newChunk.get();
        chunks.push_back(move(newChunk));
        totalBlocks += blocksPerChunk;
    }
    
public:
    MemoryPool() : freeList(nullptr), blocksPerChunk(PoolSize), totalBlocks(0), usedBlocks(0) {
        allocateNewChunk();
    }
    
    ~MemoryPool() {
        for (size_t i = 0; i < usedBlocks; ++i) {
        }
    }
    
    T* allocate() {
        if (!freeList) {
            allocateNewChunk();
        }
        
        Block* block = freeList;
        freeList = freeList->next;
        usedBlocks++;
        
        return reinterpret_cast<T*>(block);
    }
    
    void deallocate(T* ptr) {
        if (!ptr) return;
        
        Block* block = reinterpret_cast<Block*>(ptr);
        block->next = freeList;
        freeList = block;
        usedBlocks--;
    }
    
    template<typename... Args>
    T* construct(Args&&... args) {
        T* ptr = allocate();
        new (ptr) T(forward<Args>(args)...);
        return ptr;
    }
    
    void destroy(T* ptr) {
        if (ptr) {
            ptr->~T();
            deallocate(ptr);
        }
    }
    
    size_t getUsedBlocks() const { return usedBlocks; }
    size_t getTotalBlocks() const { return totalBlocks; }
    size_t getChunkCount() const { return chunks.size(); }
    double getUtilization() const { 
        return totalBlocks > 0 ? static_cast<double>(usedBlocks) / totalBlocks : 0.0; 
    }
};

template<size_t Size>
class StackAllocator {
private:
    char buffer[Size];
    size_t top;
    
public:
    StackAllocator() : top(0) {}
    
    template<typename T>
    T* allocate(size_t count = 1) {
        size_t bytes = count * sizeof(T);
        size_t aligned_bytes = (bytes + alignof(T) - 1) & ~(alignof(T) - 1);
        
        if (top + aligned_bytes > Size) {
            throw bad_alloc();
        }
        
        T* ptr = reinterpret_cast<T*>(buffer + top);
        top += aligned_bytes;
        return ptr;
    }
    
    template<typename T>
    void deallocate(T* ptr, size_t count = 1) {
        size_t bytes = count * sizeof(T);
        size_t aligned_bytes = (bytes + alignof(T) - 1) & ~(alignof(T) - 1);
        
        if (reinterpret_cast<char*>(ptr) + aligned_bytes == buffer + top) {
            top -= aligned_bytes;
        }
    }
    
    void reset() {
        top = 0;
    }
    
    size_t getBytesUsed() const { return top; }
    size_t getBytesRemaining() const { return Size - top; }
};

class LinearAllocator {
private:
    char* buffer;
    size_t size;
    size_t offset;
    bool ownsMemory;
    
public:
    LinearAllocator(size_t bufferSize) 
        : size(bufferSize), offset(0), ownsMemory(true) {
        buffer = static_cast<char*>(malloc(bufferSize));
        if (!buffer) {
            throw bad_alloc();
        }
    }
    
    LinearAllocator(void* existingBuffer, size_t bufferSize)
        : buffer(static_cast<char*>(existingBuffer)), size(bufferSize), 
          offset(0), ownsMemory(false) {}
    
    ~LinearAllocator() {
        if (ownsMemory && buffer) {
            free(buffer);
        }
    }
    
    template<typename T>
    T* allocate(size_t count = 1) {
        size_t bytes = count * sizeof(T);
        size_t aligned_offset = (offset + alignof(T) - 1) & ~(alignof(T) - 1);
        
        if (aligned_offset + bytes > size) {
            throw bad_alloc();
        }
        
        T* ptr = reinterpret_cast<T*>(buffer + aligned_offset);
        offset = aligned_offset + bytes;
        return ptr;
    }
    
    void reset() {
        offset = 0;
    }
    
    size_t getBytesUsed() const { return offset; }
    size_t getBytesRemaining() const { return size - offset; }
};

template<typename Allocator>
class CustomAllocatorAdapter {
private:
    Allocator* allocator;
    
public:
    using value_type = char;
    
    CustomAllocatorAdapter(Allocator* alloc) : allocator(alloc) {}
    
    template<typename T>
    CustomAllocatorAdapter(const CustomAllocatorAdapter<T>& other) 
        : allocator(other.allocator) {}
    
    char* allocate(size_t n) {
        return allocator->template allocate<char>(n);
    }
    
    void deallocate(char* ptr, size_t n) {
        allocator->template deallocate<char>(ptr, n);
    }
    
    template<typename T>
    bool operator==(const CustomAllocatorAdapter<T>& other) const {
        return allocator == other.allocator;
    }
    
    template<typename T>
    bool operator!=(const CustomAllocatorAdapter<T>& other) const {
        return !(*this == other);
    }
};

class TestObject {
private:
    int data[10];
    string name;
    
public:
    TestObject(int value = 0, const string& n = "default") : name(n) {
        for (int& d : data) {
            d = value;
        }
    }
    
    ~TestObject() {
    }
    
    void setValue(int value) {
        for (int& d : data) {
            d = value;
        }
    }
    
    int getValue() const { return data[0]; }
    const string& getName() const { return name; }
};

void performanceComparison() {
    cout << "\n=== Performance Comparison ===" << endl;
    
    const size_t iterations = 100000;
    vector<TestObject*> objects;
    objects.reserve(iterations);
    
    cout << "Testing with " << iterations << " allocations/deallocations" << endl;
    
    auto start = chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        objects.push_back(new TestObject(static_cast<int>(i), "standard"));
    }
    for (TestObject* obj : objects) {
        delete obj;
    }
    
    auto mid = chrono::high_resolution_clock::now();
    
    MemoryPool<TestObject> pool;
    objects.clear();
    
    for (size_t i = 0; i < iterations; ++i) {
        objects.push_back(pool.construct(static_cast<int>(i), "pooled"));
    }
    for (TestObject* obj : objects) {
        pool.destroy(obj);
    }
    
    auto end = chrono::high_resolution_clock::now();
    
    auto standardTime = chrono::duration_cast<chrono::microseconds>(mid - start);
    auto poolTime = chrono::duration_cast<chrono::microseconds>(end - mid);
    
    cout << "Standard allocation time: " << standardTime.count() << " μs" << endl;
    cout << "Memory pool time: " << poolTime.count() << " μs" << endl;
    cout << "Speedup: " << static_cast<double>(standardTime.count()) / poolTime.count() << "x" << endl;
}

void demonstrateStackAllocator() {
    cout << "\n=== Stack Allocator Demo ===" << endl;
    
    StackAllocator<1024> stackAlloc;
    
    cout << "Initial state - Used: " << stackAlloc.getBytesUsed() 
         << ", Remaining: " << stackAlloc.getBytesRemaining() << endl;
    
    int* intArray = stackAlloc.allocate<int>(10);
    for (int i = 0; i < 10; ++i) {
        intArray[i] = i * i;
    }
    
    cout << "After int array allocation - Used: " << stackAlloc.getBytesUsed() 
         << ", Remaining: " << stackAlloc.getBytesRemaining() << endl;
    
    double* doubleArray = stackAlloc.allocate<double>(5);
    for (int i = 0; i < 5; ++i) {
        doubleArray[i] = i * 3.14;
    }
    
    cout << "After double array allocation - Used: " << stackAlloc.getBytesUsed() 
         << ", Remaining: " << stackAlloc.getBytesRemaining() << endl;
    
    cout << "Int array: ";
    for (int i = 0; i < 10; ++i) {
        cout << intArray[i] << " ";
    }
    cout << endl;
    
    cout << "Double array: ";
    for (int i = 0; i < 5; ++i) {
        cout << doubleArray[i] << " ";
    }
    cout << endl;
    
    stackAlloc.deallocate(doubleArray, 5);
    cout << "After deallocating double array - Used: " << stackAlloc.getBytesUsed() 
         << ", Remaining: " << stackAlloc.getBytesRemaining() << endl;
    
    stackAlloc.reset();
    cout << "After reset - Used: " << stackAlloc.getBytesUsed() 
         << ", Remaining: " << stackAlloc.getBytesRemaining() << endl;
}

void demonstrateLinearAllocator() {
    cout << "\n=== Linear Allocator Demo ===" << endl;
    
    LinearAllocator linearAlloc(2048);
    
    cout << "Initial state - Used: " << linearAlloc.getBytesUsed() 
         << ", Remaining: " << linearAlloc.getBytesRemaining() << endl;
    
    struct Point {
        float x, y, z;
        Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    };
    
    Point* points = linearAlloc.allocate<Point>(100);
    for (int i = 0; i < 100; ++i) {
        new (&points[i]) Point(i * 0.1f, i * 0.2f, i * 0.3f);
    }
    
    cout << "After point allocation - Used: " << linearAlloc.getBytesUsed() 
         << ", Remaining: " << linearAlloc.getBytesRemaining() << endl;
    
    char* stringBuffer = linearAlloc.allocate<char>(256);
    strcpy(stringBuffer, "Hello from linear allocator!");
    
    cout << "After string allocation - Used: " << linearAlloc.getBytesUsed() 
         << ", Remaining: " << linearAlloc.getBytesRemaining() << endl;
    
    cout << "String: " << stringBuffer << endl;
    cout << "First few points: ";
    for (int i = 0; i < 5; ++i) {
        cout << "(" << points[i].x << "," << points[i].y << "," << points[i].z << ") ";
    }
    cout << endl;
}

int main() {
    cout << "Advanced Memory Pool and Custom Allocators Demo" << endl;
    cout << "===============================================" << endl;
    
    cout << "\n1. Memory Pool Basic Usage:" << endl;
    {
        MemoryPool<TestObject> pool;
        
        cout << "Initial pool state - Used: " << pool.getUsedBlocks() 
             << ", Total: " << pool.getTotalBlocks() 
             << ", Chunks: " << pool.getChunkCount() << endl;
        
        vector<TestObject*> objects;
        
        for (int i = 0; i < 10; ++i) {
            objects.push_back(pool.construct(i * 10, "Object" + to_string(i)));
        }
        
        cout << "After creating 10 objects - Used: " << pool.getUsedBlocks() 
             << ", Utilization: " << (pool.getUtilization() * 100) << "%" << endl;
        
        for (size_t i = 0; i < objects.size(); ++i) {
            cout << "Object " << i << ": value=" << objects[i]->getValue() 
                 << ", name=" << objects[i]->getName() << endl;
        }
        
        for (size_t i = 0; i < 5; ++i) {
            pool.destroy(objects[i]);
        }
        objects.erase(objects.begin(), objects.begin() + 5);
        
        cout << "After destroying 5 objects - Used: " << pool.getUsedBlocks() 
             << ", Utilization: " << (pool.getUtilization() * 100) << "%" << endl;
        
        for (TestObject* obj : objects) {
            pool.destroy(obj);
        }
    }
    
    demonstrateStackAllocator();
    demonstrateLinearAllocator();
    performanceComparison();
    
    cout << "\n2. Memory Pool Stress Test:" << endl;
    {
        MemoryPool<int, 512> intPool;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 1000);
        
        vector<int*> allocated;
        
        for (int i = 0; i < 100; ++i) {
            if (dist(gen) > 500 && !allocated.empty()) {
                size_t index = gen() % allocated.size();
                intPool.deallocate(allocated[index]);
                allocated.erase(allocated.begin() + index);
            } else {
                int* ptr = intPool.allocate();
                *ptr = dist(gen);
                allocated.push_back(ptr);
            }
            
            if (i % 20 == 0) {
                cout << "Iteration " << i << " - Used: " << intPool.getUsedBlocks() 
                     << ", Total: " << intPool.getTotalBlocks() 
                     << ", Chunks: " << intPool.getChunkCount() << endl;
            }
        }
        
        for (int* ptr : allocated) {
            intPool.deallocate(ptr);
        }
        
        cout << "Final state - Used: " << intPool.getUsedBlocks() 
             << ", Total: " << intPool.getTotalBlocks() << endl;
    }
    
    return 0;
}
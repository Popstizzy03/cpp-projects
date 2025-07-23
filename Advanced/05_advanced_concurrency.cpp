#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <chrono>
#include <vector>
#include <queue>
#include <functional>
#include <random>
#include <shared_mutex>
using namespace std;

class ProducerConsumer {
private:
    queue<int> buffer;
    mutex bufferMutex;
    condition_variable notEmpty, notFull;
    static constexpr size_t BUFFER_SIZE = 10;
    atomic<bool> finished{false};
    
public:
    void produce(int id, int items) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 100);
        
        for (int i = 0; i < items; ++i) {
            int item = dist(gen);
            
            {
                unique_lock<mutex> lock(bufferMutex);
                notFull.wait(lock, [this] { return buffer.size() < BUFFER_SIZE; });
                
                buffer.push(item);
                cout << "Producer " << id << " produced: " << item 
                     << " (buffer size: " << buffer.size() << ")" << endl;
            }
            
            notEmpty.notify_one();
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
    
    void consume(int id, int items) {
        for (int i = 0; i < items; ++i) {
            int item;
            
            {
                unique_lock<mutex> lock(bufferMutex);
                notEmpty.wait(lock, [this] { return !buffer.empty() || finished.load(); });
                
                if (buffer.empty() && finished.load()) {
                    break;
                }
                
                item = buffer.front();
                buffer.pop();
                cout << "Consumer " << id << " consumed: " << item 
                     << " (buffer size: " << buffer.size() << ")" << endl;
            }
            
            notFull.notify_one();
            this_thread::sleep_for(chrono::milliseconds(15));
        }
    }
    
    void finish() {
        finished.store(true);
        notEmpty.notify_all();
    }
};

class ReadWriteLock {
private:
    shared_mutex rwMutex;
    string sharedData = "Initial Data";
    atomic<int> readerCount{0};
    atomic<int> writerCount{0};
    
public:
    void read(int id) {
        for (int i = 0; i < 3; ++i) {
            {
                shared_lock<shared_mutex> lock(rwMutex);
                readerCount++;
                cout << "Reader " << id << " reading: \"" << sharedData 
                     << "\" (readers: " << readerCount.load() << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(50));
                readerCount--;
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
    
    void write(int id) {
        for (int i = 0; i < 2; ++i) {
            {
                unique_lock<shared_mutex> lock(rwMutex);
                writerCount++;
                sharedData = "Data modified by writer " + to_string(id) + " iteration " + to_string(i);
                cout << "Writer " << id << " wrote: \"" << sharedData 
                     << "\" (writers: " << writerCount.load() << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(100));
                writerCount--;
            }
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    }
};

class LockFreeStack {
private:
    struct Node {
        atomic<int> data;
        atomic<Node*> next;
        
        Node(int value) : data(value), next(nullptr) {}
    };
    
    atomic<Node*> head{nullptr};
    
public:
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = head.load();
        
        while (!head.compare_exchange_weak(newNode->next, newNode)) {
        }
        
        cout << "Pushed " << value << " to lock-free stack" << endl;
    }
    
    bool pop(int& result) {
        Node* oldHead = head.load();
        
        while (oldHead && !head.compare_exchange_weak(oldHead, oldHead->next)) {
        }
        
        if (oldHead) {
            result = oldHead->data.load();
            cout << "Popped " << result << " from lock-free stack" << endl;
            delete oldHead;
            return true;
        }
        
        return false;
    }
    
    ~LockFreeStack() {
        while (head.load()) {
            Node* oldHead = head.load();
            head = oldHead->next;
            delete oldHead;
        }
    }
};

class ParallelAlgorithms {
public:
    static vector<int> parallelMap(const vector<int>& input, function<int(int)> func, size_t numThreads = 4) {
        vector<int> result(input.size());
        vector<thread> threads;
        size_t chunkSize = input.size() / numThreads;
        
        for (size_t i = 0; i < numThreads; ++i) {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? input.size() : start + chunkSize;
            
            threads.emplace_back([&input, &result, func, start, end]() {
                for (size_t j = start; j < end; ++j) {
                    result[j] = func(input[j]);
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        return result;
    }
    
    static int parallelReduce(const vector<int>& input, int identity, function<int(int, int)> reducer, size_t numThreads = 4) {
        vector<future<int>> futures;
        size_t chunkSize = input.size() / numThreads;
        
        for (size_t i = 0; i < numThreads; ++i) {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? input.size() : start + chunkSize;
            
            futures.push_back(async(launch::async, [&input, identity, reducer, start, end]() {
                int localResult = identity;
                for (size_t j = start; j < end; ++j) {
                    localResult = reducer(localResult, input[j]);
                }
                return localResult;
            }));
        }
        
        int finalResult = identity;
        for (auto& f : futures) {
            finalResult = reducer(finalResult, f.get());
        }
        
        return finalResult;
    }
};

class Barrier {
private:
    mutex mtx;
    condition_variable cv;
    size_t count;
    size_t waiting;
    
public:
    Barrier(size_t numThreads) : count(numThreads), waiting(0) {}
    
    void wait() {
        unique_lock<mutex> lock(mtx);
        ++waiting;
        
        if (waiting == count) {
            waiting = 0;
            cv.notify_all();
        } else {
            cv.wait(lock, [this] { return waiting == 0; });
        }
    }
};

void demonstrateBarrier() {
    cout << "\n=== Barrier Synchronization ===" << endl;
    
    const size_t numThreads = 4;
    Barrier barrier(numThreads);
    vector<thread> threads;
    
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([i, &barrier]() {
            cout << "Thread " << i << " doing work..." << endl;
            this_thread::sleep_for(chrono::milliseconds(100 * (i + 1)));
            
            cout << "Thread " << i << " reached barrier" << endl;
            barrier.wait();
            
            cout << "Thread " << i << " passed barrier" << endl;
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    cout << "Advanced Concurrency Patterns Demo" << endl;
    cout << "===================================" << endl;
    
    cout << "\n1. Producer-Consumer Pattern:" << endl;
    {
        ProducerConsumer pc;
        
        thread producer1([&pc]() { pc.produce(1, 5); });
        thread producer2([&pc]() { pc.produce(2, 3); });
        thread consumer1([&pc]() { pc.consume(1, 4); });
        thread consumer2([&pc]() { pc.consume(2, 4); });
        
        producer1.join();
        producer2.join();
        pc.finish();
        consumer1.join();
        consumer2.join();
    }
    
    cout << "\n2. Readers-Writers Lock:" << endl;
    {
        ReadWriteLock rwLock;
        
        vector<thread> threads;
        
        for (int i = 1; i <= 3; ++i) {
            threads.emplace_back([&rwLock, i]() { rwLock.read(i); });
        }
        
        for (int i = 1; i <= 2; ++i) {
            threads.emplace_back([&rwLock, i]() { rwLock.write(i); });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
    
    cout << "\n3. Lock-Free Data Structure:" << endl;
    {
        LockFreeStack stack;
        
        vector<thread> pushers;
        vector<thread> poppers;
        
        for (int i = 1; i <= 5; ++i) {
            pushers.emplace_back([&stack, i]() {
                stack.push(i * 10);
                this_thread::sleep_for(chrono::milliseconds(10));
            });
        }
        
        for (int i = 0; i < 3; ++i) {
            poppers.emplace_back([&stack, i]() {
                this_thread::sleep_for(chrono::milliseconds(50));
                int value;
                if (stack.pop(value)) {
                    cout << "Popper " << i << " got value: " << value << endl;
                }
            });
        }
        
        for (auto& t : pushers) t.join();
        for (auto& t : poppers) t.join();
    }
    
    cout << "\n4. Parallel Algorithms:" << endl;
    {
        vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        
        cout << "Original data: ";
        for (int x : data) cout << x << " ";
        cout << endl;
        
        auto squared = ParallelAlgorithms::parallelMap(data, [](int x) { return x * x; });
        cout << "Squared (parallel map): ";
        for (int x : squared) cout << x << " ";
        cout << endl;
        
        int sum = ParallelAlgorithms::parallelReduce(data, 0, [](int a, int b) { return a + b; });
        cout << "Sum (parallel reduce): " << sum << endl;
        
        int product = ParallelAlgorithms::parallelReduce(data, 1, [](int a, int b) { return a * b; });
        cout << "Product (parallel reduce): " << product << endl;
    }
    
    demonstrateBarrier();
    
    cout << "\n5. Asynchronous Task Execution:" << endl;
    {
        auto task1 = async(launch::async, []() {
            this_thread::sleep_for(chrono::milliseconds(100));
            return 42;
        });
        
        auto task2 = async(launch::async, []() {
            this_thread::sleep_for(chrono::milliseconds(150));
            return string("Hello from async");
        });
        
        auto task3 = async(launch::async, []() {
            this_thread::sleep_for(chrono::milliseconds(75));
            return 3.14159;
        });
        
        cout << "Waiting for tasks to complete..." << endl;
        cout << "Task 1 result: " << task1.get() << endl;
        cout << "Task 2 result: " << task2.get() << endl;
        cout << "Task 3 result: " << task3.get() << endl;
    }
    
    cout << "\nAll concurrency demonstrations completed!" << endl;
    return 0;
}
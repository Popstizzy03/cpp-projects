#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <atomic>
using namespace std;

template<typename T>
class Future {
private:
    shared_ptr<promise<T>> prom;
    shared_future<T> fut;
    
public:
    Future() : prom(make_shared<promise<T>>()) {
        fut = prom->get_future().share();
    }
    
    template<typename U>
    Future(U&& value) : prom(make_shared<promise<T>>()) {
        fut = prom->get_future().share();
        prom->set_value(forward<U>(value));
    }
    
    template<typename Func>
    auto then(Func&& func) -> Future<decltype(func(declval<T>()))> {
        using ReturnType = decltype(func(declval<T>()));
        
        auto nextFuture = Future<ReturnType>();
        
        thread([fut = this->fut, func = forward<Func>(func), nextProm = nextFuture.prom]() {
            try {
                if constexpr (is_void_v<ReturnType>) {
                    func(fut.get());
                    nextProm->set_value();
                } else {
                    auto result = func(fut.get());
                    nextProm->set_value(result);
                }
            } catch (...) {
                nextProm->set_exception(current_exception());
            }
        }).detach();
        
        return nextFuture;
    }
    
    T get() {
        return fut.get();
    }
    
    bool isReady() const {
        return fut.wait_for(chrono::seconds(0)) == future_status::ready;
    }
    
    template<typename Rep, typename Period>
    future_status waitFor(const chrono::duration<Rep, Period>& timeout) {
        return fut.wait_for(timeout);
    }
    
    void setValue(T value) {
        prom->set_value(value);
    }
    
    void setException(exception_ptr exc) {
        prom->set_exception(exc);
    }
};

template<>
class Future<void> {
private:
    shared_ptr<promise<void>> prom;
    shared_future<void> fut;
    
public:
    Future() : prom(make_shared<promise<void>>()) {
        fut = prom->get_future().share();
    }
    
    template<typename Func>
    auto then(Func&& func) -> Future<decltype(func())> {
        using ReturnType = decltype(func());
        
        auto nextFuture = Future<ReturnType>();
        
        thread([fut = this->fut, func = forward<Func>(func), nextProm = nextFuture.prom]() {
            try {
                fut.get();
                if constexpr (is_void_v<ReturnType>) {
                    func();
                    nextProm->set_value();
                } else {
                    auto result = func();
                    nextProm->set_value(result);
                }
            } catch (...) {
                nextProm->set_exception(current_exception());
            }
        }).detach();
        
        return nextFuture;
    }
    
    void get() {
        fut.get();
    }
    
    bool isReady() const {
        return fut.wait_for(chrono::seconds(0)) == future_status::ready;
    }
    
    void setValue() {
        prom->set_value();
    }
    
    void setException(exception_ptr exc) {
        prom->set_exception(exc);
    }
};

class AsyncTaskRunner {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex taskMutex;
    condition_variable taskCondition;
    atomic<bool> stopping{false};
    
public:
    AsyncTaskRunner(size_t numWorkers = thread::hardware_concurrency()) {
        for (size_t i = 0; i < numWorkers; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    function<void()> task;
                    
                    {
                        unique_lock<mutex> lock(taskMutex);
                        taskCondition.wait(lock, [this] { return stopping || !tasks.empty(); });
                        
                        if (stopping && tasks.empty()) {
                            return;
                        }
                        
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    template<typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args) -> future<decltype(func(args...))> {
        using ReturnType = decltype(func(args...));
        
        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<Func>(func), forward<Args>(args)...)
        );
        
        auto result = task->get_future();
        
        {
            unique_lock<mutex> lock(taskMutex);
            if (stopping) {
                throw runtime_error("AsyncTaskRunner is stopping");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        
        taskCondition.notify_one();
        return result;
    }
    
    ~AsyncTaskRunner() {
        {
            unique_lock<mutex> lock(taskMutex);
            stopping = true;
        }
        
        taskCondition.notify_all();
        
        for (thread& worker : workers) {
            worker.join();
        }
    }
};

template<typename T>
Future<vector<T>> whenAll(vector<Future<T>>& futures) {
    auto resultFuture = Future<vector<T>>();
    
    thread([futures = move(futures), resultProm = resultFuture.prom]() mutable {
        try {
            vector<T> results;
            results.reserve(futures.size());
            
            for (auto& fut : futures) {
                results.push_back(fut.get());
            }
            
            resultProm->set_value(move(results));
        } catch (...) {
            resultProm->set_exception(current_exception());
        }
    }).detach();
    
    return resultFuture;
}

template<typename T>
Future<T> whenAny(vector<Future<T>>& futures) {
    auto resultFuture = Future<T>();
    auto completed = make_shared<atomic<bool>>(false);
    
    for (auto& fut : futures) {
        thread([fut = move(fut), resultProm = resultFuture.prom, completed]() mutable {
            try {
                T result = fut.get();
                bool expected = false;
                if (completed->compare_exchange_strong(expected, true)) {
                    resultProm->set_value(result);
                }
            } catch (...) {
                bool expected = false;
                if (completed->compare_exchange_strong(expected, true)) {
                    resultProm->set_exception(current_exception());
                }
            }
        }).detach();
    }
    
    return resultFuture;
}

class AsyncHttpSimulator {
private:
    random_device rd;
    mutable mt19937 gen;
    mutable uniform_int_distribution<> delayDist;
    mutable uniform_real_distribution<> errorDist;
    
public:
    AsyncHttpSimulator() : gen(rd()), delayDist(100, 1000), errorDist(0.0, 1.0) {}
    
    Future<string> fetchData(const string& url) const {
        auto future = Future<string>();
        
        thread([this, url, future]() mutable {
            int delay = delayDist(gen);
            this_thread::sleep_for(chrono::milliseconds(delay));
            
            if (errorDist(gen) < 0.1) {
                future.setException(make_exception_ptr(runtime_error("Network error for " + url)));
            } else {
                future.setValue("Data from " + url + " (delay: " + to_string(delay) + "ms)");
            }
        }).detach();
        
        return future;
    }
    
    Future<int> processData(const string& data) const {
        auto future = Future<int>();
        
        thread([data, future]() mutable {
            this_thread::sleep_for(chrono::milliseconds(50));
            future.setValue(static_cast<int>(data.length()));
        }).detach();
        
        return future;
    }
};

void demonstrateBasicAsyncOperations() {
    cout << "\n=== Basic Async Operations ===" << endl;
    
    auto future1 = async(launch::async, []() {
        this_thread::sleep_for(chrono::milliseconds(100));
        return 42;
    });
    
    auto future2 = async(launch::async, []() {
        this_thread::sleep_for(chrono::milliseconds(150));
        return string("Hello Async");
    });
    
    auto future3 = async(launch::async, []() {
        this_thread::sleep_for(chrono::milliseconds(75));
        return 3.14159;
    });
    
    cout << "Waiting for results..." << endl;
    cout << "Future 1: " << future1.get() << endl;
    cout << "Future 2: " << future2.get() << endl;
    cout << "Future 3: " << future3.get() << endl;
}

void demonstrateCustomFutures() {
    cout << "\n=== Custom Future Implementation ===" << endl;
    
    auto future1 = Future<int>();
    auto future2 = future1.then([](int x) {
        cout << "Processing " << x << endl;
        return x * 2;
    });
    auto future3 = future2.then([](int x) {
        cout << "Processing " << x << " again" << endl;
        return to_string(x) + " (converted to string)";
    });
    
    thread([&future1]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        future1.setValue(21);
    }).detach();
    
    cout << "Final result: " << future3.get() << endl;
}

void demonstrateTaskRunner() {
    cout << "\n=== Async Task Runner ===" << endl;
    
    AsyncTaskRunner runner(4);
    
    vector<future<int>> futures;
    
    for (int i = 1; i <= 10; ++i) {
        futures.push_back(runner.submit([i]() {
            this_thread::sleep_for(chrono::milliseconds(50 * i));
            cout << "Task " << i << " completed on thread " << this_thread::get_id() << endl;
            return i * i;
        }));
    }
    
    cout << "Waiting for all tasks to complete..." << endl;
    for (size_t i = 0; i < futures.size(); ++i) {
        cout << "Task " << (i + 1) << " result: " << futures[i].get() << endl;
    }
}

void demonstrateHttpSimulation() {
    cout << "\n=== Async HTTP Simulation ===" << endl;
    
    AsyncHttpSimulator http;
    
    vector<string> urls = {
        "https://api1.example.com/data",
        "https://api2.example.com/info", 
        "https://api3.example.com/status",
        "https://api4.example.com/metrics"
    };
    
    vector<Future<string>> fetchFutures;
    for (const auto& url : urls) {
        fetchFutures.push_back(http.fetchData(url));
    }
    
    auto pipeline = fetchFutures[0].then([&http](const string& data) {
        cout << "Received: " << data << endl;
        return http.processData(data);
    }).then([](int length) {
        cout << "Processed data length: " << length << endl;
        return length * 2;
    });
    
    try {
        cout << "Pipeline result: " << pipeline.get() << endl;
    } catch (const exception& e) {
        cout << "Pipeline error: " << e.what() << endl;
    }
    
    cout << "\nFetching remaining URLs..." << endl;
    for (size_t i = 1; i < fetchFutures.size(); ++i) {
        try {
            cout << "URL " << (i + 1) << " result: " << fetchFutures[i].get() << endl;
        } catch (const exception& e) {
            cout << "URL " << (i + 1) << " error: " << e.what() << endl;
        }
    }
}

void demonstrateParallelComputations() {
    cout << "\n=== Parallel Computations ===" << endl;
    
    const size_t dataSize = 1000000;
    vector<int> data(dataSize);
    
    for (size_t i = 0; i < dataSize; ++i) {
        data[i] = static_cast<int>(i);
    }
    
    auto start = chrono::high_resolution_clock::now();
    
    size_t numChunks = thread::hardware_concurrency();
    size_t chunkSize = dataSize / numChunks;
    
    vector<future<long long>> sumFutures;
    
    for (size_t i = 0; i < numChunks; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = (i == numChunks - 1) ? dataSize : startIdx + chunkSize;
        
        sumFutures.push_back(async(launch::async, [&data, startIdx, endIdx]() {
            long long partialSum = 0;
            for (size_t j = startIdx; j < endIdx; ++j) {
                partialSum += data[j];
            }
            return partialSum;
        }));
    }
    
    long long totalSum = 0;
    for (auto& future : sumFutures) {
        totalSum += future.get();
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    cout << "Parallel sum of " << dataSize << " elements: " << totalSum << endl;
    cout << "Computation time: " << duration.count() << " microseconds" << endl;
    cout << "Used " << numChunks << " threads" << endl;
}

int main() {
    cout << "Advanced Asynchronous Programming Demo" << endl;
    cout << "=====================================" << endl;
    
    demonstrateBasicAsyncOperations();
    demonstrateCustomFutures();
    demonstrateTaskRunner();
    demonstrateHttpSimulation();
    demonstrateParallelComputations();
    
    cout << "\n=== Exception Handling in Async Context ===" << endl;
    
    auto riskyTask = async(launch::async, []() {
        this_thread::sleep_for(chrono::milliseconds(100));
        throw runtime_error("Something went wrong!");
        return 42;
    });
    
    try {
        cout << "Risky task result: " << riskyTask.get() << endl;
    } catch (const exception& e) {
        cout << "Caught exception: " << e.what() << endl;
    }
    
    cout << "\nAll async demonstrations completed!" << endl;
    return 0;
}
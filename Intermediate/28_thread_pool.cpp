#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <chrono>
using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop;
    
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    
                    {
                        unique_lock<mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        
                        if (stop && tasks.empty()) {
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
    
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
        using return_type = typename result_of<F(Args...)>::type;
        
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<return_type> result = task->get_future();
        
        {
            unique_lock<mutex> lock(queueMutex);
            
            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            
            tasks.emplace([task]() { (*task)(); });
        }
        
        condition.notify_one();
        return result;
    }
    
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }
        
        condition.notify_all();
        
        for (thread& worker : workers) {
            worker.join();
        }
    }
    
    size_t getWorkerCount() const {
        return workers.size();
    }
};

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void simulateWork(int id, int duration) {
    cout << "Task " << id << " starting on thread " 
         << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(duration));
    cout << "Task " << id << " completed after " << duration << "ms" << endl;
}

double calculatePi(int iterations) {
    double pi = 0.0;
    for (int i = 0; i < iterations; ++i) {
        pi += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    }
    return 4.0 * pi;
}

int main() {
    cout << "Thread Pool Implementation Demo" << endl;
    cout << "===============================" << endl;
    
    const size_t numThreads = thread::hardware_concurrency();
    cout << "Creating thread pool with " << numThreads << " threads" << endl;
    
    ThreadPool pool(numThreads);
    
    cout << "\n1. Simple Task Execution:" << endl;
    vector<future<void>> simpleTasks;
    
    for (int i = 1; i <= 5; ++i) {
        simpleTasks.emplace_back(
            pool.enqueue(simulateWork, i, 100 * i)
        );
    }
    
    for (auto& task : simpleTasks) {
        task.wait();
    }
    
    cout << "\n2. Mathematical Calculations:" << endl;
    vector<future<int>> fibTasks;
    vector<future<int>> factTasks;
    
    for (int i = 35; i <= 39; ++i) {
        fibTasks.emplace_back(pool.enqueue(fibonacci, i));
    }
    
    for (int i = 5; i <= 10; ++i) {
        factTasks.emplace_back(pool.enqueue(factorial, i));
    }
    
    cout << "Fibonacci results:" << endl;
    for (size_t i = 0; i < fibTasks.size(); ++i) {
        cout << "  fib(" << (35 + i) << ") = " << fibTasks[i].get() << endl;
    }
    
    cout << "Factorial results:" << endl;
    for (size_t i = 0; i < factTasks.size(); ++i) {
        cout << "  " << (5 + i) << "! = " << factTasks[i].get() << endl;
    }
    
    cout << "\n3. Pi Calculation with Different Precision:" << endl;
    vector<future<double>> piTasks;
    vector<int> iterations = {100000, 500000, 1000000, 2000000};
    
    auto start = chrono::high_resolution_clock::now();
    
    for (int iter : iterations) {
        piTasks.emplace_back(pool.enqueue(calculatePi, iter));
    }
    
    for (size_t i = 0; i < piTasks.size(); ++i) {
        double pi = piTasks[i].get();
        cout << "  Pi with " << iterations[i] << " iterations: " << pi << endl;
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "All Pi calculations completed in " << duration.count() << "ms" << endl;
    
    cout << "\n4. Lambda Function Tasks:" << endl;
    vector<future<string>> lambdaTasks;
    
    for (int i = 1; i <= 3; ++i) {
        lambdaTasks.emplace_back(pool.enqueue([i]() -> string {
            this_thread::sleep_for(chrono::milliseconds(50));
            return "Lambda result " + to_string(i * i);
        }));
    }
    
    for (auto& task : lambdaTasks) {
        cout << "  " << task.get() << endl;
    }
    
    cout << "\nAll tasks completed. Thread pool will be destroyed." << endl;
    return 0;
}
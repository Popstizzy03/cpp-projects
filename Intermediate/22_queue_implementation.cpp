#include <iostream>
#include <deque>
using namespace std;

template<typename T>
class Queue {
private:
    deque<T> data;
    
public:
    void enqueue(const T& item) {
        data.push_back(item);
    }
    
    void dequeue() {
        if (!isEmpty()) {
            data.pop_front();
        } else {
            throw runtime_error("Queue is empty");
        }
    }
    
    T front() const {
        if (!isEmpty()) {
            return data.front();
        } else {
            throw runtime_error("Queue is empty");
        }
    }
    
    T back() const {
        if (!isEmpty()) {
            return data.back();
        } else {
            throw runtime_error("Queue is empty");
        }
    }
    
    bool isEmpty() const {
        return data.empty();
    }
    
    size_t size() const {
        return data.size();
    }
    
    void display() const {
        cout << "Queue (front to back): ";
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }
};

int main() {
    Queue<int> intQueue;
    
    intQueue.enqueue(10);
    intQueue.enqueue(20);
    intQueue.enqueue(30);
    intQueue.enqueue(40);
    
    intQueue.display();
    
    cout << "Front element: " << intQueue.front() << endl;
    cout << "Back element: " << intQueue.back() << endl;
    cout << "Queue size: " << intQueue.size() << endl;
    
    intQueue.dequeue();
    intQueue.display();
    
    return 0;
}
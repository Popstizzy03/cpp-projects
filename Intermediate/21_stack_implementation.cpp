#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Stack {
private:
    vector<T> data;
    
public:
    void push(const T& item) {
        data.push_back(item);
    }
    
    void pop() {
        if (!isEmpty()) {
            data.pop_back();
        } else {
            throw runtime_error("Stack is empty");
        }
    }
    
    T top() const {
        if (!isEmpty()) {
            return data.back();
        } else {
            throw runtime_error("Stack is empty");
        }
    }
    
    bool isEmpty() const {
        return data.empty();
    }
    
    size_t size() const {
        return data.size();
    }
    
    void display() const {
        cout << "Stack (top to bottom): ";
        for (auto it = data.rbegin(); it != data.rend(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
};

int main() {
    Stack<int> intStack;
    
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    
    intStack.display();
    
    cout << "Top element: " << intStack.top() << endl;
    cout << "Stack size: " << intStack.size() << endl;
    
    intStack.pop();
    intStack.display();
    
    Stack<string> stringStack;
    stringStack.push("First");
    stringStack.push("Second");
    stringStack.push("Third");
    stringStack.display();
    
    return 0;
}
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

template<typename T>
class NumberSequence {
private:
    T start, step;
    size_t count;
    
public:
    NumberSequence(T start, T step, size_t count) 
        : start(start), step(step), count(count) {}
    
    class Iterator {
    private:
        T current;
        T step;
        size_t position;
        
    public:
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator(T start, T step, size_t pos) 
            : current(start + step * pos), step(step), position(pos) {}
        
        T operator*() const { return current; }
        
        Iterator& operator++() {
            current += step;
            ++position;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return position == other.position;
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };
    
    Iterator begin() const {
        return Iterator(start, step, 0);
    }
    
    Iterator end() const {
        return Iterator(start, step, count);
    }
};

template<typename Container>
class ReverseWrapper {
private:
    Container& container;
    
public:
    ReverseWrapper(Container& c) : container(c) {}
    
    auto begin() { return container.rbegin(); }
    auto end() { return container.rend(); }
    auto begin() const { return container.crbegin(); }
    auto end() const { return container.crend(); }
};

template<typename Container>
ReverseWrapper<Container> reverse(Container& c) {
    return ReverseWrapper<Container>(c);
}

int main() {
    cout << "Custom Number Sequence Iterator:" << endl;
    cout << "================================" << endl;
    
    NumberSequence<int> evenNumbers(2, 2, 10);
    cout << "Even numbers (2, step=2, count=10): ";
    for (auto num : evenNumbers) {
        cout << num << " ";
    }
    cout << endl;
    
    NumberSequence<double> decimals(0.5, 0.5, 8);
    cout << "Decimal sequence (0.5, step=0.5, count=8): ";
    for (auto num : decimals) {
        cout << num << " ";
    }
    cout << endl;
    
    cout << "\nReverse Iterator Wrapper:" << endl;
    cout << "=========================" << endl;
    
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    cout << "Original: ";
    for (auto num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    
    cout << "Reversed: ";
    for (auto num : reverse(numbers)) {
        cout << num << " ";
    }
    cout << endl;
    
    cout << "\nUsing STL algorithms with custom iterators:" << endl;
    auto seq = NumberSequence<int>(1, 3, 7);
    auto it = find(seq.begin(), seq.end(), 10);
    if (it != seq.end()) {
        cout << "Found 10 in sequence" << endl;
    } else {
        cout << "10 not found in sequence" << endl;
    }
    
    return 0;
}
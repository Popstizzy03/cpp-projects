#include <iostream>
#include <memory>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <initializer_list>
#include <functional>
using namespace std;

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t capacity;
    size_t count;
    
    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = new T[capacity];
        
        for (size_t i = 0; i < count; ++i) {
            newData[i] = move(data[i]);
        }
        
        delete[] data;
        data = newData;
    }
    
public:
    class Iterator {
    private:
        T* ptr;
        
    public:
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator(T* p) : ptr(p) {}
        
        reference operator*() { return *ptr; }
        pointer operator->() { return ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++ptr; return temp; }
        Iterator& operator--() { --ptr; return *this; }
        Iterator operator--(int) { Iterator temp = *this; --ptr; return temp; }
        
        Iterator operator+(difference_type n) const { return Iterator(ptr + n); }
        Iterator operator-(difference_type n) const { return Iterator(ptr - n); }
        difference_type operator-(const Iterator& other) const { return ptr - other.ptr; }
        
        Iterator& operator+=(difference_type n) { ptr += n; return *this; }
        Iterator& operator-=(difference_type n) { ptr -= n; return *this; }
        
        reference operator[](difference_type n) { return ptr[n]; }
        
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        bool operator<(const Iterator& other) const { return ptr < other.ptr; }
        bool operator>(const Iterator& other) const { return ptr > other.ptr; }
        bool operator<=(const Iterator& other) const { return ptr <= other.ptr; }
        bool operator>=(const Iterator& other) const { return ptr >= other.ptr; }
    };
    
    using iterator = Iterator;
    using const_iterator = const Iterator;
    
    DynamicArray() : data(nullptr), capacity(0), count(0) {}
    
    DynamicArray(initializer_list<T> init) : data(nullptr), capacity(0), count(0) {
        for (const auto& item : init) {
            push_back(item);
        }
    }
    
    DynamicArray(const DynamicArray& other) : data(nullptr), capacity(other.capacity), count(other.count) {
        if (capacity > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < count; ++i) {
                data[i] = other.data[i];
            }
        }
    }
    
    DynamicArray(DynamicArray&& other) noexcept 
        : data(other.data), capacity(other.capacity), count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
    }
    
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            
            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < count; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }
    
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            count = other.count;
            
            other.data = nullptr;
            other.capacity = 0;
            other.count = 0;
        }
        return *this;
    }
    
    ~DynamicArray() {
        delete[] data;
    }
    
    void push_back(const T& value) {
        if (count >= capacity) {
            resize();
        }
        data[count++] = value;
    }
    
    void push_back(T&& value) {
        if (count >= capacity) {
            resize();
        }
        data[count++] = move(value);
    }
    
    void pop_back() {
        if (count > 0) {
            --count;
        }
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    T& at(size_t index) {
        if (index >= count) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (index >= count) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }
    
    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
    bool empty() const { return count == 0; }
    
    iterator begin() { return Iterator(data); }
    iterator end() { return Iterator(data + count); }
    const_iterator begin() const { return Iterator(data); }
    const_iterator end() const { return Iterator(data + count); }
};

template<typename T>
class CircularBuffer {
private:
    T* buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    
public:
    CircularBuffer(size_t cap) : capacity(cap), head(0), tail(0), count(0) {
        buffer = new T[capacity];
    }
    
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;
    
    CircularBuffer(CircularBuffer&& other) noexcept
        : buffer(other.buffer), capacity(other.capacity), 
          head(other.head), tail(other.tail), count(other.count) {
        other.buffer = nullptr;
        other.capacity = 0;
        other.head = 0;
        other.tail = 0;
        other.count = 0;
    }
    
    ~CircularBuffer() {
        delete[] buffer;
    }
    
    void push(const T& value) {
        buffer[tail] = value;
        tail = (tail + 1) % capacity;
        
        if (count == capacity) {
            head = (head + 1) % capacity;
        } else {
            ++count;
        }
    }
    
    T pop() {
        if (empty()) {
            throw runtime_error("Buffer is empty");
        }
        
        T value = buffer[head];
        head = (head + 1) % capacity;
        --count;
        return value;
    }
    
    const T& front() const {
        if (empty()) {
            throw runtime_error("Buffer is empty");
        }
        return buffer[head];
    }
    
    const T& back() const {
        if (empty()) {
            throw runtime_error("Buffer is empty");
        }
        return buffer[(tail - 1 + capacity) % capacity];
    }
    
    bool empty() const { return count == 0; }
    bool full() const { return count == capacity; }
    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
    
    void display() const {
        cout << "CircularBuffer: [";
        for (size_t i = 0; i < count; ++i) {
            size_t index = (head + i) % capacity;
            cout << buffer[index];
            if (i < count - 1) cout << ", ";
        }
        cout << "] (size: " << count << "/" << capacity << ")" << endl;
    }
};

template<typename Key, typename Value, typename Hash = hash<Key>>
class SimpleHashMap {
private:
    struct Entry {
        Key key;
        Value value;
        bool occupied;
        bool deleted;
        
        Entry() : occupied(false), deleted(false) {}
    };
    
    Entry* table;
    size_t capacity;
    size_t count;
    Hash hasher;
    
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;
    
    size_t findSlot(const Key& key) const {
        size_t index = hasher(key) % capacity;
        size_t original = index;
        
        while (table[index].occupied && table[index].key != key) {
            index = (index + 1) % capacity;
            if (index == original) {
                throw runtime_error("Hash table is full");
            }
        }
        
        return index;
    }
    
    void rehash() {
        Entry* oldTable = table;
        size_t oldCapacity = capacity;
        
        capacity *= 2;
        table = new Entry[capacity];
        count = 0;
        
        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        
        delete[] oldTable;
    }
    
public:
    SimpleHashMap(size_t initialCapacity = 16) : capacity(initialCapacity), count(0) {
        table = new Entry[capacity];
    }
    
    ~SimpleHashMap() {
        delete[] table;
    }
    
    SimpleHashMap(const SimpleHashMap&) = delete;
    SimpleHashMap& operator=(const SimpleHashMap&) = delete;
    
    void insert(const Key& key, const Value& value) {
        if (static_cast<double>(count) / capacity > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }
        
        size_t index = findSlot(key);
        
        if (!table[index].occupied) {
            ++count;
        }
        
        table[index].key = key;
        table[index].value = value;
        table[index].occupied = true;
        table[index].deleted = false;
    }
    
    Value* find(const Key& key) {
        size_t index = hasher(key) % capacity;
        size_t original = index;
        
        while (table[index].occupied || table[index].deleted) {
            if (table[index].occupied && !table[index].deleted && table[index].key == key) {
                return &table[index].value;
            }
            index = (index + 1) % capacity;
            if (index == original) {
                break;
            }
        }
        
        return nullptr;
    }
    
    bool remove(const Key& key) {
        Value* value = find(key);
        if (value) {
            size_t index = hasher(key) % capacity;
            size_t original = index;
            
            while (table[index].key != key) {
                index = (index + 1) % capacity;
                if (index == original) {
                    return false;
                }
            }
            
            table[index].deleted = true;
            --count;
            return true;
        }
        return false;
    }
    
    Value& operator[](const Key& key) {
        Value* value = find(key);
        if (value) {
            return *value;
        } else {
            insert(key, Value{});
            return *find(key);
        }
    }
    
    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
    double getLoadFactor() const { return static_cast<double>(count) / capacity; }
    
    void display() const {
        cout << "HashMap (size: " << count << ", capacity: " << capacity 
             << ", load factor: " << getLoadFactor() << "):" << endl;
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].occupied && !table[i].deleted) {
                cout << "  [" << i << "] " << table[i].key << " -> " << table[i].value << endl;
            }
        }
    }
};

template<typename T, typename Compare = less<T>>
class BinarySearchTree {
private:
    struct Node {
        T data;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        
        Node(const T& value) : data(value) {}
    };
    
    unique_ptr<Node> root;
    Compare comp;
    size_t count;
    
    void insertHelper(unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = make_unique<Node>(value);
            ++count;
        } else if (comp(value, node->data)) {
            insertHelper(node->left, value);
        } else if (comp(node->data, value)) {
            insertHelper(node->right, value);
        }
    }
    
    bool findHelper(const unique_ptr<Node>& node, const T& value) const {
        if (!node) {
            return false;
        } else if (comp(value, node->data)) {
            return findHelper(node->left, value);
        } else if (comp(node->data, value)) {
            return findHelper(node->right, value);
        } else {
            return true;
        }
    }
    
    void inorderHelper(const unique_ptr<Node>& node, function<void(const T&)> visit) const {
        if (node) {
            inorderHelper(node->left, visit);
            visit(node->data);
            inorderHelper(node->right, visit);
        }
    }
    
public:
    BinarySearchTree() : count(0) {}
    
    void insert(const T& value) {
        insertHelper(root, value);
    }
    
    bool find(const T& value) const {
        return findHelper(root, value);
    }
    
    size_t size() const { return count; }
    bool empty() const { return count == 0; }
    
    template<typename Func>
    void inorderTraversal(Func func) const {
        inorderHelper(root, func);
    }
    
    void display() const {
        cout << "BST inorder: ";
        inorderTraversal([](const T& value) {
            cout << value << " ";
        });
        cout << "(size: " << count << ")" << endl;
    }
};

void demonstrateDynamicArray() {
    cout << "\n=== Dynamic Array Demo ===" << endl;
    
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    
    cout << "Initial array: ";
    for (const auto& item : arr) {
        cout << item << " ";
    }
    cout << "(size: " << arr.size() << ", capacity: " << arr.getCapacity() << ")" << endl;
    
    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    
    cout << "After adding elements: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << "(size: " << arr.size() << ", capacity: " << arr.getCapacity() << ")" << endl;
    
    arr.pop_back();
    cout << "After pop_back: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    cout << "Using STL algorithms:" << endl;
    sort(arr.begin(), arr.end(), greater<int>());
    cout << "Sorted (descending): ";
    for (const auto& item : arr) {
        cout << item << " ";
    }
    cout << endl;
    
    auto it = find(arr.begin(), arr.end(), 5);
    if (it != arr.end()) {
        cout << "Found 5 at position: " << (it - arr.begin()) << endl;
    }
}

void demonstrateCircularBuffer() {
    cout << "\n=== Circular Buffer Demo ===" << endl;
    
    CircularBuffer<int> buffer(5);
    
    cout << "Adding elements 1-7 to buffer of size 5:" << endl;
    for (int i = 1; i <= 7; ++i) {
        buffer.push(i);
        cout << "After push(" << i << "): ";
        buffer.display();
    }
    
    cout << "\nPopping elements:" << endl;
    for (int i = 0; i < 3; ++i) {
        int value = buffer.pop();
        cout << "Popped: " << value << ", ";
        buffer.display();
    }
    
    cout << "\nAdding more elements:" << endl;
    for (int i = 10; i <= 12; ++i) {
        buffer.push(i);
        cout << "After push(" << i << "): ";
        buffer.display();
    }
}

void demonstrateHashMap() {
    cout << "\n=== Simple Hash Map Demo ===" << endl;
    
    SimpleHashMap<string, int> map;
    
    map.insert("apple", 100);
    map.insert("banana", 200);
    map.insert("cherry", 300);
    map.insert("date", 400);
    
    map.display();
    
    cout << "\nLookup operations:" << endl;
    string keys[] = {"apple", "banana", "grape", "cherry"};
    
    for (const auto& key : keys) {
        int* value = map.find(key);
        if (value) {
            cout << key << " -> " << *value << endl;
        } else {
            cout << key << " -> not found" << endl;
        }
    }
    
    cout << "\nUsing operator[]:" << endl;
    map["elderberry"] = 500;
    map["apple"] = 150;
    
    cout << "elderberry: " << map["elderberry"] << endl;
    cout << "apple (updated): " << map["apple"] << endl;
    
    map.display();
    
    cout << "\nRemoving 'banana':" << endl;
    map.remove("banana");
    map.display();
}

void demonstrateBinarySearchTree() {
    cout << "\n=== Binary Search Tree Demo ===" << endl;
    
    BinarySearchTree<int> bst;
    
    vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    
    cout << "Inserting values: ";
    for (int value : values) {
        cout << value << " ";
        bst.insert(value);
    }
    cout << endl;
    
    bst.display();
    
    cout << "\nSearch operations:" << endl;
    vector<int> searchValues = {25, 55, 80, 90};
    
    for (int value : searchValues) {
        bool found = bst.find(value);
        cout << "Find " << value << ": " << (found ? "found" : "not found") << endl;
    }
    
    cout << "\nCustom traversal (values > 40):" << endl;
    bst.inorderTraversal([](int value) {
        if (value > 40) {
            cout << value << " ";
        }
    });
    cout << endl;
}

void demonstrateContainerComparison() {
    cout << "\n=== Container Performance Comparison ===" << endl;
    
    const size_t testSize = 10000;
    
    auto start = chrono::high_resolution_clock::now();
    DynamicArray<int> dynArr;
    for (size_t i = 0; i < testSize; ++i) {
        dynArr.push_back(static_cast<int>(i));
    }
    auto end = chrono::high_resolution_clock::now();
    auto dynArrTime = chrono::duration_cast<chrono::microseconds>(end - start);
    
    start = chrono::high_resolution_clock::now();
    vector<int> stdVec;
    for (size_t i = 0; i < testSize; ++i) {
        stdVec.push_back(static_cast<int>(i));
    }
    end = chrono::high_resolution_clock::now();
    auto stdVecTime = chrono::duration_cast<chrono::microseconds>(end - start);
    
    cout << "Performance test (" << testSize << " push_back operations):" << endl;
    cout << "DynamicArray: " << dynArrTime.count() << " μs" << endl;
    cout << "std::vector: " << stdVecTime.count() << " μs" << endl;
    cout << "Ratio: " << static_cast<double>(dynArrTime.count()) / stdVecTime.count() << "x" << endl;
}

int main() {
    cout << "Custom Container Implementations Demo" << endl;
    cout << "====================================" << endl;
    
    demonstrateDynamicArray();
    demonstrateCircularBuffer();
    demonstrateHashMap();
    demonstrateBinarySearchTree();
    demonstrateContainerComparison();
    
    cout << "\nAll container demonstrations completed!" << endl;
    return 0;
}
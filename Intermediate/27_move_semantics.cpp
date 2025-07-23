#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>
using namespace std;

class Resource {
private:
    int* data;
    size_t size;
    string name;
    
public:
    Resource(const string& n, size_t s) : name(n), size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
        cout << "Constructor: " << name << " (size: " << size << ")" << endl;
    }
    
    Resource(const Resource& other) : name(other.name + "_copy"), size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        cout << "Copy Constructor: " << name << " from " << other.name << endl;
    }
    
    Resource(Resource&& other) noexcept 
        : data(other.data), size(other.size), name(move(other.name) + "_moved") {
        other.data = nullptr;
        other.size = 0;
        cout << "Move Constructor: " << name << endl;
    }
    
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data;
            name = other.name + "_assigned";
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            cout << "Copy Assignment: " << name << " from " << other.name << endl;
        }
        return *this;
    }
    
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            name = move(other.name) + "_move_assigned";
            
            other.data = nullptr;
            other.size = 0;
            cout << "Move Assignment: " << name << endl;
        }
        return *this;
    }
    
    ~Resource() {
        delete[] data;
        if (!name.empty()) {
            cout << "Destructor: " << name << endl;
        }
    }
    
    void display() const {
        cout << "Resource " << name << " (size: " << size << ")";
        if (data && size > 0) {
            cout << " [" << data[0] << "..." << data[size-1] << "]";
        }
        cout << endl;
    }
    
    const string& getName() const { return name; }
};

Resource createResource(const string& name, size_t size) {
    return Resource(name, size);
}

template<typename T>
void processValue(T&& value) {
    cout << "Processing: ";
    if constexpr (is_lvalue_reference_v<T>) {
        cout << "lvalue reference" << endl;
    } else {
        cout << "rvalue reference" << endl;
    }
    value.display();
}

int main() {
    cout << "Move Semantics and Perfect Forwarding Demo" << endl;
    cout << "==========================================" << endl;
    
    cout << "\n1. Basic Construction:" << endl;
    Resource r1("Resource1", 5);
    r1.display();
    
    cout << "\n2. Copy Construction:" << endl;
    Resource r2 = r1;
    r2.display();
    
    cout << "\n3. Move Construction:" << endl;
    Resource r3 = move(r1);
    r3.display();
    cout << "r1 after move: ";
    r1.display();
    
    cout << "\n4. Assignment Operations:" << endl;
    Resource r4("Resource4", 3);
    r4 = r2;
    Resource r5("Resource5", 2);
    r5 = move(r3);
    
    cout << "\n5. Return Value Optimization (RVO):" << endl;
    Resource r6 = createResource("RVO_Resource", 7);
    r6.display();
    
    cout << "\n6. Vector with Move Semantics:" << endl;
    vector<Resource> resources;
    resources.reserve(3);
    
    cout << "Emplacing resources:" << endl;
    resources.emplace_back("VectorResource1", 4);
    resources.emplace_back("VectorResource2", 6);
    
    cout << "Moving resource into vector:" << endl;
    Resource temp("TempResource", 8);
    resources.push_back(move(temp));
    
    cout << "\n7. Perfect Forwarding:" << endl;
    Resource lvalueRes("LValue", 3);
    processValue(lvalueRes);
    processValue(Resource("RValue", 2));
    
    cout << "\n8. Using move with standard containers:" << endl;
    vector<string> source = {"apple", "banana", "cherry"};
    vector<string> destination;
    
    cout << "Before move - source size: " << source.size() << endl;
    destination = move(source);
    cout << "After move - source size: " << source.size() << endl;
    cout << "Destination size: " << destination.size() << endl;
    
    cout << "\nEnd of program - destructors will be called:" << endl;
    return 0;
}
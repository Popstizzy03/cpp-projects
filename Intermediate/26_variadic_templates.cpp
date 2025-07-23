#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <type_traits>
using namespace std;

template<typename T>
void print(const T& value) {
    cout << value << endl;
}

template<typename T, typename... Args>
void print(const T& first, const Args&... args) {
    cout << first << " ";
    print(args...);
}

template<typename... Args>
auto sum(Args... args) -> decltype((args + ...)) {
    return (args + ...);
}

template<typename... Args>
auto multiply(Args... args) -> decltype((args * ...)) {
    return (args * ...);
}

template<typename... Args>
string concatenate(Args... args) {
    ostringstream oss;
    ((oss << args), ...);
    return oss.str();
}

template<typename... Args>
void printTypes() {
    cout << "Types: ";
    ((cout << typeid(Args).name() << " "), ...);
    cout << endl;
}

template<typename T>
void addToVector(vector<T>& vec, const T& value) {
    vec.push_back(value);
}

template<typename T, typename... Args>
void addToVector(vector<T>& vec, const T& first, const Args&... args) {
    vec.push_back(first);
    addToVector(vec, args...);
}

template<typename... Args>
auto makeVector(Args... args) {
    using FirstType = typename tuple_element<0, tuple<Args...>>::type;
    vector<FirstType> result;
    addToVector(result, args...);
    return result;
}

template<typename... Args>
size_t countArgs(Args... args) {
    return sizeof...(args);
}

int main() {
    cout << "Variadic Templates Demo" << endl;
    cout << "=======================" << endl;
    
    cout << "\n1. Variadic Print Function:" << endl;
    print("Hello", "World", 42, 3.14, 'A');
    print(1, 2, 3, 4, 5);
    print("Single argument");
    
    cout << "\n2. Variadic Sum and Multiply:" << endl;
    cout << "Sum of 1,2,3,4,5: " << sum(1, 2, 3, 4, 5) << endl;
    cout << "Sum of 1.5,2.5,3.0: " << sum(1.5, 2.5, 3.0) << endl;
    cout << "Multiply 2,3,4,5: " << multiply(2, 3, 4, 5) << endl;
    
    cout << "\n3. String Concatenation:" << endl;
    cout << "Concatenated: " << concatenate("Hello", " ", "World", " ", 2024) << endl;
    
    cout << "\n4. Type Information:" << endl;
    printTypes<int, double, string, char>();
    
    cout << "\n5. Creating Vector from Variadic Args:" << endl;
    auto intVector = makeVector(10, 20, 30, 40, 50);
    cout << "Vector contents: ";
    for (const auto& val : intVector) {
        cout << val << " ";
    }
    cout << endl;
    
    cout << "\n6. Counting Arguments:" << endl;
    cout << "Number of args (1,2,3,4,5): " << countArgs(1, 2, 3, 4, 5) << endl;
    cout << "Number of args ('a','b','c'): " << countArgs('a', 'b', 'c') << endl;
    
    return 0;
}
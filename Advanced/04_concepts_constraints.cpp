#include <iostream>
#include <vector>
#include <string>
#include <concepts>
#include <type_traits>
#include <iterator>
using namespace std;

template<typename T>
concept Integral = is_integral_v<T>;

template<typename T>
concept FloatingPoint = is_floating_point_v<T>;

template<typename T>
concept Numeric = Integral<T> || FloatingPoint<T>;

template<typename T>
concept Printable = requires(T t) {
    cout << t;
};

template<typename T>
concept Container = requires(T t) {
    t.begin();
    t.end();
    t.size();
};

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> convertible_to<bool>;
    { a > b } -> convertible_to<bool>;
    { a == b } -> convertible_to<bool>;
};

template<typename T>
concept Iterator = requires(T it) {
    ++it;
    *it;
    it != it;
};

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

template<Integral T>
T factorial(T n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

template<FloatingPoint T>
T circleArea(T radius) {
    return 3.14159 * radius * radius;
}

template<Printable T>
void safePrint(const T& value) {
    cout << "Printing: " << value << endl;
}

template<Container C>
void printContainer(const C& container) {
    cout << "Container contents: ";
    for (const auto& item : container) {
        cout << item << " ";
    }
    cout << " (size: " << container.size() << ")" << endl;
}

template<typename T>
requires Addable<T> && Comparable<T>
T findMax(const vector<T>& vec) {
    if (vec.empty()) {
        throw invalid_argument("Vector is empty");
    }
    
    T maxVal = vec[0];
    for (const auto& item : vec) {
        if (item > maxVal) {
            maxVal = item;
        }
    }
    return maxVal;
}

template<Iterator It>
void advanceIterator(It& it, size_t steps) {
    for (size_t i = 0; i < steps; ++i) {
        ++it;
    }
}

template<typename T>
concept Sortable = requires(T container) {
    sort(container.begin(), container.end());
};

template<Sortable C>
void sortContainer(C& container) {
    sort(container.begin(), container.end());
    cout << "Container sorted successfully" << endl;
}

template<typename T>
concept Serializable = requires(T t) {
    { t.serialize() } -> convertible_to<string>;
    T::deserialize(declval<string>());
};

class SerializableClass {
private:
    int value;
    string name;
    
public:
    SerializableClass(int v, const string& n) : value(v), name(n) {}
    
    string serialize() const {
        return to_string(value) + ":" + name;
    }
    
    static SerializableClass deserialize(const string& data) {
        size_t pos = data.find(':');
        if (pos != string::npos) {
            int v = stoi(data.substr(0, pos));
            string n = data.substr(pos + 1);
            return SerializableClass(v, n);
        }
        throw invalid_argument("Invalid serialization format");
    }
    
    friend ostream& operator<<(ostream& os, const SerializableClass& obj) {
        return os << "{" << obj.value << ", " << obj.name << "}";
    }
};

template<Serializable T>
void processSerializable(const T& obj) {
    string serialized = obj.serialize();
    cout << "Serialized: " << serialized << endl;
    
    auto deserialized = T::deserialize(serialized);
    cout << "Deserialized: " << deserialized << endl;
}

template<typename From, typename To>
concept ConvertibleTo = convertible_to<From, To>;

template<typename T, typename U>
requires ConvertibleTo<T, U>
U safeConvert(const T& value) {
    return static_cast<U>(value);
}

template<typename T>
concept DefaultConstructible = default_initializable<T>;

template<DefaultConstructible T>
vector<T> createVector(size_t size) {
    return vector<T>(size);
}

template<typename T>
concept CopyConstructible = copy_constructible<T>;

template<CopyConstructible T>
void duplicateValue(const T& original, T& copy) {
    copy = T(original);
}

auto conceptualFunction(Integral auto x, FloatingPoint auto y) {
    return static_cast<double>(x) + y;
}

template<Numeric T>
class Calculator {
private:
    T accumulator;
    
public:
    Calculator() : accumulator(T{}) {}
    
    void add(T value) requires Addable<T> {
        accumulator = accumulator + value;
    }
    
    void reset() {
        accumulator = T{};
    }
    
    T getResult() const {
        return accumulator;
    }
    
    template<Comparable U = T>
    bool isGreaterThan(U value) const {
        return accumulator > value;
    }
};

int main() {
    cout << "C++20 Concepts and Constraints Demo" << endl;
    cout << "===================================" << endl;
    
    cout << "\n1. Basic Numeric Concepts:" << endl;
    cout << "add(5, 3) = " << add(5, 3) << endl;
    cout << "add(2.5, 1.7) = " << add(2.5, 1.7) << endl;
    
    cout << "\n2. Integral-only Functions:" << endl;
    cout << "factorial(5) = " << factorial(5) << endl;
    cout << "factorial(7) = " << factorial(7) << endl;
    
    cout << "\n3. Floating-point-only Functions:" << endl;
    cout << "circleArea(3.0) = " << circleArea(3.0) << endl;
    cout << "circleArea(5.5f) = " << circleArea(5.5f) << endl;
    
    cout << "\n4. Printable Concept:" << endl;
    safePrint(42);
    safePrint("Hello World");
    safePrint(3.14159);
    
    cout << "\n5. Container Concept:" << endl;
    vector<int> numbers = {1, 2, 3, 4, 5};
    vector<string> words = {"hello", "world", "concepts"};
    
    printContainer(numbers);
    printContainer(words);
    
    cout << "\n6. Constrained Templates with Multiple Requirements:" << endl;
    cout << "Max in numbers: " << findMax(numbers) << endl;
    cout << "Max in words: " << findMax(words) << endl;
    
    cout << "\n7. Iterator Concepts:" << endl;
    auto it = numbers.begin();
    cout << "Before advancing: " << *it << endl;
    advanceIterator(it, 2);
    cout << "After advancing 2 steps: " << *it << endl;
    
    cout << "\n8. Sortable Concept:" << endl;
    vector<int> unsorted = {5, 2, 8, 1, 9, 3};
    cout << "Before sorting: ";
    printContainer(unsorted);
    sortContainer(unsorted);
    cout << "After sorting: ";
    printContainer(unsorted);
    
    cout << "\n9. Custom Serializable Concept:" << endl;
    SerializableClass obj(42, "TestObject");
    processSerializable(obj);
    
    cout << "\n10. Type Conversion with Concepts:" << endl;
    int intValue = 42;
    double doubleValue = safeConvert<int, double>(intValue);
    cout << "Converted " << intValue << " to " << doubleValue << endl;
    
    cout << "\n11. Default Construction Concept:" << endl;
    auto intVector = createVector<int>(5);
    auto stringVector = createVector<string>(3);
    cout << "Created int vector of size: " << intVector.size() << endl;
    cout << "Created string vector of size: " << stringVector.size() << endl;
    
    cout << "\n12. Copy Construction Concept:" << endl;
    string original = "Hello";
    string copy;
    duplicateValue(original, copy);
    cout << "Original: " << original << ", Copy: " << copy << endl;
    
    cout << "\n13. Abbreviated Function Templates:" << endl;
    cout << "conceptualFunction(5, 3.14) = " << conceptualFunction(5, 3.14) << endl;
    
    cout << "\n14. Constrained Class Templates:" << endl;
    Calculator<int> intCalc;
    intCalc.add(10);
    intCalc.add(20);
    cout << "Int calculator result: " << intCalc.getResult() << endl;
    cout << "Is greater than 25? " << intCalc.isGreaterThan(25) << endl;
    
    Calculator<double> doubleCalc;
    doubleCalc.add(3.14);
    doubleCalc.add(2.86);
    cout << "Double calculator result: " << doubleCalc.getResult() << endl;
    
    return 0;
}
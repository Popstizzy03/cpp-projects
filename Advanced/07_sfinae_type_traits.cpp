#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <memory>
using namespace std;

template<typename T>
struct has_size {
private:
    template<typename U>
    static auto test(int) -> decltype(declval<U>().size(), true_type{});
    
    template<typename>
    static false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
constexpr bool has_size_v = has_size<T>::value;

template<typename T>
struct has_push_back {
private:
    template<typename U>
    static auto test(int) -> decltype(declval<U>().push_back(declval<typename U::value_type>()), true_type{});
    
    template<typename>
    static false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
constexpr bool has_push_back_v = has_push_back<T>::value;

template<typename T>
struct has_begin_end {
private:
    template<typename U>
    static auto test(int) -> decltype(
        declval<U>().begin(),
        declval<U>().end(),
        true_type{}
    );
    
    template<typename>
    static false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
constexpr bool has_begin_end_v = has_begin_end<T>::value;

template<typename T>
struct is_smart_pointer : false_type {};

template<typename T>
struct is_smart_pointer<unique_ptr<T>> : true_type {};

template<typename T>
struct is_smart_pointer<shared_ptr<T>> : true_type {};

template<typename T>
struct is_smart_pointer<weak_ptr<T>> : true_type {};

template<typename T>
constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

template<typename T>
enable_if_t<has_size_v<T>, size_t>
getSize(const T& container) {
    return container.size();
}

template<typename T>
enable_if_t<!has_size_v<T>, size_t>
getSize(const T&) {
    return 0;
}

template<typename T>
enable_if_t<has_push_back_v<T>>
addElement(T& container, const typename T::value_type& element) {
    container.push_back(element);
    cout << "Added element using push_back" << endl;
}

template<typename T>
enable_if_t<!has_push_back_v<T> && has_begin_end_v<T>>
addElement(T& container, const typename T::value_type& element) {
    container.insert(container.end(), element);
    cout << "Added element using insert" << endl;
}

template<typename T>
enable_if_t<is_integral_v<T>, T>
processValue(T value) {
    cout << "Processing integral value: " << value << endl;
    return value * 2;
}

template<typename T>
enable_if_t<is_floating_point_v<T>, T>
processValue(T value) {
    cout << "Processing floating-point value: " << value << endl;
    return value * 3.14;
}

template<typename T>
enable_if_t<is_same_v<T, string>, T>
processValue(const T& value) {
    cout << "Processing string value: " << value << endl;
    return value + " (processed)";
}

template<typename T>
struct remove_all_pointers {
    using type = T;
};

template<typename T>
struct remove_all_pointers<T*> {
    using type = typename remove_all_pointers<T>::type;
};

template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using return_type = R;
    static constexpr size_t arity = sizeof...(Args);
    
    template<size_t N>
    struct argument {
        static_assert(N < arity, "Index out of bounds");
        using type = tuple_element_t<N, tuple<Args...>>;
    };
};

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {};

template<typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...)> : function_traits<R(Args...)> {};

template<typename T>
struct is_container {
private:
    template<typename U>
    static auto test(int) -> decltype(
        declval<U>().begin(),
        declval<U>().end(),
        typename U::value_type{},
        true_type{}
    );
    
    template<typename>
    static false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
constexpr bool is_container_v = is_container<T>::value;

template<typename T>
enable_if_t<is_container_v<T>>
printContainer(const T& container) {
    cout << "Container contents: ";
    for (const auto& item : container) {
        cout << item << " ";
    }
    cout << "(size: " << container.size() << ")" << endl;
}

template<typename T>
enable_if_t<!is_container_v<T>>
printContainer(const T& value) {
    cout << "Single value: " << value << endl;
}

template<typename Iter>
using iterator_value_t = typename iterator_traits<Iter>::value_type;

template<typename Iter>
enable_if_t<is_same_v<typename iterator_traits<Iter>::iterator_category, random_access_iterator_tag>, iterator_value_t<Iter>>
getMiddleElement(Iter begin, Iter end) {
    auto distance = end - begin;
    return *(begin + distance / 2);
}

template<typename Iter>
enable_if_t<!is_same_v<typename iterator_traits<Iter>::iterator_category, random_access_iterator_tag>, iterator_value_t<Iter>>
getMiddleElement(Iter begin, Iter end) {
    auto distance = std::distance(begin, end);
    advance(begin, distance / 2);
    return *begin;
}

template<typename T, typename = void>
struct has_custom_to_string : false_type {};

template<typename T>
struct has_custom_to_string<T, void_t<decltype(declval<T>().toString())>> : true_type {};

template<typename T>
constexpr bool has_custom_to_string_v = has_custom_to_string<T>::value;

template<typename T>
enable_if_t<has_custom_to_string_v<T>, string>
stringify(const T& obj) {
    return obj.toString();
}

template<typename T>
enable_if_t<!has_custom_to_string_v<T> && is_arithmetic_v<T>, string>
stringify(const T& value) {
    return to_string(value);
}

template<typename T>
enable_if_t<!has_custom_to_string_v<T> && !is_arithmetic_v<T> && is_same_v<T, string>, string>
stringify(const T& str) {
    return str;
}

class CustomStringifiable {
private:
    int value;
    string name;
    
public:
    CustomStringifiable(int v, const string& n) : value(v), name(n) {}
    
    string toString() const {
        return name + "(" + to_string(value) + ")";
    }
};

template<bool Condition>
using enable_if_bool = enable_if_t<Condition, bool>;

template<typename T>
enable_if_bool<is_pointer_v<T>>
isValidPointer(T ptr) {
    return ptr != nullptr;
}

template<typename T>
enable_if_bool<is_smart_pointer_v<T>>
isValidPointer(const T& ptr) {
    return static_cast<bool>(ptr);
}

template<typename T>
enable_if_bool<!is_pointer_v<T> && !is_smart_pointer_v<T>>
isValidPointer(const T&) {
    return true;
}

int main() {
    cout << "SFINAE and Advanced Type Traits Demo" << endl;
    cout << "====================================" << endl;
    
    cout << "\n1. Custom Type Traits:" << endl;
    cout << "vector<int> has size: " << has_size_v<vector<int>> << endl;
    cout << "int has size: " << has_size_v<int> << endl;
    cout << "vector<int> has push_back: " << has_push_back_v<vector<int>> << endl;
    cout << "string has push_back: " << has_push_back_v<string> << endl;
    cout << "vector<int> has begin/end: " << has_begin_end_v<vector<int>> << endl;
    cout << "int has begin/end: " << has_begin_end_v<int> << endl;
    
    cout << "\n2. Smart Pointer Detection:" << endl;
    cout << "unique_ptr<int> is smart pointer: " << is_smart_pointer_v<unique_ptr<int>> << endl;
    cout << "shared_ptr<string> is smart pointer: " << is_smart_pointer_v<shared_ptr<string>> << endl;
    cout << "int* is smart pointer: " << is_smart_pointer_v<int*> << endl;
    
    cout << "\n3. SFINAE-based Function Overloading:" << endl;
    vector<int> vec = {1, 2, 3, 4, 5};
    string str = "hello";
    int number = 42;
    
    cout << "Size of vector: " << getSize(vec) << endl;
    cout << "Size of string: " << getSize(str) << endl;
    cout << "Size of int: " << getSize(number) << endl;
    
    cout << "\n4. Container Manipulation:" << endl;
    vector<int> numbers;
    addElement(numbers, 10);
    addElement(numbers, 20);
    
    cout << "\n5. Value Processing by Type:" << endl;
    cout << "Result: " << processValue(42) << endl;
    cout << "Result: " << processValue(3.14) << endl;
    cout << "Result: " << processValue(string("Hello")) << endl;
    
    cout << "\n6. Advanced Type Manipulation:" << endl;
    cout << "int*** -> " << typeid(remove_all_pointers_t<int***>).name() << endl;
    cout << "Original: int***, After removal: int" << endl;
    
    cout << "\n7. Function Traits:" << endl;
    using func_type = int(double, string, bool);
    cout << "Function arity: " << function_traits<func_type>::arity << endl;
    cout << "Return type is int: " << is_same_v<function_traits<func_type>::return_type, int> << endl;
    cout << "First argument is double: " << is_same_v<function_traits<func_type>::argument<0>::type, double> << endl;
    
    cout << "\n8. Container Detection and Printing:" << endl;
    vector<string> words = {"hello", "world", "SFINAE"};
    printContainer(words);
    printContainer(42);
    printContainer(string("single string"));
    
    cout << "\n9. Iterator Category-based Optimization:" << endl;
    vector<int> randomAccessVec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << "Middle element (random access): " << getMiddleElement(randomAccessVec.begin(), randomAccessVec.end()) << endl;
    
    cout << "\n10. Custom toString Detection:" << endl;
    CustomStringifiable obj(42, "MyObject");
    cout << "Custom object: " << stringify(obj) << endl;
    cout << "Integer: " << stringify(123) << endl;
    cout << "Double: " << stringify(3.14159) << endl;
    cout << "String: " << stringify(string("direct string")) << endl;
    
    cout << "\n11. Pointer Validation:" << endl;
    int value = 100;
    int* rawPtr = &value;
    int* nullPtr = nullptr;
    auto smartPtr = make_unique<int>(200);
    unique_ptr<int> nullSmartPtr;
    
    cout << "Raw pointer (valid): " << isValidPointer(rawPtr) << endl;
    cout << "Raw pointer (null): " << isValidPointer(nullPtr) << endl;
    cout << "Smart pointer (valid): " << isValidPointer(smartPtr) << endl;
    cout << "Smart pointer (null): " << isValidPointer(nullSmartPtr) << endl;
    cout << "Regular value: " << isValidPointer(value) << endl;
    
    cout << "\n12. Container Type Detection:" << endl;
    cout << "vector<int> is container: " << is_container_v<vector<int>> << endl;
    cout << "string is container: " << is_container_v<string> << endl;
    cout << "int is container: " << is_container_v<int> << endl;
    cout << "array<int,5> is container: " << is_container_v<array<int, 5>> << endl;
    
    return 0;
}
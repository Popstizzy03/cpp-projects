#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

template<typename T>
struct TypeTraits {
    static constexpr bool is_pointer = false;
    static constexpr bool is_const = false;
    static constexpr bool is_reference = false;
    using type = T;
};

template<typename T>
struct TypeTraits<T*> {
    static constexpr bool is_pointer = true;
    static constexpr bool is_const = false;
    static constexpr bool is_reference = false;
    using type = T;
};

template<typename T>
struct TypeTraits<const T> {
    static constexpr bool is_pointer = false;
    static constexpr bool is_const = true;
    static constexpr bool is_reference = false;
    using type = T;
};

template<typename T>
struct TypeTraits<T&> {
    static constexpr bool is_pointer = false;
    static constexpr bool is_const = false;
    static constexpr bool is_reference = true;
    using type = T;
};

template<bool Condition, typename TrueType, typename FalseType>
struct ConditionalType {
    using type = TrueType;
};

template<typename TrueType, typename FalseType>
struct ConditionalType<false, TrueType, FalseType> {
    using type = FalseType;
};

template<typename T>
class SmartContainer {
private:
    using storage_type = typename ConditionalType<
        sizeof(T) <= sizeof(void*) && is_trivially_copyable_v<T>,
        T,
        unique_ptr<T>
    >::type;
    
    storage_type data;
    
public:
    SmartContainer(const T& value) {
        if constexpr (is_same_v<storage_type, T>) {
            data = value;
        } else {
            data = make_unique<T>(value);
        }
    }
    
    const T& get() const {
        if constexpr (is_same_v<storage_type, T>) {
            return data;
        } else {
            return *data;
        }
    }
    
    void set(const T& value) {
        if constexpr (is_same_v<storage_type, T>) {
            data = value;
        } else {
            *data = value;
        }
    }
};

template<size_t... Indices>
struct IndexSequence {};

template<size_t N, size_t... Indices>
struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Indices...> {};

template<size_t... Indices>
struct MakeIndexSequence<0, Indices...> {
    using type = IndexSequence<Indices...>;
};

template<typename Tuple, size_t... Indices>
void printTupleImpl(const Tuple& t, IndexSequence<Indices...>) {
    ((cout << get<Indices>(t) << " "), ...);
}

template<typename... Args>
void printTuple(const tuple<Args...>& t) {
    printTupleImpl(t, typename MakeIndexSequence<sizeof...(Args)>::type{});
    cout << endl;
}

template<typename T, size_t N>
constexpr size_t arraySize(T (&)[N]) {
    return N;
}

template<typename T>
constexpr auto getValue() {
    if constexpr (is_integral_v<T>) {
        return T{42};
    } else if constexpr (is_floating_point_v<T>) {
        return T{3.14};
    } else if constexpr (is_same_v<T, string>) {
        return string{"Hello"};
    } else {
        return T{};
    }
}

template<typename T, T... Values>
struct ValueList {
    static constexpr size_t size = sizeof...(Values);
    
    template<size_t Index>
    static constexpr T get() {
        static_assert(Index < size, "Index out of bounds");
        constexpr T values[] = {Values...};
        return values[Index];
    }
};

using Primes = ValueList<int, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29>;

int main() {
    cout << "Template Metaprogramming Demo" << endl;
    cout << "=============================" << endl;
    
    cout << "\n1. Compile-time Factorial:" << endl;
    cout << "5! = " << Factorial<5>::value << endl;
    cout << "10! = " << Factorial<10>::value << endl;
    
    cout << "\n2. Compile-time Fibonacci:" << endl;
    cout << "fib(10) = " << Fibonacci<10>::value << endl;
    cout << "fib(15) = " << Fibonacci<15>::value << endl;
    
    cout << "\n3. Type Traits Analysis:" << endl;
    cout << "int: pointer=" << TypeTraits<int>::is_pointer 
         << ", const=" << TypeTraits<int>::is_const 
         << ", reference=" << TypeTraits<int>::is_reference << endl;
    
    cout << "int*: pointer=" << TypeTraits<int*>::is_pointer 
         << ", const=" << TypeTraits<int*>::is_const 
         << ", reference=" << TypeTraits<int*>::is_reference << endl;
    
    cout << "const int: pointer=" << TypeTraits<const int>::is_pointer 
         << ", const=" << TypeTraits<const int>::is_const 
         << ", reference=" << TypeTraits<const int>::is_reference << endl;
    
    cout << "int&: pointer=" << TypeTraits<int&>::is_pointer 
         << ", const=" << TypeTraits<int&>::is_const 
         << ", reference=" << TypeTraits<int&>::is_reference << endl;
    
    cout << "\n4. Smart Container (Small Object Optimization):" << endl;
    SmartContainer<int> intContainer(42);
    SmartContainer<string> stringContainer("Hello World");
    SmartContainer<vector<int>> vectorContainer(vector<int>{1, 2, 3, 4, 5});
    
    cout << "Int container: " << intContainer.get() << endl;
    cout << "String container: " << stringContainer.get() << endl;
    cout << "Vector container size: " << vectorContainer.get().size() << endl;
    
    cout << "\n5. Tuple Printing with Index Sequences:" << endl;
    auto myTuple = make_tuple(42, 3.14, "Hello", 'A');
    cout << "Tuple contents: ";
    printTuple(myTuple);
    
    cout << "\n6. Constexpr if with Type Detection:" << endl;
    cout << "getValue<int>(): " << getValue<int>() << endl;
    cout << "getValue<double>(): " << getValue<double>() << endl;
    cout << "getValue<string>(): " << getValue<string>() << endl;
    
    cout << "\n7. Compile-time Value Lists:" << endl;
    cout << "Prime list size: " << Primes::size << endl;
    cout << "First 5 primes: ";
    cout << Primes::get<0>() << " ";
    cout << Primes::get<1>() << " ";
    cout << Primes::get<2>() << " ";
    cout << Primes::get<3>() << " ";
    cout << Primes::get<4>() << endl;
    
    cout << "\n8. Array Size Deduction:" << endl;
    int arr1[] = {1, 2, 3, 4, 5};
    double arr2[] = {1.1, 2.2, 3.3};
    cout << "arr1 size: " << arraySize(arr1) << endl;
    cout << "arr2 size: " << arraySize(arr2) << endl;
    
    cout << "\n9. Performance: Compile-time vs Runtime:" << endl;
    auto start = chrono::high_resolution_clock::now();
    
    constexpr int compileTimeFact = Factorial<12>::value;
    
    auto mid = chrono::high_resolution_clock::now();
    
    int runtimeFact = 1;
    for (int i = 1; i <= 12; ++i) {
        runtimeFact *= i;
    }
    
    auto end = chrono::high_resolution_clock::now();
    
    cout << "Compile-time factorial: " << compileTimeFact << endl;
    cout << "Runtime factorial: " << runtimeFact << endl;
    
    auto compileTime = chrono::duration_cast<chrono::nanoseconds>(mid - start);
    auto runtimeTime = chrono::duration_cast<chrono::nanoseconds>(end - mid);
    
    cout << "Compile-time overhead: " << compileTime.count() << " ns" << endl;
    cout << "Runtime calculation: " << runtimeTime.count() << " ns" << endl;
    
    return 0;
}
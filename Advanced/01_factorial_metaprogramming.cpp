// Advanced/factorial_metaprogramming.cpp
#include <iostream>

using namespace std;

template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial {
    static const int value = 1;
}

int main() {
    constexpr int result = Factorial::value; // Calculated at compile time
    cout << " Factorial of 5: " << result << endl;
    return 0;
}
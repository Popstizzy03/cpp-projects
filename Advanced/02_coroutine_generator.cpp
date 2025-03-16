// Advanced/02_coroutine_generator.cpp
#include <iostream>
#include <coroutine>

using namespace std;

struct Generator {
    struct promise_type {
        int current_value;
        suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
    }
}
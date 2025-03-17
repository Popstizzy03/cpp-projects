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
        suspend_always initial_suspend() { return {}; }
        suspend_always final_suspend() noexcept { return {}; }
        Generator get_return_object() { return Generator{coroutine_handle<promise_type>::from_promise(*this)}; }
        void unhandled_exception() {}
    };
    coroutine_handle<promise_type> handle;
    Generator(coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    int value() { return handle.promise().current_value; }
    bool next() { return handle.resume(), !handle.done(); }
};
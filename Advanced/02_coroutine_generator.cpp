// Advanced/02_coroutine_generator.cpp
#include <iostream>
#include <coroutine>

struct Generator {
    struct promise_type {
        int current_value;
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        Generator get_return_object() { return Generator{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        void unhandled_exception() {}
    };
    std::coroutine_handle<promise_type> handle;
    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    int value() { return handle.promise().current_value; }
    bool next() { return handle.resume(), !handle.done(); }
};

Generator generateNumbers(int count) {
    for (int i = 0; i < count; ++i) {
        co_yield i;
    }
}

int main() {
    auto generator = generateNumbers(5);
    while (generator.next()) {
        std::cout << generator.value() << " ";
    }
    std::cout << std::endl;
    return 0;
}
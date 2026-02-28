#include <iostream>
#include <functional>
#include <vector>
#include <string>

#define TEST(name) void name()
#define RUN_TEST(name) runners.push_back(#name, name)

struct TestCase {
    std::string name;
    std::function<void()> func;
};

std::vector<TestCase> tests;

void add_test(const std::string& name, std::function<void()> func) {
    tests.push_back({name, func});
}

int run_tests() {
    int passed = 0;
    for (const auto& test : tests) {
        std::cout << "Running " << test.name << "... ";
        try {
            test.func();
            std::cout << "PASSED\n";
            passed++;
        } catch (const std::exception& e) {
            std::cout << "FAILED: " << e.what() << "\n";
        } catch (...) {
            std::cout << "FAILED: unknown exception\n";
        }
    }
    std::cout << "\n" << passed << "/" << tests.size() << " tests passed.\n";
    return (passed == tests.size()) ? 0 : 1;
}

#define ASSERT(cond) do { if (!(cond)) throw std::runtime_error("Assertion failed: " #cond); } while(0)

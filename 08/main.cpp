#include <iostream>
#include "ThreadPool.h"
#include <chrono>
#include <cassert>

int main() {
    // test 1 
    {
        struct A {
            int a = 5;
        };
        auto foo = [](const A& p) { return p.a;};

        ThreadPool pool(8);

        auto task1 = pool.exec(foo, A());
        assert(task1.get() == 5);

        auto task2 = pool.exec([]() { return 1; });
        assert(task2.get() == 1);
    }
    // test 2
    {
        auto boo = [](int x) { return x*x; };
        ThreadPool pool(4);
        auto task = pool.exec(boo, 25);
        assert(task.get() == 25*25);
    }
    std::cout << "All tests passed" << std::endl;
    return 0;
}
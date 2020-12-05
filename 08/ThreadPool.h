#pragma once
#include <functional>
#include <vector>
#include <cstdlib>
#include <future>
#include <queue>
#include <thread>
//#include <iostream>

using task = std::function<void ()>;

class ThreadPool {
public:
    ThreadPool(ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    explicit ThreadPool(size_t poolSize): not_done(true) {
        //std::cout << "construct" << std::endl;
        for (size_t i = 0; i < poolSize; ++i) {
        threads.emplace_back([this]() {
            while (not_done) {
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    while (tqueue.empty() && not_done)
                        cond.wait(lock);
                }
                mutex.lock();
                if (!tqueue.empty()) {
                    auto next_task = std::move(tqueue.front());
                    tqueue.pop();
                    mutex.unlock();
                    next_task();
                }
                else {
                    mutex.unlock();
                    continue;
                }
            }
            });
        }  
    }

    ~ThreadPool() {
        //std::cout << "decstruct" << std::endl;
        not_done = false;
        cond.notify_all();
        for (auto& thr : threads) {
            thr.join();
        }
    }
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        //std::cout << "exec" << std::endl;
        using rettype = decltype(func(args...));
        auto new_task = std::make_shared<std::packaged_task<rettype ()>>([func, args...]() {
            return func(args...);
        });
        {
            std::lock_guard<std::mutex> lock(mutex);
            tqueue.emplace([new_task]() { (*new_task)(); });
        }
        cond.notify_one();
        return new_task->get_future();
    }
private:
    std::atomic<bool> not_done;
    std::vector<std::thread> threads;
    std::queue<task> tqueue;
    std::mutex mutex;
    std::condition_variable cond;
};

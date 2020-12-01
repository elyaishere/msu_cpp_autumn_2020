#pragma once
#include <functional>
#include <vector>
#include <cstdlib>
#include <future>
#include <queue>
#include <thread>

using task = std::function<void ()>;

class ThreadPool {
public:
    ThreadPool(ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    explicit ThreadPool(size_t poolSize): not_done(true) {
        for (size_t i = 0; i < poolSize; ++i) {
        threads.emplace_back([this]() {
            while (not_done) {
                std::unique_lock<std::mutex> lock(this->mutex);
                if (!tqueue.empty()) {
                    auto next_task = std::move(tqueue.front());
                    tqueue.pop();
                    lock.unlock();
                    next_task();
                    }
                    else {
                        cond.wait(lock);
                    }
                }
            });
        }  
    }

    ~ThreadPool() {
        not_done = false;
        cond.notify_all();
        for (auto& thr : threads) {
            if (thr.joinable()) {
                thr.join();
            }
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
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
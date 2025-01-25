#include "thread_pool.h"
#include <mutex>
#include <iostream>

extern std::condition_variable cond;
extern bool isRunning;
namespace {
    std::mutex tasks_mutex;
}
// using templates, we don't need tasks to be of specific type. Any type of task can be passed even if not related
// we also won't need the use of unique_ptr.
template <typename T>
void ThreadPool::add_task(T&& task)
{
    std::unique_lock<std::mutex> guard(tasks_mutex);
    tasks.push(std::forward<T>(task)); // preserve original T type as rvalue/lvalue before that the compiler deduced its type
    // need to write examples about std::forward, I understood the purpose anyway.
    guard.unlock();
    cond.notify_one();
}

ThreadPool::ThreadPool(int nb_threads)
{
    for(int i = 0; i < nb_threads; i++) {
        workers.push_back(std::thread([this] () {
            while(isRunning) {
                std::unique_lock<std::mutex> guard(tasks_mutex);
                cond.wait(guard);
                if(!isRunning) return; // ended program
                auto task = std::move(tasks.front());
                tasks.pop();
                task();
            }
        
        }));
    }
}
ThreadPool::~ThreadPool() {
    for(int i = 0; i < nb_threads; i++) {
        workers[i].join();
    }
}

#include "thread_pool/thread_pool.h"

#include <mutex>
#include <iostream>

#include "task/send_response.h"
#include "task/read_request.h"
#include "logging/trace.h"

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
    tasks.push(std::forward<T>(task));
    guard.unlock();
    cond.notify_one();
}

// problem with templates is that if the method definition is not header files u have to do the following
template void ThreadPool::add_task<SendResponse>(SendResponse&&);
template void ThreadPool::add_task<ReadRequest>(ReadRequest&&);

ThreadPool::ThreadPool(int nb_threads)
{
    for(int i = 0; i < nb_threads; i++) {
        workers.push_back(std::thread([this] () {
            while(isRunning) {
                std::unique_lock<std::mutex> guard(tasks_mutex);
                cond.wait(guard);
                if(!isRunning) return; // ended program
                std::function<void()> current_task = std::move(tasks.front());
                tasks.pop();
                current_task();
            }
        
        }));
    }
}

ThreadPool::~ThreadPool() {
    for(int i = 0; i < nb_threads; i++) {
        workers[i].join();
    }
    TRACE_DEBUG("All threads joined");
}

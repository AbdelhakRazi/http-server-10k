#include "thread_pool.h"
#include <mutex>
#include <iostream>

std::mutex tasks_mutex;
extern std::condition_variable cond;
extern bool isRunning;

void ThreadPool::add_task(std::unique_ptr<TcpTask> task)
{
    std::unique_lock<std::mutex> guard(tasks_mutex);
    tasks.push(std::move(task));
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
                std::unique_ptr<TcpTask> task = std::move(tasks.front());
                tasks.pop();
                task->execute();
            }
        
        }));
    }
}
ThreadPool::~ThreadPool() {
    for(int i = 0; i < nb_threads; i++) {
        workers[i].join();
    }
}

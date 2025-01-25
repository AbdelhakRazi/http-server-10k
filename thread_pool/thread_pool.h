#ifndef __THREAD_POOL__H__
#define __THREAD_POOL__H__

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
class ThreadPool {
   std::vector<std::thread> workers;
   std::queue<std::function<void()>> tasks;
   int nb_threads;
   public:
        template <typename T> void add_task(T&& task);
        ThreadPool() = default; // voir ce problème après
        ThreadPool(int nb_threads);
        ~ThreadPool();

};
#endif  //!__THREAD_POOL__H__
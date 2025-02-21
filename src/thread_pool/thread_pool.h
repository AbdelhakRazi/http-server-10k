#ifndef __THREAD_POOL__H__
#define __THREAD_POOL__H__

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <functional>

class Worker;
class ThreadPool {
   std::vector<std::thread> threads;
   std::vector<Worker> workers;
   int current_worker{0};
   int nb_threads;
   public:
        explicit ThreadPool(int nb_threads);
        void add_client(int client_fd);
        ~ThreadPool();

};
#endif  //!__THREAD_POOL__H__
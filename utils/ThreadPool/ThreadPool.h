#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <stdexcept>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t num_threads
               = thread::hardware_concurrency())
    {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(
                            queueMutex);

                        condition.wait(lock, [this] {
                            return !tasks.empty() || stop;
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void enqueue(function<void()> task)
    {
        {
            unique_lock<mutex> lock(queueMutex);
            tasks.emplace(move(task));
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }

        condition.notify_all();

        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    mutex queueMutex;
    condition_variable condition;
    
    vector<thread> threads;
    bool stop;

    queue<function<void()>> tasks;
};

#endif

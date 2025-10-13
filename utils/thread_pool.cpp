#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

/*
The user gives a callable and a list of arguments.
Those are bind into a single functor.
We will make a new function with that functor (so it doesn't die when
the scope ends) and we will put its pointer in the queue.
Any thread grabs that pointer and calls it.
*/

class ThreadPool {
    using ull = unsigned long long;
    vector<pthread_t> threads;
    
    queue<pair<void*, ull>> q;
    pthread_mutex_t queueMutex;
    sem_t pendingTasks;
    pthread_mutex_t shutdownMutex;
    bool shutdown = false;
    pthread_mutex_t setMutex;
    set<int> pendingTaskIds;
    ull curId = 0;
    
    void runNext() {
        pthread_mutex_lock(&queueMutex);
        function<void()>* f = static_cast<function<void()>*>(q.front().first);
        unsigned long long taskid = q.front().second;
        q.pop();
        pthread_mutex_unlock(&queueMutex);

        (*f)();
        delete f;

        pthread_mutex_lock(&setMutex);
        pendingTaskIds.erase(taskid);
        pthread_mutex_unlock(&setMutex);
    }

public:
    ThreadPool(int n) {
        // 0 pending tasks
        sem_init(&pendingTasks, 0, 0);
        pthread_mutex_init(&queueMutex, nullptr);
        pthread_mutex_init(&shutdownMutex, nullptr);
        pthread_mutex_init(&setMutex, nullptr);

        threads.resize(n);
        for (int i = 0; i < n; i++)
            pthread_create(&threads[i], nullptr, pthreadTakeTask, this);
    }

    ~ThreadPool() {
        pthread_mutex_lock(&shutdownMutex);
        shutdown = true;
        pthread_mutex_unlock(&shutdownMutex);
        for (int i = 0; i < threads.size(); i++)
            sem_post(&pendingTasks);
        for (int i = 0; i < threads.size(); i++)
            pthread_join(threads[i], nullptr);
        while (!q.empty()) {
            function<void()>* f = static_cast<function<void()>*>(q.front().first);
            q.pop();
            delete f;
        }
        sem_destroy(&pendingTasks);
        pthread_mutex_destroy(&queueMutex);
        pthread_mutex_destroy(&shutdownMutex);
        pthread_mutex_destroy(&setMutex);
    }

    static void* pthreadTakeTask(void *arg) {
        ThreadPool* t = static_cast<ThreadPool*>(arg);
        while (1) {
            sem_wait(&t->pendingTasks);
            pthread_mutex_lock(&t->shutdownMutex);
            if (t->shutdown) break;
            pthread_mutex_unlock(&t->shutdownMutex);
            t->runNext();
        }
        pthread_mutex_unlock(&t->shutdownMutex);
        return nullptr;
    }

    template<typename Callable, typename... Args>
    int execute(Callable c, Args&&... fargs) {
        auto bound = std::bind(c, fargs...);
        // converts from bound (functor) to std::function
        auto* cptr = new std::function<void()>(bound);
        pthread_mutex_lock(&queueMutex);
        int taskid = curId++;
        
        pthread_mutex_lock(&setMutex);
        pendingTaskIds.insert(taskid);
        pthread_mutex_unlock(&setMutex);

        q.push({static_cast<void*>(cptr), taskid});
        pthread_mutex_unlock(&queueMutex);
        sem_post(&pendingTasks);
        return taskid;
    }

    void join(int taskid) {
        while (1) {
            pthread_mutex_lock(&setMutex);
            if (!pendingTaskIds.count(taskid)) break;
            pthread_mutex_unlock(&setMutex);
        }
        pthread_mutex_unlock(&setMutex);
    }
};
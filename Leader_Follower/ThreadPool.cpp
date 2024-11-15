#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void enqueueTask(const std::function<void()>& task);
    void stop();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stopFlag;

    void workerThread(int id);
};

ThreadPool::ThreadPool(int numThreads) : stopFlag(false) {
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this, i);
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::enqueueTask(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    condition.notify_all();

    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerThread(int id) {
    std::function<void()> task;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !tasks.empty() || stopFlag; });

            if (stopFlag && tasks.empty()) {
                return;
            }

            task = tasks.front();
            tasks.pop();
        }

        // Leader part
        std::cout << "Thread " << id << " is now the leader.\n";
        task();  // Execute the task

        // Rotate the leader role
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Simulate task execution time
    }
}

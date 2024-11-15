#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

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
    int currentLeader;

    void workerThread(int id);
};

#endif // THREADPOOL_HPP

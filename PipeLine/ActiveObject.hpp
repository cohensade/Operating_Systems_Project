#ifndef ACTIVE_OBJECT_HPP
#define ACTIVE_OBJECT_HPP

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

class ActiveObject {
public:
    ActiveObject();
    ~ActiveObject();

    void start();
    void stop();
    void enqueue(std::function<void()> task);

private:
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread workerThread;
    std::atomic<bool> running;
};

#endif // ACTIVE_OBJECT_HPP

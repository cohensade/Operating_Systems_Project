#include "ActiveObject.hpp"

ActiveObject::ActiveObject() : running(false) {}

ActiveObject::~ActiveObject() {
    stop();
}

void ActiveObject::start() {
    running = true;
    workerThread = std::thread([this]() {
        while (running) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return !tasks.empty() || !running; });
                if (!running && tasks.empty()) break;
                task = std::move(tasks.front());
                tasks.pop();
            }
            if (task) task();
        }
    });
}

void ActiveObject::stop() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        cv.notify_all();
    }
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ActiveObject::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        cv.notify_one();
    }
}

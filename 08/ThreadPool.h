#ifndef INC_08_THREADPOOL_H
#define INC_08_THREADPOOL_H
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <queue>

class ThreadPool {
    using Task = std::function<void ()>;
public:
    explicit ThreadPool(size_t poolSize) {
        alive = true;
        size = poolSize;
        for (size_t i = 0; i < poolSize; i++) {
            threads.emplace_back(
                    [this](){
                        while (alive) {
                            std::unique_lock <std::mutex> lock(m);
                            if (not taskQueue.empty()) {
                                auto task(std::move(taskQueue.front()));
                                taskQueue.pop();
                                lock.unlock();
                                task();
                            } else {
                                while (alive) {
                                    newTask.wait(lock);
                                    if (not taskQueue.empty()) {
                                        break;
                                    }
                                }
                            }
                        }
                    });
        }
    }

    ~ThreadPool() {
        if (alive) {
            alive = false;
            newTask.notify_all();
            for (int i = 0; i < size; i++) {
                threads[i].join();
            }
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        auto promise = new std::promise<decltype(func(args...))>();
        auto future = promise->get_future();

        auto task = [this](std::promise<decltype(func(args...))> *promise, Func func, Args... args) {
            makeTask(promise, func, args...);
        };

        Task t = std::bind(task, promise, func, args...);
        m.lock();
        taskQueue.push(t);
        m.unlock();

        newTask.notify_one();
        return future;
    }


private:

    template <class Promise_ptr, class Func, class... Args>
    void makeTask(Promise_ptr task, Func func, Args... args) {
        task->set_value(func(args...));
    }

    template <class Func, class... Args>
    void makeTask(std::promise<void> *task, Func func, Args... args) {
        task->set_value();
        func(args...);
    }

    std::queue<Task> taskQueue;
    std::condition_variable newTask;
    std::mutex m;
    std::vector<std::thread> threads;
    std::atomic<bool> alive;
    size_t size;
};

#endif //INC_08_THREADPOOL_H

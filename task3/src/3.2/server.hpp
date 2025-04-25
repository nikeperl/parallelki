#pragma once
#include <thread>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <functional>

template<typename T>
class Server {
public:
    using Task = std::function<T()>;

    Server() : running(false), task_id_counter(0) {}

    void start() {
        running = true;
        worker = std::thread(&Server::process_tasks, this);
    }

    void stop() {
        {
            std::unique_lock lock(queue_mutex);
            running = false;
        }
        queue_cv.notify_all();
        if (worker.joinable()) worker.join();
    }

    size_t add_task(Task task) {
        std::unique_lock lock(queue_mutex);
        size_t id = task_id_counter++;
        std::promise<T> prom;
        results[id] = prom.get_future();
        task_queue.emplace(id, std::move(task), std::move(prom));
        queue_cv.notify_one();
        return id;
    }

    T request_result(size_t id) {
        std::shared_future<T> fut;
        {
            std::unique_lock lock(queue_mutex);
            if (results.find(id) == results.end())
                throw std::runtime_error("No such result id.");
            fut = results[id];
        }
        return fut.get();  // блокирующий
    }

private:
    struct TaskEntry {
        size_t id;
        Task task;
        std::promise<T> promise;
        TaskEntry(size_t i, Task&& t, std::promise<T>&& p)
            : id(i), task(std::move(t)), promise(std::move(p)) {}
    };

    std::queue<TaskEntry> task_queue;
    std::unordered_map<size_t, std::shared_future<T>> results;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    std::atomic<bool> running;
    std::atomic<size_t> task_id_counter;
    std::thread worker;

    void process_tasks() {
        while (running || !task_queue.empty()) {
            TaskEntry entry(0, nullptr, std::promise<T>());
            {
                std::unique_lock lock(queue_mutex);
                queue_cv.wait(lock, [&]() {
                    return !task_queue.empty() || !running;
                });
                if (!task_queue.empty()) {
                    entry = std::move(task_queue.front());
                    task_queue.pop();
                } else {
                    continue;
                }
            }

            try {
                T result = entry.task();
                entry.promise.set_value(result);
            } catch (...) {
                entry.promise.set_exception(std::current_exception());
            }
        }
    }
};

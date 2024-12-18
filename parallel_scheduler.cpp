#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

class parallel_scheduler {
public:
    using Task = std::function<void(void*)>;

    // Constructor: takes max threads as capacity
    explicit parallel_scheduler(size_t capacity) : stop(false) {
        for (size_t i = 0; i < capacity; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    Task task;
                    void* arg;

                    // Lock and get a task
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this]() { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) return;

                        task = tasks.front().first;
                        arg = tasks.front().second;
                        tasks.pop();
                    }

                    // Execute the task
                    task(arg);
                }
                });
        }
    }

    // Submit a task to the thread pool
    void run(Task func, void* arg) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(func, arg);
        }
        condition.notify_one();
    }

    // Destructor to stop threads
    ~parallel_scheduler() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            if (worker.joinable()) worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::pair<Task, void*>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#include "thread.hpp"

//private:
void Worker_thread::thread_func()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return stop_ || running_; });

        if (stop_) break;

        task_();

        running_ = false;
        cv_.notify_all();
    }
}

//public:
Worker_thread::Worker_thread(std::function<void()> task)
    : task_(std::move(task)) 
{
    worker_ = std::thread(&Worker_thread::thread_func, this);
}

Worker_thread::~Worker_thread()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        stop_ = true;
    }

    cv_.notify_all();
    if (worker_.joinable())
        worker_.join();
}


void Worker_thread::wait_go(std::function<void()> task)
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return !running_; });

    task();

    running_ = true;
    cv_.notify_all();
}
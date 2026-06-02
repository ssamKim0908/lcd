#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

class Worker_thread
{
public:
    Worker_thread(std::function<void()> task);
    ~Worker_thread();

    Worker_thread(const Worker_thread&)            = delete;
    Worker_thread& operator=(const Worker_thread&) = delete;

    void wait_go(std::function<void()> task);
    
private:
    void thread_func();

private:

    std::thread                worker_;
    std::mutex                 mtx_;
    std::condition_variable    cv_;
    std::function<void()>      task_;

    bool                       running_ = false;
    bool                       stop_    = false;
};
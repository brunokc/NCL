#pragma once

#include <array>
#include <functional>
#include <mutex>
#include <memory>

#include <Threading/Event.h>

namespace WCL::Threading::Tasks {

enum class TaskStatus
{
    Created = 0,
    WaitingForActivation = 1,
    WaitingToRun = 2,
    Running = 3,
    WaitingForChildrenToComplete = 4,
    RanToCompletion = 5,
    Cancelled = 6,
};

class Task : public std::enable_shared_from_this<Task>
{
public:
    template<class T>
    static std::shared_ptr<Task> Run(T&& task)
    {
        auto t = std::make_shared<Task>(task);
        t->Start();
        return t;
    }

    //static void WaitAny(const Task* tasks, int count);
    
    template<typename... Args>
    static int WaitAny(uint32_t timeout, const std::shared_ptr<Task>& t1, const Args&... args)
    {
        HANDLE handle[] = { t1->GetCompletionEvent(), args->GetCompletionEvent()... };
        return WaitHandle::WaitAny(timeout, handle, static_cast<DWORD>(std::size(handle)));
    }

    //static void WaitAll(uint32_t timeout, const Task* tasks, int count);

    template<typename... Args>
    static int WaitAll(uint32_t timeout, const std::shared_ptr<Task>& t1, const Args&... args)
    {
        HANDLE handle[] = { t1->GetCompletionEvent(), args->GetCompletionEvent()... };
        return WaitHandle::WaitAll(timeout, handle, static_cast<DWORD>(std::size(handle)));
    }

    template<class T>
    Task(T&& task) :
        _taskRoutine(task)
    {
    }

    bool IsCompleted();

    void Start();
    void Wait(int timeout = INFINITE);

private:
    HANDLE GetCompletionEvent() const;

    //static void ThreadProc(void* context);
    void TaskProc();

private:
    std::mutex _mutex;
    TaskStatus _status = TaskStatus::Created;
    ManualResetEvent _completedEvent;
    std::function<void()> _taskRoutine;
};

} // namespace WCL::Threading

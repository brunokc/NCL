#include "Task.h"
#include <Threading/Threading.h>

using namespace WCL::Threading::Tasks;

// static 
//void Task::WaitAny(const Task* tasks, int count)
//{
//    std::vector<HANDLE> completionEvents(count);
//    for (int i = 0; i < count; ++i)
//    {
//        completionEvents[i] = tasks[i].GetCompletionEvent();
//    }
//
//    WaitHandle::WaitAny(completionEvents.data(), count);
//}

// static 
//void Task::WaitAll(const Task* tasks, int count, uint32_t timeout)
//{
//    std::vector<HANDLE> completionEvents(count);
//    for (int i = 0; i < count; ++i)
//    {
//        completionEvents[i] = tasks[i].GetCompletionEvent();
//    }
//
//    WaitHandle::WaitAll(completionEvents.data(), count);
//}

bool Task::IsCompleted()
{
    std::lock_guard lock(_mutex);
    return _status == TaskStatus::RanToCompletion;
}

void Task::Start()
{
    {
        std::lock_guard lock(_mutex);
        _status = TaskStatus::WaitingForActivation;
    }

    auto weakThis = weak_from_this();
    RunOnThreadpool([weakThis]() { 
        auto strongThis = weakThis.lock();
        if (strongThis)
        {
            strongThis->TaskProc();
        }
    });
}

void Task::Wait(int timeout)
{
    _completedEvent.Wait(timeout);
}

HANDLE Task::GetCompletionEvent() const
{
    return _completedEvent.get();
}

//// static 
//void Task::ThreadProc(void* context)
//{
//    auto task = static_cast<Task*>(context);
//    task->TaskProc();
//}

void Task::TaskProc()
{
    {
        std::lock_guard lock(_mutex);
        _status = TaskStatus::Running;
    }

    _taskRoutine();

    {
        std::lock_guard lock(_mutex);
        _status = TaskStatus::RanToCompletion;
        _completedEvent.Set();
    }
}

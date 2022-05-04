
#include <windows.h>
//#include <stdio.h>
//#include <vector>
//#include <string>
#include <atomic>

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <Threading/Event.h>
#include <Threading/Tasks/Task.h>

using namespace WCL::Threading;
using namespace WCL::Threading::Tasks;

TEST_CASE("Execute a task in the background", "[task]") 
{
    int signal = 0;
    auto task = std::make_shared<Task>([&signal]() {
        Sleep(250);
        signal = 1;
    });
    REQUIRE(task != nullptr);

    REQUIRE(signal == 0);
    DWORD start = ::GetTickCount();
    task->Start();
    task->Wait(1000);
    DWORD end = ::GetTickCount();
    REQUIRE(signal == 1);
    REQUIRE((end - start) < 1000);
}

TEST_CASE("Wait on all of tasks", "[task]")
{
    std::atomic<int> count = 0;
    auto code = [&count](int timeout) {
        Sleep(timeout);
        count++;
    };

    auto task1 = std::make_shared<Task>(std::bind(code, 100));
    auto task2 = std::make_shared<Task>(std::bind(code, 200));
    auto task3 = std::make_shared<Task>(std::bind(code, 300));

    REQUIRE(count == 0);
    DWORD start = ::GetTickCount();
    task1->Start();
    task2->Start();
    task3->Start();
    Task::WaitAll(1000, task1, task2, task3);
    DWORD elapsedTime = ::GetTickCount() - start;
    REQUIRE(count == 3);
    // Allow for some variance and imprecision in the Sleep times above
    REQUIRE(elapsedTime >= 290);
    REQUIRE(elapsedTime < 1000);
}

TEST_CASE("Wait on any of the tasks", "[task]")
{
    std::atomic<int> sentinel = 0;
    auto code = [&sentinel](int value, int timeout) {
        Sleep(timeout);
        sentinel = value;
    };

    auto task1 = std::make_shared<Task>(std::bind(code, 10, 100));
    auto task2 = std::make_shared<Task>(std::bind(code, 20, 200));
    auto task3 = std::make_shared<Task>(std::bind(code, 30, 300));

    REQUIRE(sentinel == 0);
    DWORD start = ::GetTickCount();
    task1->Start();
    task2->Start();
    task3->Start();
    Task::WaitAny(1000, task1, task2, task3);
    DWORD elapsedTime = ::GetTickCount() - start;
    REQUIRE(sentinel == 10);
    // Allow for some variance and imprecision in the Sleep times above
    REQUIRE(elapsedTime >= 90);
    REQUIRE(elapsedTime < 200);
}


#include <windows.h>
#include <stdio.h>
#include <vector>
#include <random>

#include <Threading/Event.h>

using namespace WFx::Threading;

DWORD
WINAPI
ThreadProc(
    _In_ PVOID context
    )
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 4);

    auto events = static_cast<std::vector<AutoResetEvent*>*>(context);
    for(auto event : *events)
    {
        int sleepTime = distribution(generator);
        Sleep(sleepTime * 1000);

        wprintf(L"Setting handle %#p\n", event->Get());
        SetEvent(event->Get());
        Sleep(10);
    }

    return 0;
}

int __cdecl main(int, wchar_t*[])
{
    AutoResetEvent e1;
    AutoResetEvent e2;
    AutoResetEvent e3;

    wprintf(L"Queueing work item...\n");
    std::vector<AutoResetEvent*> events;// = { &e1, &e2, &e3 };
    events.push_back(&e1);
    events.push_back(&e2);
    events.push_back(&e3);
    QueueUserWorkItem(ThreadProc, &events, WT_EXECUTEDEFAULT);

    wprintf(L"Waiting...\n");
    // bool waited = WaitHandle::WaitAll(h1, h2, h3);
    bool waited = WaitHandle::WaitAll(7500, e1, e2, e3);
    wprintf(L"Waited = %s.\n", (waited ? L"yes" : L"no"));
    Sleep(100);
}

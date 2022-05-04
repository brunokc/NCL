add_library(wcl_threading OBJECT 
    Event.h
    EventWaitHandle.cpp 
    EventWaitHandle.h
    Tasks/Task.cpp
    Tasks/Task.h
    Threading.h
    WaitHandle.cpp
    WaitHandle.h
 )

target_link_libraries(wcl_threading PRIVATE 
    WIL::WIL
)

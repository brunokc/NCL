add_library(wcl_threading OBJECT 
    EventWaitHandle.cpp 
    WaitHandle.cpp
)

target_link_libraries(wcl_threading PRIVATE 
    WIL::WIL
)

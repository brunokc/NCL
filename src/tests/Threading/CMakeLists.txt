add_executable(WaitHandleTests 
    WaitHandleTests.cpp
)

target_link_libraries(WaitHandleTests PRIVATE
    WCL
)

add_executable(TaskTests 
    TaskTests.cpp
)

target_link_libraries(TaskTests PRIVATE
    WCL
    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(TaskTests)

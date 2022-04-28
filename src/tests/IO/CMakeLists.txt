add_executable(IOTests 
    MemoryMappedFileTests.cpp
)

target_link_libraries(IOTests PRIVATE 
    WCL 
    Catch2::Catch2
)

configure_file(test_memory_map_file.bin test_memory_map_file.bin COPYONLY)

include(CTest)
include(Catch)
catch_discover_tests(IOTests)

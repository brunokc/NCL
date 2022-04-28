add_executable(SecurityTests 
    HashTests.cpp
)

target_link_libraries(SecurityTests PRIVATE 
    WCL 
    Catch2::Catch2
)

configure_file(sample-2mb-text-file.txt sample-2mb-text-file.txt COPYONLY)

include(CTest)
include(Catch)
catch_discover_tests(SecurityTests)

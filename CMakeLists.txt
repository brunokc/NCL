cmake_minimum_required(VERSION 3.15)
project(WCL VERSION 1.0)

include(GNUInstallDirs)

option(WCL_BUILD_TESTS "Sets option to build the unit tests, default on" ON)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/nologo /W4 /WX /permissive-)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_compile_definitions(UNICODE _UNICODE NOMINMAX)

add_subdirectory(src)

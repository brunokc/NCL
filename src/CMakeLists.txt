configure_file(WCLConfig.h.in WCLConfig.h)
#target_include_directories(Tutorial PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           )

include(FetchContent)

FetchContent_Declare(
	wil
	GIT_REPOSITORY https://github.com/microsoft/wil.git
	GIT_TAG f9284c19c9873664978b873b8858d7dfacc6af1e # Dec 11, 2021
)

set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Disable WIL packaging")
set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Disable WIL tests")

FetchContent_MakeAvailable(wil)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(commandline)
add_subdirectory(io)
add_subdirectory(security)
add_subdirectory(text)
add_subdirectory(threading)
add_subdirectory(tests)

# Add source to this project's executable.
add_library(WCL STATIC 
    Environment.h
    Environment.cpp
    WCL.cpp
    WCL.h
 )
add_library(${PROJECT_NAME}::WCL ALIAS WCL)
target_compile_features(WCL PUBLIC cxx_std_20)

# Add a public dependency on WIL so that anybody that pulls in our library will also pull in WIL
target_link_libraries(WCL PUBLIC WIL::WIL)

target_link_libraries(WCL PRIVATE 
    wcl_commandline 
    wcl_io 
    wcl_cryptography
    wcl_text
    wcl_threading
)

# The interface's include directory.
target_include_directories(WCL PUBLIC 
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.h")
install(TARGETS WCL 
    EXPORT WCLTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADER_FILES} DESTINATION include)

# Install Package Configuration
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

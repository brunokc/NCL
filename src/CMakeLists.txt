configure_file(WCLConfig.h.in WCLConfig.h)
#target_include_directories(Tutorial PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           )

include_directories(. ${WIL_SOURCE_DIR}/include)

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

include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
	GIT_TAG			v2.13.9
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

enable_testing()

add_subdirectory(security)
add_subdirectory(threading)

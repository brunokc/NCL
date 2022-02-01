include(FetchContent)

FetchContent_Declare(
	wil
	GIT_REPOSITORY https://github.com/microsoft/wil.git
	GIT_TAG f9284c19c9873664978b873b8858d7dfacc6af1e # Dec 11, 2021
)

set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Disable WIL packaging")
set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Disable WIL tests")
FetchContent_MakeAvailable(wil)

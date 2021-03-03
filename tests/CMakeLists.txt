add_executable(tests test_calculator.cpp)

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(tests PRIVATE gmock_main ${PROJECT_NAME}_lib)

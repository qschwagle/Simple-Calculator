cmake_minimum_required(VERSION 3.14)
project(simple-calculator)


set(FILES
	src/Calculator.cpp
	src/Tokenizer.cpp
)

add_library(${PROJECT_NAME}_lib ${FILES})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)


SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
SET_PROPERTY(TARGET ${PROJECT_NAME}_lib PROPERTY CXX_STANDARD 17)

add_subdirectory(tests)

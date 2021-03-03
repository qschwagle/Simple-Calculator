cmake_minimum_required(VERSION 3.14)
project(simple-calculator)


set(FILES
	src/Calculator.cpp
	src/Tokenizer.cpp
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${FILES})


SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

FLAGS=-std=c++17 -Wall

LIB=lib.cpp

build:
	clang++ $(FLAGS) $(LIB) main.cpp -o calculator

test:
	clang++ $(FLAGS) -D TEST $(LIB) tests.cpp test.cpp -o test

.Phony: clean

clean:
	rm calculator
	rm test

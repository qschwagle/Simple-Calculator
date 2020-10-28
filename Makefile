FLAGS=-std=c++17 -Wall


build:
	clang++ -g $(FLAGS) src/Calculator.cpp src/Tokenizer.cpp src/main.cpp -o calculator

test:
	clang++ $(FLAGS) -D TEST $(LIB) tests.cpp test.cpp -o test

.Phony: clean

clean:
	rm calculator
	rm test

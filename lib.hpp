#pragma once

#include<iostream>
#include<string>
#include<unordered_map>
#include<memory>
#include<vector>

#ifdef TEST
#include "tests.hpp"
#endif


enum TokenType {
    number,
    op,
    param,
    none,
};

class Token {
    public:
        Token(std::string raw) : raw_{raw} {}
        const std::string& get_raw(void) const { return raw_; }
        virtual TokenType get_type() const { return none; }

        void print() {
            std::cout << raw_;
        }
        virtual ~Token() {

        }
    private:
        std::string raw_;
};

class Number : public Token {
    public:
        Number(std::string raw) : Token{raw} {}
        TokenType get_type() const override { return number; }
};

enum Associativity {
    left, right
};

class Operator : public Token {
    public:
        Operator(std::string raw) : Token{raw} {
            switch(raw[0]) {
                case '+':
                    precedence_ = 1;
                    break;
                case '-':
                    precedence_ = 1;
                    break;
                case '/':
                    precedence_ = 2;
                    break;
                case '*':
                    precedence_ = 2;
                    break;
            }
        }
        TokenType get_type() const override { return op; }
        int get_precedence() const { return precedence_; }
        Associativity get_assoc() const { return left; } 
    private:
        int precedence_;
};

class Param : public Token {
    public:
        Param(std::string raw) : Token{raw} {}
        TokenType get_type() const override { return param; }
};

class Tokenizer {
    public:
        Tokenizer(std::string& raw) : buffer_{raw}, pos_{0} {}
        Tokenizer()=delete;
        Tokenizer(const Tokenizer&)=delete;
        Tokenizer& operator=(const Tokenizer&)=delete;
        void ignore_white(void);
        std::unique_ptr<Number> number(void);
        std::unique_ptr<Operator> op(void);
        std::unique_ptr<Token> get_next_token(void);
        std::unique_ptr<Param> param(void);
        int get_pos() const { return pos_; }

    private:
        void set_pos(unsigned int pos) { pos_ = pos; }
        std::string buffer_;
        unsigned int pos_;

#ifdef TEST
    friend void test_space(void);
    friend void test_number(void);
    friend void test_op(void);
#endif 
};

class Calculator {
public:
    Calculator()=default;
    Calculator& operator=(const Calculator&)=delete;
    Calculator(const Calculator&)=delete;

    std::string evaluate(std::string expression);
private:
};

int evaluate(std::vector<std::unique_ptr<Token>>& list, int& pos);

/* vim:set sw=4 ts=4 et: */

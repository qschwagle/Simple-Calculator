#pragma once

#include<memory>

#include"Number.hpp"
#include"Operator.hpp"
#include"Token.hpp"
#include"Param.hpp"

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


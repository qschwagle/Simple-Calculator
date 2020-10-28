#pragma once

#include<iostream>
#include<string>
#include<unordered_map>
#include<memory>
#include<vector>

#ifdef TEST
#include "tests.hpp"
#endif


class Operator;
class Param;
class Number;
class Token;


class Visitor {
public:
    virtual void set_op(Operator* op)=0;
    virtual void set_number(Number *num)=0;
    virtual void set_param(Param *param)=0;
    virtual void set_token(Token *token)=0;
};


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

        virtual void pair_visit(Visitor* visit) {
            visit->set_token(this);
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

        void pair_visit(Visitor* visit) override {
            visit->set_number(this);
        }

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

        void pair_visit(Visitor* visit) override {
            visit->set_op(this);
        }



    private:
        int precedence_;
};

class Param : public Token {
    public:
        Param(std::string raw) : Token{raw} {}
        TokenType get_type() const override { return param; }

        void pair_visit(Visitor* visit) override {
            visit->set_param(this);
        }
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

double evaluate(std::vector<std::unique_ptr<Token>>& list, int& pos);

class PairVisitor : public Visitor {
public:
    PairVisitor()=default;
    PairVisitor(const PairVisitor&)=delete;
    PairVisitor& operator=(const PairVisitor&)=delete;

    void set_op(Operator* op) override {
        if(op_one_ == nullptr) {
            op_one_ = op;
        } else {
            op_two_ = op;
        }
    }

    void set_number(Number *num) override {
        if(num_one_ == nullptr) {
            num_one_ = num;
        } else {
            num_two_ = num;
        }
    }

    void set_param(Param *param) override {
        if(param_one_ == nullptr) {
            param_one_ = param;
        } else {
            param_two_ = param;
        }
    }

    void set_token(Token *token) override {
        std::cerr << "ERROR: PAIR VISITOR RECIEVED A STRAIGHT TOKEN. NOT EXPECTED" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<Token> enter_token(std::unique_ptr<Token> t) {
        t->pair_visit(this);
        return t;
    }

    bool first_is_number(void) {
        return num_one_ != nullptr;
    }

    bool first_is_op(void) {
        return op_one_ != nullptr;
    }

    bool first_is_param(void) {
        return param_one_ != nullptr;
    }

    Associativity first_op_assoc() {
        return op_one_->get_assoc();
    }

    int comp_op_lesser_prec() {
        return op_one_->get_precedence() < op_two_->get_precedence();
    }

    int comp_op_equal_prec() {
        return (op_one_->get_precedence() == op_two_->get_precedence());
    }

    bool second_is_op(void) {
        return op_two_ != nullptr;
    }


private:
    Operator* op_one_{nullptr};
    Operator* op_two_{nullptr};
    Number* num_one_{nullptr};
    Number* num_two_{nullptr};
    Param* param_one_{nullptr};
    Param* param_two_{nullptr};
};


/* vim:set sw=4 ts=4 et: */

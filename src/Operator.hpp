#pragma once

#include "Token.hpp"

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


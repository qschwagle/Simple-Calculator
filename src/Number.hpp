#pragma once

#include "Token.hpp"

class Number : public Token {
    public:
        Number(std::string raw) : Token{raw} {}
        TokenType get_type() const override { return number; }

        void pair_visit(Visitor* visit) override {
            visit->set_number(this);
        }

};


#pragma once

#include "Token.hpp"

class Param : public Token {
    public:
        Param(std::string raw) : Token{raw} {}
        TokenType get_type() const override { return param; }

        void pair_visit(Visitor* visit) override {
            visit->set_param(this);
        }
};


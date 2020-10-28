#pragma once

#include<iostream>
#include "Visitor.hpp"

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


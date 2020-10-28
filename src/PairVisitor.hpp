#pragma once

#include "Visitor.hpp"


#include "Token.hpp"
#include "Operator.hpp"
#include "Number.hpp"
#include "Param.hpp"

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

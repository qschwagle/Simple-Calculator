#include "lib.hpp"
#include "exception"

void Tokenizer::ignore_white(void)
{
    for(;buffer_.size() > pos_ && (buffer_[pos_] == ' ' || buffer_[pos_] == '\r' || buffer_[pos_] == '\n'); ++pos_);
}

std::unique_ptr<Number> Tokenizer::number(void)
{
    unsigned int pos = pos_;
    for(;buffer_.size() > pos && 
            (
             buffer_[pos] == '0' || 
             buffer_[pos] == '1' || 
             buffer_[pos] == '2' || 
             buffer_[pos] == '3' || 
             buffer_[pos] == '4' || 
             buffer_[pos] == '5' || 
             buffer_[pos] == '6' || 
             buffer_[pos] == '7' || 
             buffer_[pos] == '8' || 
             buffer_[pos] == '9'
             ); ++pos);
    if(pos == pos_) { 
        return std::unique_ptr<Number>();
    }
    else  {
        auto res =  std::make_unique<Number>(buffer_.substr(pos_, pos-pos_));
        pos_ = pos;
        return res;
    }
}

std::unique_ptr<Operator> Tokenizer::op(void)
{
    if(buffer_.size() > pos_ && 
           (buffer_[pos_] == '+' ||
            buffer_[pos_] == '-' ||
            buffer_[pos_] == '*' ||
            buffer_[pos_] == '/')) {
        ++pos_;
        return std::make_unique<Operator>(std::string(1, buffer_[pos_-1]));
    } else {
        return nullptr;
    }
}

std::unique_ptr<Param> Tokenizer::param(void)
{
    if(buffer_.size() > pos_ && (
            buffer_[pos_] == ')' ||
            buffer_[pos_] == '(')) {
        ++pos_;
        return std::make_unique<Param>(std::string(1, buffer_[pos_-1]));
    } else {
        return nullptr;
    }
}

std::unique_ptr<Token> Tokenizer::get_next_token(void)
{
    std::unique_ptr<Token> res = nullptr;
    ignore_white();
    ((res = op()) || (res = number()) || (res = param()));
    return res;
}

int evaluate(std::vector<std::unique_ptr<Token>>& list, int& pos) {
    auto& last = list[pos];
    --pos;
    if(last->get_type() == number) {
        return std::stoi(last->get_raw());
    } else if(last->get_type() == op) {
        int val = evaluate(list, pos);

        switch(last->get_raw()[0]) {
            case '-':
                val -= evaluate(list, pos);
                break;
            case '+':
                val += evaluate(list, pos);
                break;
            case '*':
                val *= evaluate(list, pos);
                break;
            case '/':
                val /= evaluate(list, pos);
                break;
        } 
        return val;
    }
    return 0;
}

// Two Tokens must be compared together
// Token A is type Ca
// Token B is type Cb
// based on the types and zero or more parameterized values, an action must be taken and a modification done/result returned



/** Shunting yard needs to be turned into a generator pattern */
std::string Calculator::evaluate(std::string expression) {
    Tokenizer tokenizer(expression);
    std::unique_ptr<Token> token;
    std::vector<std::unique_ptr<Token>> output;
    std::vector<std::unique_ptr<Token>> ops;
    while ((token = tokenizer.get_next_token())) {
        PairVisitor pv;
        token = pv.enter_token(std::move(token));
        if(pv.first_is_number()) {
            output.push_back(std::move(token));
        } else if(pv.first_is_op()) {
            while(true) {
                if((ops.size() > 0)) {
                    PairVisitor pv;
                    token = pv.enter_token(std::move(token));
                    auto temp = pv.enter_token(std::move(ops.back()));
                    ops.pop_back();
                    ops.push_back((std::move(temp)));
                    if(pv.second_is_op() && (pv.comp_op_lesser_prec() || (pv.comp_op_equal_prec() && pv.first_op_assoc() == left))) {
                        output.push_back(std::move(ops.back()));
                        ops.pop_back();
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            ops.push_back(std::move(token));
        } else if(pv.first_is_param()) {
            if(token->get_raw()[0] == '(') {
                ops.push_back(std::move(token));
            } else if(token->get_raw()[0] == ')') {
                bool completed_successfully = false;
                while (ops.size() > 0) {
                    std::unique_ptr<Token> temp = std::move(ops.back());
                    ops.pop_back();
                    if(temp->get_raw()[0] != '(') {
                        output.push_back(std::move(temp));
                    } else {
                        completed_successfully = true;
                        if(temp->get_raw()[0] != '(') {
                            ops.push_back(std::move(temp));
                        } else {
                            break;
                        }
                    }
                }
                if(!completed_successfully) {
                    std::cerr << "non matching parenthesis" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    while (ops.size() > 0) {
        output.push_back(std::move(ops.back()));
        ops.pop_back();
    }
    for(auto& i: output) {
        i->print();
        std::cout << ",";
    }
    std::cout << std::endl;

    int pos = output.size()-1;
    std::cout << "output: " << ::evaluate(output, pos) << std::endl;;
    return "";
}

// vim:set sw=4 ts=4 et:

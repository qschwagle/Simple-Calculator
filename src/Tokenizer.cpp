#include "Tokenizer.hpp"

void Tokenizer::ignore_white(void)
{
    for(;buffer_.size() > pos_ && (buffer_[pos_] == ' ' || buffer_[pos_] == '\r' || buffer_[pos_] == '\n' || buffer_[pos_] == '\t'); ++pos_);
}


std::unique_ptr<Number> Tokenizer::number(void)
{
    unsigned int pos = pos_;
    bool pt = false;
    bool dec = false;
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

    if(buffer_.size() > pos && buffer_[pos] == '.') {
        ++pos;
        pt = true;
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
                 ); ++pos ) { dec = true; }
    }

    if(pos == pos_ && (pt == dec) ) { 
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
    ignore_white();
    if(res == nullptr && buffer_.size() != pos_ ) {
        std::cerr << "Unknown character: \"" << buffer_[pos_] << "\""<< std::endl; 
        exit(EXIT_FAILURE);
    }
    return res;
}


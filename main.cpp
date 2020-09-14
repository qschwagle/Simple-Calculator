#include<iostream>
#include<string>
#include<unordered_map>
#include<memory>
#include<vector>

// SUM -> VALUE + SUM | VALUE - SUM  | VALUE
// PRODUCT -> SUM * PRODUCT | SUM / PRODUCT | SUM


// S -> PRODUCT | PAR
// PARPROD -> ( PRODUCT ) | SUM  
// PRODUCT -> SUM | PARPROD * PARPROD | 
// SUM -> VALUE | S - S | S + S 

// 1 + 1
// 1 + 1 * 1
// 1 * 1 + 1

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

    friend void test_space(void);
    friend void test_number(void);
    friend void test_op(void);
};

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

template<typename T> 
bool my_assert(T expected, T actual, std::string test_name)
{
    if(actual == expected) {
        std::cout << test_name << " passed" << std::endl;
        return true;
    } else {
        std::cout << test_name << " failed(expected: " << expected << ", actual: " <<  actual << ")" << std::endl;
        return false;
    }
}


void test_space() {
    std::string test = "";
    Tokenizer t(test);
    t.ignore_white();
    my_assert(0, t.get_pos(), "test 1");

    test = "c";
    Tokenizer t2(test);
    t2.ignore_white();
    my_assert(0, t2.get_pos(), "test 2");
    
    test = "    c";
    Tokenizer t3(test);
    t3.ignore_white();
    t3.ignore_white();
    my_assert(4, t3.get_pos(), "test 3" );

    test = "\r c";
    Tokenizer t4(test);
    t4.ignore_white();
    my_assert(2, t4.get_pos(), "test 4");

    test = "c\r";
    Tokenizer t5(test);
    t5.set_pos(1);
    t5.ignore_white();
    my_assert(2, t5.get_pos(), "test 5");

    test = "c    ";
    Tokenizer t6(test);
    t6.set_pos(1);
    t6.ignore_white();
    my_assert(5, t6.get_pos(), "test 6");
}

void test_number() {
    std::string test = "";
    Tokenizer t(test);
    auto res = t.number();
    my_assert(0, t.get_pos(), "test number 1 pos");
    my_assert(true, res.get() == nullptr, "test number 1 value");

    test = "c";
    Tokenizer t2(test);
    res = t2.number();
    my_assert(0, t2.get_pos(), "test number 2 pos");
    my_assert(true, res.get() == nullptr, "test number 2 value");

    test = "123";
    Tokenizer t3(test);
    res = t3.number();
    my_assert(3, t3.get_pos(), "test number 3 pos");
    if(my_assert(true, res.get() != nullptr, "test number 3 value null?")) {
        my_assert(true, res.get()->get_raw() == "123", "test number 3 value");
    } else {
        my_assert(true, false, "test number 3 value");
    }
}

void test_op() {
    std::string test = "";
    Tokenizer t(test);
    auto res = t.op();
    my_assert(0, t.get_pos(), "test op 1 pos");
    my_assert(true, res.get() == nullptr, "test op 1 value");

    test = "+";
    Tokenizer t2(test);
    res = t2.op();
    my_assert(1, t2.get_pos(), "test op 2 pos");
    if(my_assert(true, res.get() != nullptr, "test op 2 value null?")) {
        my_assert(true, res.get()->get_raw() == "+", "test number 2 value");
    } else {
        my_assert(true, false, "test op 2 value");
    }

    test = "-";
    Tokenizer t3(test);
    res = t3.op();
    my_assert(1, t2.get_pos(), "test op 3 pos");
    if(my_assert(true, res.get() != nullptr, "test op 3 value null?")) {
        my_assert(true, res.get()->get_raw() == "-", "test number 3 value");
    } else {
        my_assert(true, false, "test op 3 value");
    }
}

void test_next_token() {
    std::string test = "(1 +    1)";
    Tokenizer t(test);
    auto res = t.get_next_token();
    my_assert(true, res->get_raw() == std::string("("), "test next token (");
    res = t.get_next_token();
    my_assert(true, res->get_raw() == std::string("1"), "test next token 1");
    my_assert(true, res->get_type() == number, "test next token 1 type");
    res = t.get_next_token();
    my_assert(true, res->get_raw() == std::string("+"), "test next token +");
    res = t.get_next_token();
    my_assert(true, res->get_raw() == std::string("1"), "test next token 1");
    res = t.get_next_token();
    my_assert(true, res->get_raw() == std::string(")"), "test next token )");
    res = t.get_next_token();
    my_assert(true, res == nullptr, "test next token done");
}

class Calculator {
public:
    Calculator()=default;
    Calculator& operator=(const Calculator&)=delete;
    Calculator(const Calculator&)=delete;

    std::string evaluate(std::string expression);
private:
};

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
}

/** Shunting yard needs to be turned into a generator pattern */
std::string Calculator::evaluate(std::string expression) {
    Tokenizer tokenizer(expression);
    std::unique_ptr<Token> token;
    std::vector<std::unique_ptr<Token>> output;
    std::vector<std::unique_ptr<Operator>> ops;
    while ((token = tokenizer.get_next_token())) {
        if(token->get_type() == number) {
            output.push_back(std::move(token));
        } else if (token->get_type() == op) {
            // YES THIS IS BAD CODE
            // YES IF DYNAMIC_CAST FAILS, THIS WILL BREAK HARD
            // however, given that we checked for the type before the conversion, the conversion *should* always succeed
            // should refactor later
            std::unique_ptr<Operator> op(dynamic_cast<Operator*>(token.release()));
            while((ops.size() > 0) && 
                    (( ops.back()->get_precedence() > op->get_precedence() || ops.back()->get_precedence() == op->get_precedence() && op->get_assoc() == left))) {
                output.push_back(std::move(ops.back()));
                ops.pop_back();
            }
            ops.push_back(std::move(op));
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



int main(int argc, char** argv)
{
    test_space();
    test_number();
    test_op();
    test_next_token();
    Calculator c;
    c.evaluate("1+1*2*2");
    c.evaluate("1+5*2*2");

}
/* vim:set sw=4 ts=4 et: */

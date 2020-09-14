#include "lib.hpp"

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
// vim:set sw=4 ts=4 et:

#include <gtest/gtest.h>

#include "Calculator.hpp"

TEST(CALCULATOR, NONE) {

}

TEST(CALCULATOR, ONE) {
    Calculator cal;
    std::string out = cal.evaluate("1");
    ASSERT_EQ(out, "1");
}

TEST(CALCULATOR, THIRTEEN) {
    Calculator cal;
    std::string out = cal.evaluate("13");
    ASSERT_EQ(out, "13");
}

TEST(CALCULATOR, ONE_MULTI_IDENTITY) {
    Calculator cal;
    std::string out = cal.evaluate("1*1");
    ASSERT_EQ(out, "1");
}

TEST(CALCULATOR, FOURTY_TWO_MULT) {
    Calculator cal;
    std::string out = cal.evaluate("7*6");
    ASSERT_EQ(out, "42");
}

TEST(CALCULATOR, PARENTHESIS_ZERO) {
    Calculator cal;
    std::string out = cal.evaluate("(0)");
    ASSERT_EQ(out, "0");
}

TEST(CALCULATOR, PARENTHESIS_MULT_AFTER) {
    Calculator cal;

    std::string out = cal.evaluate("(2+2)*3");
    ASSERT_EQ(out, "12");
}

TEST(CALCULATOR, PARENTHESIS_MULT_BEFORE) {
    Calculator cal;

    std::string out = cal.evaluate("10*(32+2)");
    ASSERT_EQ(out, "340");
}

TEST(CALCULATOR, PARENTHESIS_DIV) {
    Calculator cal;

    std::string out = cal.evaluate("9/(2+1)");
    ASSERT_EQ(out, "3");

    out = cal.evaluate("100/(20-10)");
    ASSERT_EQ(out, "10");
}

// vim:set sw=4 ts=4 et:

#include <gtest/gtest.h>

#include "Tokenizer.hpp"

TEST(TOKENIZER, zero) {
    std::string buffer = "0.000";
    Tokenizer tokenizer(buffer);
    auto out = tokenizer.number();
    ASSERT_EQ(out->get_type(), number);
}
// vim:set sw=4 ts=4 et:

#include "lib.hpp"

int main(int argc, char** argv)
{
    while(true) {
        std::string user_input;
        getline(std::cin, user_input);
        Calculator cal;
        cal.evaluate(user_input);
    }
}
// vim:set sw=4 ts=4 et: 

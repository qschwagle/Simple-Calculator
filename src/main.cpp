#include "Calculator.hpp"

#include<sstream>
#include<string>
#include<iostream>

int main(int argc, char** argv)
{

    if(argc > 2) {
        std::stringstream builder;
        for(int i = 1; i < argc; ++i) {
            builder << argv[i];
            if(i+1 != argc) {
                builder << " ";
            }
        }
        Calculator cal; 
        std::cout << cal.evaluate(builder.str()) << std::endl;
        return 0;
    }
    while(true) {
        std::string user_input;
        getline(std::cin, user_input);
        Calculator cal; 
        std::cout << cal.evaluate(user_input) << std::endl;
    }
}
// vim:set sw=4 ts=4 et: 

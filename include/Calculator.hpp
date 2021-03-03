#pragma once

#include<string>
#include<vector>
#include<memory>

#include "Token.hpp"

class Calculator {
public:
    Calculator()=default;
    Calculator& operator=(const Calculator&)=delete;
    Calculator(const Calculator&)=delete;

    std::string evaluate(std::string expression);
private:
};

double evaluate(std::vector<std::unique_ptr<Token>>& list, int& pos);

#include "Calculator.hpp"

#include "Tokenizer.hpp"
#include "PairVisitor.hpp"

#include <sstream>

double evaluate(std::vector<std::unique_ptr<Token>>& list, int& pos) {
    auto& last = list[pos];
    --pos;
    if(last->get_type() == number) {
        return std::stod(last->get_raw());
    } else if(last->get_type() == op) {
        double val = evaluate(list, pos);

        switch(last->get_raw()[0]) {
            case '-':
                val = evaluate(list, pos) - val;
                break;
            case '+':
                val += evaluate(list, pos);
                break;
            case '*':
                val *= evaluate(list, pos);
                break;
            case '/':
                val = evaluate(list, pos) / val;
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
            while(ops.size() > 0) {
                PairVisitor pv;
                token = pv.enter_token(std::move(token));
                auto temp = pv.enter_token(std::move(ops.back()));
                ops.pop_back();
                ops.push_back((std::move(temp)));
                if((pv.second_is_op() && (pv.comp_op_lesser_prec() || (pv.comp_op_equal_prec() && pv.first_op_assoc() == left)))) {
                    output.push_back(std::move(ops.back()));
                    ops.pop_back();
                } else {
                    break;
                }
            }
            ops.push_back(std::move(token));
        } else if(pv.first_is_param()) {
            switch(token->get_raw()[0]) {
                case '(':
                {
                    ops.push_back(std::move(token));
                    break;
                }
                case ')':
                {
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
                    break;
                }
                default:
                {
                    std::cerr << "PARAM SHOULD ONLY HAVE EITHER ()" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    while (ops.size() > 0) {
        output.push_back(std::move(ops.back()));
        ops.pop_back();
    }
    //for(auto& i: output) {
    //    i->print();
    //    std::cout << ",";
    //}
    //std::cout << std::endl;

    int pos = output.size()-1;
    std::stringstream out;
    out << ::evaluate(output, pos);
    return out.str();
}


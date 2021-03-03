#pragma once

class Operator;
class Param;
class Number;
class Token;


class Visitor {
public:
    virtual void set_op(Operator* op)=0;
    virtual void set_number(Number *num)=0;
    virtual void set_param(Param *param)=0;
    virtual void set_token(Token *token)=0;
};



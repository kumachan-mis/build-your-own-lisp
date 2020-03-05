#ifndef _LISPVALUE_HPP_
#define _LISPVALUE_HPP_


#include <string>
#include <vector>


enum struct LispType {
    Number,
    Symbol,
    S_Expression
};

struct LispValue {
    LispType type;
    int number;
    std::string symbol;
    std::vector<LispValue> cells;

    LispValue(int _number): type(LispType::Number), number(_number), symbol(), cells() {}
    LispValue(const std::string& _symbol): type(LispType::Symbol), number(), symbol(_symbol), cells() {}
    LispValue(): type(LispType::S_Expression), number(), symbol(), cells() {}
};

void cout_lispvalue(const LispValue& value);

#endif // _LISPVALUE_HPP_

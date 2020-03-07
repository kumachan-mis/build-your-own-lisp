#ifndef _LISPVALUE_HPP_
#define _LISPVALUE_HPP_


#include <string>
#include <vector>


enum struct LispType {
    // Atoms
    Number,
    Symbol,
    // Lists
    S_Expression,
    Q_Expression
};

struct LispValue {
    LispType type;
    // Atoms
    std::string value;
    // Lists
    std::vector<LispValue> cells;

    LispValue(const LispType& _type): type(_type), value(), cells() {}
    LispValue(const LispType& _type, const std::string& _value): type(_type), value(_value), cells() {}
    int to_int() const;
};

std::ostream& operator<<(std::ostream& os, const LispValue& value);


#endif // _LISPVALUE_HPP_

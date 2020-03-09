#include "lispvalue.hpp"
#include <iostream>
#include <vector>


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector);

std::ostream& operator<<(std::ostream& os, const LispValue& value) {
    switch (value.type) {
        case LispType::Unit:
            return os << "()";
        case LispType::Number:
            return os << value.number;
        case LispType::Symbol:
            return os << value.symbol;
        case LispType::BuiltinFunction:
        case LispType::LambdaFunction:
            return os << "<function>";
        case LispType::S_Expression:
            return os << '(' << value.expr_cells << ')';
        case LispType::Q_Expression:
            return os << '{' << value.expr_cells << '}';
        default:
            return os;
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
    for (auto itr = vector.begin(), end = vector.end(); itr != end; itr++) {
        os << *itr;
        if (itr != end - 1) os << ' ';
    }
    return os;
}

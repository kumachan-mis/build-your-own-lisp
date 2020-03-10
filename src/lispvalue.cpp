#include "lispvalue.hpp"
#include <iostream>

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
            return os << "<built-in> " << value.symbol;
        case LispType::LambdaFunction:
            return os << "lambda " << value.cells[0] << ' ' << value.cells[1];
        case LispType::S_Expression:
            return os << '(' << value.cells << ')';
        case LispType::Q_Expression:
            return os << '{' << value.cells << '}';
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

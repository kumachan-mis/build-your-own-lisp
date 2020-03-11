#include "lispvalue.hpp"
#include <iostream>
#include <algorithm>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector);

std::ostream& operator<<(std::ostream& os, const LispValue& value) {
    switch (value.type) {
        case LispType::Unit:
            return os << "()";
        case LispType::Number:
            return os << value.number;
        case LispType::String:
            return os << '\"' << value.str << '\"';
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

bool operator ==(const LispValue & x, const LispValue& y) {
    if (x.type != y.type) return false;
    switch (x.type) {
        case LispType::Unit:
            return true;
        case LispType::Number:
            return x.number == y.number;
        case LispType::String:
            return x.str == y.str;
        case LispType::Symbol:
        case LispType::BuiltinFunction:
            return x.symbol == y.symbol;
        case LispType::LambdaFunction:
            return x.cells == y.cells && x.local_environment == y.local_environment;
        case LispType::S_Expression:
        case LispType::Q_Expression:
            return x.cells == y.cells;
        default:
            throw std::invalid_argument("Error: Unknown type");
    }
}

bool operator !=(const LispValue & x, const LispValue& y) {
    return !(x == y);
}

std::string LispValue::type_name() const {
    switch (type) {
        case LispType::Unit:
            return "Unit";
        case LispType::Number:
            return "Number";
        case LispType::String:
            return "String";
        case LispType::Symbol:
            return "Symbol";
        case LispType::BuiltinFunction:
            return "BuiltinFunction";
        case LispType::LambdaFunction:
            return "LambdaFunction";
        case LispType::S_Expression:
            return "S-Expression";
        case LispType::Q_Expression:
            return "Q-Expression";
        default:
            throw std::invalid_argument("Error: Unknown type");
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
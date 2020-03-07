#include "lispvalue.hpp"
#include <iostream>
#include <vector>


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector);


int LispValue::to_int() const {
    if (this->type != LispType::Number) {
        throw std::invalid_argument("Error: argument type is not number");
    }
    try {
        return std::stoi(this->value);
    } catch (...) {
        throw std::invalid_argument("Error: fail to convert argument to number");
    }
}

std::ostream& operator<<(std::ostream& os, const LispValue& value) {
    switch (value.type) {
        case LispType::Number:
        case LispType::Symbol:
            return os << value.value;
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

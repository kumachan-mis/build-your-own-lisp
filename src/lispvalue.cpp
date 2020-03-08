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
        case LispType::Symbol:
            return os << value.value;
        case LispType::Function:
            return os << "<function> " << value.value;
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

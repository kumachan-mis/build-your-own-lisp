#include "lispvalue.hpp"
#include <iostream>


void cout_lispvalue(const LispValue& value);
void rec_cout_lispvalue(const LispValue& value);


void cout_lispvalue(const LispValue& value) {
    rec_cout_lispvalue(value);
    std::cout << std::endl;
}

void rec_cout_lispvalue(const LispValue& value) {
    switch (value.type) {
        case LispType::Number:
            std::cout << value.number;
            break;
        case LispType::Symbol:
            std::cout << value.symbol;
            break;
        case LispType::S_Expression:
            std::cout << '(';
            for (
                auto cell_itr = value.cells.begin(), end = value.cells.end();
                cell_itr != end; cell_itr++
            ) {
                rec_cout_lispvalue(*cell_itr);
                if (cell_itr != end - 1) std::cout << ' ';
            }
            std::cout << ')';
            break;
    }
}

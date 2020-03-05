#include <iostream>
#include <string>
#include <stdexcept>
#include "operation.hpp"
#include "lispvalue.hpp"


LispValue evaluate(LispValue& value);
int evaluate_operator(std::string op, int x, int y);
int evaluate_operator(std::string op, int x);


LispValue evaluate(LispValue& value) {
    const int num_cells = value.cells.size();
    for (int index = 0; index < num_cells; index++) {
        value.cells[index] = evaluate(value.cells[index]);
    }

    if (num_cells == 0) return value;
    if (num_cells == 1) return value.cells[0];

    LispValue symbol = value.cells[0];
    if (symbol.type != LispType::Symbol) {
        throw std::invalid_argument("Error: S-Expression does not start with symbol");
    }
    for (
        auto cell_itr = value.cells.begin() + 1, end = value.cells.end();
        cell_itr != end; cell_itr++
    ) {
        if (cell_itr->type != LispType::Number) {
            throw std::invalid_argument("Cannot operate on non-number");
        }
    }

    int number = (value.cells.begin() + 1)->number;
    if (num_cells == 2) {
        number = evaluate_operator(symbol.symbol, number);
    } else {
        for (
            auto cell_itr = value.cells.begin() + 2, end = value.cells.end(); 
            cell_itr != end; cell_itr++
        ) {
            number = evaluate_operator(symbol.symbol, number, cell_itr->number);
        }
    }
    value.type = LispType::Number;
    value.number = number;
    value.cells.clear();
    return value;
}

int evaluate_operator(std::string op, int x, int y) {
    if (op == "+") return safe_add(x, y);
    if (op == "-") return safe_sub(x, y);
    if (op == "*") return safe_mul(x, y);
    if (op == "/") return safe_div(x, y);
    if (op == "%") return safe_mod(x, y);
    if (op == "^") return safe_pow(x, y);
    throw std::invalid_argument("Error: operator \"" + op + "\" is not supported");
}

int evaluate_operator(std::string op, int x) {
    if (op == "+") return  x;
    if (op == "-") return safe_neg(x);
    throw std::invalid_argument("Error: operator \"" + op + "\" is not supported");
}
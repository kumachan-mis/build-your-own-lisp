#include <iostream>
#include <string>
#include <stdexcept>
#include "builtin.hpp"
#include "lispvalue.hpp"


LispValue evaluate_sexpr(LispValue& value);
inline LispValue evaluate_builtin_functions(
    const std::string& function_name,
    const std::vector<LispValue>& arguments
);

LispValue evaluate(LispValue& value) {
    if (value.type == LispType::S_Expression) return evaluate_sexpr(value);
    return value;
}

LispValue evaluate_sexpr(LispValue& value) {
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
    value.cells.erase(value.cells.begin());
    value = evaluate_builtin_functions(symbol.value, value.cells);
    return value;
}

inline LispValue evaluate_builtin_functions(
    const std::string& function_name,
    const std::vector<LispValue>& arguments
) {
    if (function_name == "+")    return builtin_add(arguments);
    if (function_name == "-")    return builtin_sub(arguments);
    if (function_name == "*")    return builtin_mul(arguments);
    if (function_name == "/")    return builtin_div(arguments);
    if (function_name == "%")    return builtin_mod(arguments);
    if (function_name == "^")    return builtin_pow(arguments);
    if (function_name == "list") return builtin_list(arguments);
    if (function_name == "head") return builtin_head(arguments);
    if (function_name == "tail") return builtin_tail(arguments);
    if (function_name == "join") return builtin_join(arguments);
    if (function_name == "eval") return builtin_eval(arguments);
    if (function_name == "cons") return builtin_cons(arguments);
    if (function_name == "len")  return builtin_len(arguments);
    throw std::invalid_argument("Error: built-in function \"" + function_name + "\" is not supported");
}

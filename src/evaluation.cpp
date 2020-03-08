#include <iostream>
#include <string>
#include <stdexcept>
#include "builtin.hpp"
#include "lispvalue.hpp"


inline void add_builtin_function(
    const std::string& name,
    const LispBuiltinFunction& function,
    LispEnvironment& environment
);
inline LispValue evaluate_symbol(LispValue& value, LispEnvironment& environment);
inline LispValue evaluate_sexpr(LispValue& value, LispEnvironment& environment);


LispEnvironment global_environment() {
    LispEnvironment environment;
    add_builtin_function("+", builtin_add, environment);
    add_builtin_function("-", builtin_sub, environment);
    add_builtin_function("*", builtin_mul, environment);
    add_builtin_function("/", builtin_div, environment);
    add_builtin_function("%", builtin_mod, environment);
    add_builtin_function("^", builtin_pow, environment);

    add_builtin_function("list", builtin_list, environment);
    add_builtin_function("head", builtin_head, environment);
    add_builtin_function("tail", builtin_tail, environment);
    add_builtin_function("join", builtin_join, environment);
    add_builtin_function("eval", builtin_eval, environment);
    add_builtin_function("cons", builtin_cons, environment);
    add_builtin_function("len",  builtin_len,  environment);

    add_builtin_function("def",  builtin_def,  environment);

    add_builtin_function("exit",  builtin_exit,  environment);

    return environment;
}

LispValue evaluate(LispValue& value, LispEnvironment& environment) {
    switch (value.type) {
        case LispType::Unit:
            /* Already evaluated */
            return value;
        case LispType::Number:
            /* Already evaluated */
            return value;
        case LispType::Symbol:
            return evaluate_symbol(value, environment);
        case LispType::Function:
            /* Already evaluated */
            return value;
        case LispType::S_Expression:
            return evaluate_sexpr(value, environment);
        case LispType::Q_Expression:
            /* Stop evaluation */
            return value;
        default:
            throw std::invalid_argument("Error: Unexpected type");
    }
}


inline void add_builtin_function(
    const std::string& name,
    const LispBuiltinFunction& function,
    LispEnvironment& environment
) {
    LispValue value(LispType::Function, name);
    value.function = function;
    environment.define(name, value);
}

inline LispValue evaluate_symbol(LispValue& value, LispEnvironment& environment) {
    return environment.find(value);
}

inline LispValue evaluate_sexpr(LispValue& value, LispEnvironment& environment) {
    const int num_cells = value.cells.size();
    for (int index = 0; index < num_cells; index++) {
        value.cells[index] = evaluate(value.cells[index], environment);
    }
    if (num_cells == 1) return value.cells[0];

    LispValue function = value.cells[0];
    if (function.type != LispType::Function) {
        throw std::invalid_argument("Error: S-Expression does not start with function");
    }
    value.cells.erase(value.cells.begin());
    value = function.function(value.cells, environment);
    return value;
}

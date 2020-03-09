#include "builtin.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include "evaluation.hpp"


LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op
);
inline int _add(int x, int y);
inline int _sub(int x, int y);
inline int _mul(int x, int y);
inline int _div(int x, int y);
inline int _mod(int x, int y);
inline int _pow(int x, int y);
inline int _neg(int x);


LispValue builtin_add(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: function \"add\" takes 1 or more arguments");
    }
    if (num_args == 1) {
        int result = evaluated_arguments[0].number;
        return LispValue(LispType::Number, result);
    }
    return _operator(evaluated_arguments, _add);
}

LispValue builtin_sub(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
     size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: function \"sub\" takes one or more arguments");
    }
    if (num_args == 1) {
        int result = _neg(evaluated_arguments[0].number);
        return LispValue(LispType::Number, result);
    }
    return _operator(evaluated_arguments, _sub);
}

LispValue builtin_mul(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mul\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mul);
}

LispValue builtin_div(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"div\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _div);
}

LispValue builtin_mod(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mod\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mod);
}

LispValue builtin_pow(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"pow\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _pow);
}

LispValue builtin_list(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    return LispValue(LispType::Q_Expression, evaluated_arguments);
}

LispValue builtin_head(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"head\" takes one argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"head\" takes Q-Expression");
    }
    if (argument.expr_cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.expr_cells = { argument.expr_cells[0] };
    return argument;
}

LispValue builtin_tail(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"tail\" takes a argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"tail\" takes Q-Expression");
    }
    if (argument.expr_cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.expr_cells.erase(argument.expr_cells.begin());
    return argument;
}

LispValue builtin_join(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 1) {
        throw std::invalid_argument("Error: function \"join\" takes one or more arguments");
    }

    if (
        !std::all_of(
            evaluated_arguments.begin(), evaluated_arguments.end(),
            [](const LispValue& argument) { return argument.type == LispType::Q_Expression; })
    ) {
        throw std::invalid_argument("Error: function \"join\" takes Q-Expressions");
    }
    LispValue& result(evaluated_arguments[0]);
    for (
        auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
        itr != end; itr++
    ) {
        result.expr_cells.insert(
            result.expr_cells.end(),
            itr->expr_cells.begin(), itr->expr_cells.end()
        );
    }
    return result;
}

LispValue builtin_eval(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"eval\" takes one argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"eval\" takes Q-Expression");
    }
    argument.type = LispType::S_Expression;
    return evaluate(argument, environment);
}

LispValue builtin_cons(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function \"cons\" takes two arguments");
    }
    const LispValue& head(evaluated_arguments[0]);
    LispValue& tail(evaluated_arguments[1]);
    if (tail.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: second argument is expected to be Q-Expression");
    }
    tail.expr_cells.insert(tail.expr_cells.begin(), head);
    return tail;
}

LispValue builtin_len(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"len\" takes one argument");
    }
    const LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"len\" takes Q-Expression");
    }
    return LispValue(LispType::Number, argument.expr_cells.size());
}

LispValue builtin_def(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"def\" takes 2 or more arguments");
    }

    const std::vector<LispValue>& symbols(evaluated_arguments[0].expr_cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        symbols.size() < 1 ||
        !std::all_of(
            symbols.begin(), symbols.end(),
            [](const LispValue& symbol) { return symbol.type == LispType::Symbol; })
    ) {
        throw std::invalid_argument("Error: first argument is expected to be Q-Expression of symbols");
    }
    
    if (symbols.size() != evaluated_arguments.size() - 1) {
        throw std::invalid_argument("Error: cannot define incorrect number of values to symbols");
    }

    for (size_t index = 0; index < symbols.size(); index++) {
        environment.define(symbols[index], evaluated_arguments[index + 1]);
    }
    return LispValue();
}


LispValue builtin_del(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"del\" takes one argument");
    }

    const std::vector<LispValue>& symbols(evaluated_arguments[0].expr_cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        symbols.size() < 1 ||
        !std::all_of(
            symbols.begin(), symbols.end(),
            [](const LispValue& symbol) { return symbol.type == LispType::Symbol; })
    ) {
        throw std::invalid_argument("Error: first argument is expected to be Q-Expression of symbols");
    }

    for (size_t index = 0; index < symbols.size(); index++) {
        environment.erase(symbols[index]);
    }
    return LispValue();
}

LispValue builtin_exit(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"exit\" takes one argument");
    }
    const LispValue& argument(evaluated_arguments[0]);
    if (argument.type == LispType::Unit) {
        exit(0);
    } else if (argument.type == LispType::Number) {
        exit(argument.number);
    } else {
        throw std::invalid_argument("Error: function \"len\" takes Unit or Number");
    }
}


/*
Requires
    evaluated_arguments: size should be 2 or more
*/
LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op
) {
    int result = evaluated_arguments[0].number;
    for (
        auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
        itr != end; itr++
    ) {
        result = op(result, itr->number);
    }
    return LispValue(LispType::Number, result);
}

inline int _add(int x, int y) {
    std::numeric_limits<int> limits;
    if (
        (x > 0 && y > 0 && x > limits.max() - y) ||
        (x < 0 && y < 0 && x < limits.min() - y)
    ) {
        throw std::overflow_error("Error: overflow occurs");
    }
    return x + y;
}

inline int _sub(int x, int y) {
    std::numeric_limits<int> limits;
    if (
        (x > 0 && y < 0 && y < x - limits.max()) ||
        (x < 0 && y > 0 && x < limits.min() + y)
    ) {
        throw std::overflow_error("Error: overflow occurs");
    }
    return x - y;
}

inline int _mul(int x, int y) {
    std::numeric_limits<int> limits;
    if (
        (x > 0 && y > 0 && x > limits.max() / y) ||
        (x < 0 && y < 0 && x < limits.max() / y) ||
        (x > 0 && y < 0 && y < limits.min() / x) ||
        (x < 0 && y > 0 && x < limits.min() / y)
    ) {
        throw std::overflow_error("Error: overflow occurs");
    }
    return x * y;
}

inline int _div(int x, int y) {
    std::numeric_limits<int> limits;
    if (y == 0) {
        throw std::invalid_argument("Error: zero division is invalid");
    }
    if (y == -1 && x == limits.min()) {
         throw std::overflow_error("Error: overflow occurs");
    }
    return x / y;
}

inline int _mod(int x, int y) {
    std::numeric_limits<int> limits;
    if (y == 0) {
        throw std::invalid_argument("Error: zero division is invalid");
    }
    if (y == -1 && x == limits.min()) {
         throw std::overflow_error("Error: overflow occurs");
    }
    return x % y;
}

 inline int _pow(int x, int y) {
    if (y < 0) {
        throw std::invalid_argument("Error: negative exponent is not supported");
    }
    int ret = 1;
    int temp = x;
    while (y > 0) {
        if (y % 2 == 1) ret = _mul(ret, temp);
        temp = _mul(temp, temp);
        y /= 2;
    }
    return ret;
}

inline int _neg(int x) {
    std::numeric_limits<int> limits;
    if (x == limits.min()) {
        throw std::overflow_error("Error: overflow occurs");
    }
    return -x;
}

#include "builtin.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include "evaluation.hpp"


LispValue _operator(
    const std::vector<LispValue>& evaluated_arguments,
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
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: function \"add\" takes 1 or more arguments");
    }
    if (num_args == 1) {
        int result = evaluated_arguments[0].to_int();
        return LispValue(LispType::Number, std::to_string(result));
    }
    return _operator(evaluated_arguments, _add);
}

LispValue builtin_sub(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
     size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: function \"sub\" takes one or more arguments");
    }
    if (num_args == 1) {
        int result = _neg(evaluated_arguments[0].to_int());
        return LispValue(LispType::Number, std::to_string(result));
    }
    return _operator(evaluated_arguments, _sub);
}

LispValue builtin_mul(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mul\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mul);
}

LispValue builtin_div(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"div\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _div);
}

LispValue builtin_mod(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mod\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mod);
}

LispValue builtin_pow(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"pow\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _pow);
}

LispValue builtin_list(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    LispValue result(LispType::Q_Expression);
    result.cells = evaluated_arguments;
    return result;
}

LispValue builtin_head(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"head\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"head\" takes Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells = { argument.cells[0] };
    return argument;
}

LispValue builtin_tail(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"tail\" takes a argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"tail\" takes Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells.erase(argument.cells.begin());
    return argument;
}

LispValue builtin_join(
    const std::vector<LispValue>& evaluated_arguments,
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
    LispValue result(evaluated_arguments[0]);
    for (
        auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
        itr != end; itr++
    ) {
        result.cells.insert(result.cells.end(), itr->cells.begin(), itr->cells.end());
    }
    return result;
}

LispValue builtin_eval(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"eval\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"eval\" takes Q-Expression");
    }
    argument.type = LispType::S_Expression;
    return evaluate(argument, environment);
}

LispValue builtin_cons(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function \"cons\" takes two arguments");
    }
    LispValue head(evaluated_arguments[0]);
    LispValue tail(evaluated_arguments[1]);
    if (tail.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: second argument is expected to be Q-Expression");
    }
    tail.cells.insert(tail.cells.begin(), head);
    return tail;
}

LispValue builtin_len(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"len\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"len\" takes Q-Expression");
    }
    return LispValue(LispType::Number, std::to_string(argument.cells.size()));
}

LispValue builtin_def(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"def\" takes 2 or more arguments");
    }

    const std::vector<LispValue> symbols(evaluated_arguments[0].cells);
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
    return LispValue(LispType::Unit);
}

LispValue builtin_exit(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"exit\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type == LispType::Unit) {
        exit(0);
    } else if (argument.type == LispType::Number) {
        exit(argument.to_int());
    } else {
        throw std::invalid_argument("Error: function \"len\" takes Unit or Number");
    }
}


/*
Requires
    evaluated_arguments: size should be 2 or more
*/
LispValue _operator(
    const std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op
) {
    int result = evaluated_arguments[0].to_int();
    for (
        auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
        itr != end; itr++
    ) {
        result = op(result, itr->to_int());
    }
    return LispValue(LispType::Number, std::to_string(result));
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

#include "builtin.hpp"
#include <functional>
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


LispValue builtin_add(const std::vector<LispValue>& evaluated_arguments) {
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

LispValue builtin_sub(const std::vector<LispValue>& evaluated_arguments) {
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

LispValue builtin_mul(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mul\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mul);
}

LispValue builtin_div(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"div\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _div);
}

LispValue builtin_mod(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"mod\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _mod);
}

LispValue builtin_pow(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function \"pow\" takes two or more arguments");
    }
    return _operator(evaluated_arguments, _pow);
}

LispValue builtin_list(const std::vector<LispValue>& evaluated_arguments) {
    LispValue result(LispType::Q_Expression);
    result.cells = evaluated_arguments;
    return result;
}

LispValue builtin_head(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"head\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"head\" takes only Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells = { argument.cells[0] };
    return argument;
}

LispValue builtin_tail(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"tail\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"tail\" takes only Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells.erase(argument.cells.begin());
    return argument;
}

LispValue builtin_join(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() < 1) {
        throw std::invalid_argument("Error: function \"join\" takes one or more arguments");
    }
    LispValue result(evaluated_arguments[0]);
    if (result.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"tail\" takes only Q-Expression");
    }

    for (
        auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
        itr != end; itr++
    ) {
        result.cells.insert(result.cells.end(), itr->cells.begin(), itr->cells.end());
    }
    return result;
}

LispValue builtin_eval(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"eval\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"eval\" takes only Q-Expression");
    }
    argument.type = LispType::S_Expression;
    return evaluate(argument);
}

LispValue builtin_cons(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function \"cons\" takes two arguments");
    }
    LispValue head(evaluated_arguments[0]);
    LispValue tail(evaluated_arguments[1]);
    if (tail.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: the second argument is expected to be Q-Expression");
    }
    tail.cells.insert(tail.cells.begin(), head);
    return tail;
}

LispValue builtin_len(const std::vector<LispValue>& evaluated_arguments) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function \"len\" takes one argument");
    }
    LispValue argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function \"len\" takes only Q-Expression");
    }
    return LispValue(LispType::Number, std::to_string(argument.cells.size()));
}


LispValue _operator(
    const std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op
) {
    /*
    Requires:
        evaluated_arguments: size should be 2 or more
    */

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

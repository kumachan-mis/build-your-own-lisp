#include "builtin.hpp"

#include <algorithm>
#include "evaluation.hpp"


inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op,
    const std::string& op_name
);
inline int _add(int x, int y);
inline int _sub(int x, int y);
inline int _mul(int x, int y);
inline int _div(int x, int y);
inline int _mod(int x, int y);
inline int _pow(int x, int y);
inline int _nega(int x);

inline LispValue _order(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& order,
    const std::string& order_name
);
inline LispValue _order(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int)>& order,
    const std::string& order_name
);
inline int _gt(int x, int y);
inline int _geq(int x, int y);
inline int _lt(int x, int y);
inline int _leq(int x, int y);
inline int _and(int x, int y);
inline int _or(int x, int y);
inline int _not(int x);

inline bool all_type_of(const std::vector<LispValue>& cells, LispType type);


LispValue builtin_add(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: operator + takes one or more arguments");
    }
    if (num_args == 1) {
        LispValue& argument(evaluated_arguments[0]);
        if (argument.type != LispType::Number) {
            throw std::invalid_argument("Error: operator + takes numbers");
        }
        return argument;
    }
    return _operator(evaluated_arguments, _add, "add");
}

LispValue builtin_sub(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: operator - takes one or more arguments");
    }
    if (num_args == 1) {
        LispValue& argument(evaluated_arguments[0]);
        if (argument.type != LispType::Number) {
            throw std::invalid_argument("Error: operator - takes numbers");
        }
        argument.number = _nega(argument.number);
        return argument;
    }
    return _operator(evaluated_arguments, _sub, "sub");
}

LispValue builtin_mul(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _mul, "mul");
}

LispValue builtin_div(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _div, "div");
}

LispValue builtin_mod(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _mod, "mod");
}

LispValue builtin_pow(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _pow, "pow");
}

LispValue builtin_if(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 3) {
        throw std::invalid_argument("Error: function if takes three arguments");
    }
    const LispValue& condition(evaluated_arguments[0]);
    if (condition.type != LispType::Number) {
        throw std::invalid_argument("Error: first argument is expected to be number");
    }
    LispValue& then_qexpr(evaluated_arguments[1]);
    LispValue& else_qexpr(evaluated_arguments[2]);
    if (
        then_qexpr.type != LispType::Q_Expression ||
        else_qexpr.type != LispType::Q_Expression
    ) {
        throw std::invalid_argument(
            "Error: second and third argument is expected to be Q-Expression");
    }

    if (condition.number) {
        then_qexpr.type = LispType::S_Expression;
        return evaluate(then_qexpr, environment);
    } else {
        else_qexpr.type = LispType::S_Expression;
        return evaluate(else_qexpr, environment);
    }
}

LispValue builtin_eq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: order eq takes two arguments");
    }

    return LispValue(LispType::Number, evaluated_arguments[0] == evaluated_arguments[1]);
}

LispValue builtin_neq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: order neq takes two arguments");
    }

    return LispValue(LispType::Number, evaluated_arguments[0] != evaluated_arguments[1]);
}

LispValue builtin_gt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _gt, "gt");
}

LispValue builtin_geq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _geq, "geq");
}

LispValue builtin_lt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _lt, "lt");
}

LispValue builtin_leq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _lt, "leq");
}

LispValue builtin_and(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _and, "and");
}

LispValue builtin_or(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _or, "or");
}

LispValue builtin_not(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _order(evaluated_arguments, _not, "not");
}

LispValue builtin_list(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return LispValue(LispType::Q_Expression, evaluated_arguments);
}

LispValue builtin_eval(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function eval takes one argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function eval takes Q-Expression");
    }

    argument.type = LispType::S_Expression;
    return evaluate(argument, environment);
}

LispValue builtin_head(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function head takes one argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function head takes Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells = { argument.cells[0] };
    return argument;
}

LispValue builtin_tail(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function tail takes one argument");
    }
    LispValue& argument(evaluated_arguments[0]);
    if (argument.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: function tail takes Q-Expression");
    }
    if (argument.cells.size() == 0) {
        throw std::invalid_argument("Error: the argument is empty Q-Expression");
    }
    argument.cells.erase(argument.cells.begin());
    return argument;
}

LispValue builtin_cons(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function cons takes two arguments");
    }
    const LispValue& head(evaluated_arguments[0]);
    LispValue& tail(evaluated_arguments[1]);
    if (tail.type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: second argument is expected to be Q-Expression");
    }
    tail.cells.insert(tail.cells.begin(), head);
    return tail;
}

LispValue builtin_join(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() < 1) {
        throw std::invalid_argument("Error: function join takes one or more arguments");
    }
    if (all_type_of(evaluated_arguments, LispType::Q_Expression)) {
        LispValue& result(evaluated_arguments[0]);
        for (
            auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
            itr != end; itr++
        ) {
            result.cells.insert(result.cells.end(), itr->cells.begin(), itr->cells.end());
        }
        return result;
    } else if (all_type_of(evaluated_arguments, LispType::String)) {
        LispValue& result(evaluated_arguments[0]);
        for (
            auto itr = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();
            itr != end; itr++
        ) {
            result.str += itr->str;
        }
        return result;
    } else {
        throw std::invalid_argument("Error: function join takes strings or Q-Expressions");
    }
}

LispValue builtin_len(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function len takes one argument");
    }
    const LispValue& argument(evaluated_arguments[0]);
    if (argument.type == LispType::Q_Expression) {
        return LispValue(LispType::Number, argument.cells.size());
    } else if (argument.type == LispType::String) {
        return LispValue(LispType::Number, argument.str.length());
    } else {
        throw std::invalid_argument("Error: function len takes string or Q-Expression");
    }
}

LispValue builtin_lambda(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function lambda takes two arguments");
    }
    std::vector<LispValue>& params(evaluated_arguments[0].cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        !all_type_of(params, LispType::Symbol)
    ) {
        throw std::invalid_argument(
            "Error: first argument is expected to be Q-Expression of zero or more symbols");
    }
    if (std::any_of(
            params.begin(), params.end(),
            [environment](const LispValue& value) { return environment->is_reserved(value.symbol); })
    ) {
        throw std::invalid_argument("Error: cannot use reserved symbol as parameter name");
    }
    if (evaluated_arguments[1].type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: second argument is expected to be Q-Expression");
    }

    std::shared_ptr<LispEnvironment> local_environment(new LispEnvironment(environment));
    return LispValue(LispType::LambdaFunction, evaluated_arguments, local_environment);
}

LispValue builtin_def(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: function def takes 2 or more arguments");
    }

    const std::vector<LispValue>& symbols(evaluated_arguments[0].cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        symbols.size() < 1 || !all_type_of(symbols, LispType::Symbol)
    ) {
        throw std::invalid_argument(
            "Error: first argument is expected to be Q-Expression of one or more symbols");
    }
    if (std::any_of(
            symbols.begin(), symbols.end(),
            [environment](const LispValue& value) { return environment->is_reserved(value.symbol); })
    ) {
        throw std::invalid_argument("Error: cannot re-define reserved symbol");
    }
    if (symbols.size() != evaluated_arguments.size() - 1) {
        throw std::invalid_argument("Error: cannot define incorrect number of values to symbols");
    }

    for (size_t index = 0; index < symbols.size(); index++) {
        environment->define_global(symbols[index].symbol, evaluated_arguments[index + 1]);
    }
    return LispValue();
}

LispValue builtin_defun(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: function defun takes two arguments");
    }
    std::vector<LispValue>& signiture(evaluated_arguments[0].cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        signiture.size() < 1 || !all_type_of(signiture, LispType::Symbol)
    ) {
        throw std::invalid_argument(
            "Error: first argument is expected to be Q-Expression of one or more symbols");
    }
    if (environment->is_reserved(signiture[0].symbol)) {
        throw std::invalid_argument("Error: cannot re-define reserved symbol");
    }
    if (std::any_of(
            signiture.begin() + 1, signiture.end(),
            [environment](const LispValue& value) { return environment->is_reserved(value.symbol); })
    ) {
        throw std::invalid_argument("Error: cannot use reserved symbol as parameter name");
    }
    if (evaluated_arguments[1].type != LispType::Q_Expression) {
        throw std::invalid_argument("Error: second argument is expected to be Q-Expression");
    }

    LispValue symbol(signiture[0]);
    signiture.erase(signiture.begin());

    std::shared_ptr<LispEnvironment> local_environment(new LispEnvironment(environment));
    environment->define_global(
        symbol.symbol,
        LispValue(LispType::LambdaFunction, evaluated_arguments, local_environment)
    );
    return LispValue();
}

LispValue builtin_del(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function del takes one argument");
    }
    const std::vector<LispValue>& symbols(evaluated_arguments[0].cells);
    if (
        evaluated_arguments[0].type != LispType::Q_Expression ||
        symbols.size() < 1 || !all_type_of(symbols, LispType::Symbol)
    ) {
        throw std::invalid_argument(
            "Error: first argument is expected to be Q-Expression of one or more symbols");
    }

    for (size_t index = 0; index < symbols.size(); index++) {
        environment->delete_global(symbols[index].symbol);
    }
    return LispValue();
}

LispValue builtin_type(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: function type takes one argument");
    }
    return LispValue(LispType::String, evaluated_arguments[0].type_name());
}

LispValue builtin_exit(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args != 1) {
        throw std::invalid_argument("Error: function exit takes zero or one argument");
    }
    const LispValue& argument(evaluated_arguments[0]);
    if (argument.type == LispType::Unit) {
        exit(0);
    } else if (argument.type == LispType::Number) {
        exit(argument.number);
    } else {
        throw std::invalid_argument("Error: function exit takes unit or number");
    }
}


inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& op,
    const std::string& op_name
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: operator " + op_name + " takes two or more arguments");
    }
    if (!all_type_of(evaluated_arguments, LispType::Number)) {
        throw std::invalid_argument("Error: operator " + op_name + " takes numbers");
    }

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

inline int _nega(int x) {
    std::numeric_limits<int> limits;
    if (x == limits.min()) {
        throw std::overflow_error("Error: overflow occurs");
    }
    return -x;
}

inline LispValue _order(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& order,
    const std::string& order_name
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: order " + order_name + " takes two arguments");
    }
    if (!all_type_of(evaluated_arguments, LispType::Number)) {
        throw std::invalid_argument("Error: order " + order_name + " takes numbers");
    }

    int result = order(evaluated_arguments[0].number, evaluated_arguments[1].number);
    return LispValue(LispType::Number, result);
}

inline LispValue _order(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int)>& order,
    const std::string& order_name
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: order " + order_name + " takes one argument");
    }
    if (evaluated_arguments[0].type != LispType::Number) {
        throw std::invalid_argument("Error: order " + order_name + " takes a number");
    }

    int result = order(evaluated_arguments[0].number);
    return LispValue(LispType::Number, result);
}

inline int _gt(int x, int y) {
    return x > y;
}

inline int _geq(int x, int y) {
    return x >= y;
}

inline int _lt(int x, int y) {
    return x < y;
}

inline int _leq(int x, int y) {
    return x < y;
}

inline int _and(int x, int y) {
    return x && y;
}

inline int _or(int x, int y) {
    return x || y;
}

inline int _not(int x) {
    return !x;
}

inline bool all_type_of(const std::vector<LispValue>& cells, LispType type) {
    for (const LispValue& value : cells) {
        if (value.type != type) return false;
    }
    return true;
}

#include "builtin.hpp"

#include <algorithm>
#include "evaluation.hpp"


inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int)>& unary_op,
    const std::function<int(int, int)>& binary_op,
    const std::string& name
);
inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& binary_op,
    const std::string& name
);
inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int)>& unary_op,
    const std::string& name
);
inline LispValue _relation(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& relation,
    const std::string& name
);

inline int _add(int x, int y);
inline int _sub(int x, int y);
inline int _mul(int x, int y);
inline int _div(int x, int y);
inline int _mod(int x, int y);
inline int _pow(int x, int y);
inline int _posi(int x);
inline int _nega(int x);

inline int _and(int x, int y);
inline int _or(int x, int y);
inline int _not(int x);
inline int _gt(int x, int y);
inline int _geq(int x, int y);
inline int _lt(int x, int y);
inline int _leq(int x, int y);

inline bool all_type_of(const std::vector<LispValue>& cells, LispType type);


LispValue builtin_add(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _posi, _add, "add");
}

LispValue builtin_sub(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _nega, _sub, "sub");
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
    size_t num_args = evaluated_arguments.size();
    if (num_args != 2 && num_args != 3) {
        throw std::invalid_argument("Error: function if takes two or three arguments");
    }
    const LispValue& condition(evaluated_arguments[0]);
    if (condition.type != LispType::Number) {
        throw std::invalid_argument("Error: first argument is expected to be number");
    }

    LispValue nil(LispType::Q_Expression);
    LispValue& then_qexpr(evaluated_arguments[1]);
    LispValue& else_qexpr(num_args == 3 ? evaluated_arguments[2] : nil);
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

LispValue builtin_cond(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    for (const LispValue& argument : evaluated_arguments) {
        if (
            argument.type != LispType::Q_Expression ||
            argument.cells.size() != 2 || 
            argument.cells[1].type != LispType::Q_Expression
        ) {
            throw std::invalid_argument(
                "Error: each argument is expected to be { condition { statement } }");
        }
    }
    const LispValue& otherwise(environment->resolve("otherwise"));
    for (LispValue& argument : evaluated_arguments) {
        LispValue condition = evaluate(argument.cells[0], environment);
        if (condition.type != LispType::Number && condition != otherwise) {
            throw std::invalid_argument("Error: condition does not evaluate to number");
        }
        if ((condition.type == LispType::Number && condition.number) || condition == otherwise) {
            argument.cells[1].type = LispType::S_Expression;
            return evaluate(argument.cells[1], environment);
        }
    }
    return LispValue();
}

LispValue builtin_case(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() < 1) {
        throw std::invalid_argument("Error: function case takes one or more arguments");
    }
    LispValue value = evaluated_arguments[0];
    evaluated_arguments.erase(evaluated_arguments.begin());
    for (const LispValue& condition_statement : evaluated_arguments) {
        if (
            condition_statement.type != LispType::Q_Expression ||
            condition_statement.cells.size() != 2 ||
            condition_statement.cells[1].type != LispType::Q_Expression
        ) {
            throw std::invalid_argument(
                "Error: each condition statement is expected to be { value { statement } }");
        }
    }

    const LispValue& otherwise(environment->resolve("otherwise"));
    for (LispValue& condition_statement : evaluated_arguments) {
        const LispValue& case_value(condition_statement.cells[0]);
        LispValue& statement(condition_statement.cells[1]);
        if (value == case_value || case_value == otherwise) {
            statement.type = LispType::S_Expression;
            return evaluate(statement, environment);
        }
    }
    return LispValue();
}

LispValue builtin_and(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _and, "and");
}

LispValue builtin_or(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _or, "or");
}

LispValue builtin_not(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _operator(evaluated_arguments, _not, "not");
}

LispValue builtin_eq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: operator eq takes two arguments");
    }
    return LispValue(LispType::Number, evaluated_arguments[0] == evaluated_arguments[1]);
}

LispValue builtin_neq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() != 2) {
        throw std::invalid_argument("Error: operator neq takes two arguments");
    }
    return LispValue(LispType::Number, evaluated_arguments[0] != evaluated_arguments[1]);
}

LispValue builtin_gt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _relation(evaluated_arguments, _gt, "gt");
}

LispValue builtin_geq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _relation(evaluated_arguments, _geq, "geq");
}

LispValue builtin_lt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _relation(evaluated_arguments, _lt, "lt");
}

LispValue builtin_leq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return _relation(evaluated_arguments, _leq, "leq");
}

LispValue builtin_list(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return LispValue(LispType::Q_Expression, evaluated_arguments);
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
    if (argument.type == LispType::String) {
        if (argument.str.empty()) {
            throw std::invalid_argument("Error: argument is empty string");
        }
        argument.str = argument.str[0];
    } else if (argument.type == LispType::Q_Expression) {
        if (argument.cells.empty()) {
            throw std::invalid_argument("Error: argument is empty Q-Expression");
        }
        argument.cells = { argument.cells[0] };
    } else {
        throw std::invalid_argument("Error: function head takes string or Q-Expression");
    }
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
    if (argument.type == LispType::String) {
        const size_t len = argument.str.length();
        if (len == 0) {
            throw std::invalid_argument("Error: argument is empty string");
        }
        argument.str = argument.str.substr(1, len - 1);
    } else if (argument.type == LispType::Q_Expression) {
        if (argument.cells.empty()) {
            throw std::invalid_argument("Error: the argument is empty Q-Expression");
        }
        argument.cells.erase(argument.cells.begin());
    } else {
        throw std::invalid_argument("Error: function tail takes string or Q-Expression");
    }
    return argument;
}

LispValue builtin_join(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
) {
    if (evaluated_arguments.size() < 1) {
        throw std::invalid_argument("Error: function join takes one or more arguments");
    }

    using cells_itr = std::vector<LispValue>::const_iterator;
    const cells_itr begin = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();

    if (all_type_of(evaluated_arguments, LispType::Q_Expression)) {
        LispValue& result(evaluated_arguments[0]);
        for (cells_itr itr = begin; itr != end; itr++) {
            result.cells.insert(result.cells.end(), itr->cells.begin(), itr->cells.end());
        }
        return result;
    } else if (all_type_of(evaluated_arguments, LispType::String)) {
        LispValue& result(evaluated_arguments[0]);
        for (cells_itr itr = begin; itr != end; itr++) {
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

    const bool reserved_symbol_exists = std::any_of(
        params.begin(), params.end(),
        [&environment](const LispValue& value) { return environment->is_reserved(value.symbol); }
    );
    if (reserved_symbol_exists) {
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

    const bool reserved_symbol_exists = std::any_of(
        symbols.begin(), symbols.end(),
        [&environment](const LispValue& value) { return environment->is_reserved(value.symbol); }
    );
    if (reserved_symbol_exists) {
        throw std::invalid_argument("Error: cannot re-define reserved symbol");
    }
    if (symbols.size() != evaluated_arguments.size() - 1) {
        throw std::invalid_argument("Error: cannot define incorrect number of values to symbols");
    }

    for (size_t index = 0, size = symbols.size(); index < size; index++) {
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

    const bool reserved_symbol_exists = std::any_of(
        signiture.begin() + 1, signiture.end(),
        [&environment](const LispValue& value) { return environment->is_reserved(value.symbol); }
    );
    if (reserved_symbol_exists) {
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

    for (const LispValue& symbol : symbols) {
        environment->delete_global(symbol.symbol);
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
    const std::function<int(int)>& unary_op,
    const std::function<int(int, int)>& binary_op,
    const std::string& name
) {
    size_t num_args = evaluated_arguments.size();
    if (num_args < 1) {
        throw std::invalid_argument("Error: operator " + name + " takes one or more arguments");
    }

    LispValue& result(evaluated_arguments[0]);
    if (num_args == 1) {
        if (result.type != LispType::Number) {
            throw std::invalid_argument("Error: operator " + name + " takes numbers");
        }
        result.number = unary_op(result.number);
        return result;
    }

    using cells_itr = std::vector<LispValue>::const_iterator;
    const cells_itr begin = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();

    for (cells_itr itr = begin; itr != end; itr++) {
        result.number = binary_op(result.number, itr->number);
    }
    return result;
}

inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& binary_op,
    const std::string& name
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: operator " + name + " takes two or more arguments");
    }
    if (!all_type_of(evaluated_arguments, LispType::Number)) {
        throw std::invalid_argument("Error: operator " + name + " takes numbers");
    }

    using cells_itr = std::vector<LispValue>::const_iterator;
    const cells_itr begin = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();

    LispValue& result(evaluated_arguments[0]);
    for (cells_itr itr = begin; itr != end; itr++) {
        result.number = binary_op(result.number, itr->number);
    }
    return result;
}

inline LispValue _operator(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int)>& unary_op,
    const std::string& name
) {
    if (evaluated_arguments.size() != 1) {
        throw std::invalid_argument("Error: operator " + name + " takes one argument");
    }
    if (evaluated_arguments[0].type != LispType::Number) {
        throw std::invalid_argument("Error: operator " + name + " takes number");
    }

    LispValue& result(evaluated_arguments[0]);
    result.number = unary_op(result.number);
    return result;
}

inline LispValue _relation(
    std::vector<LispValue>& evaluated_arguments,
    const std::function<int(int, int)>& relation,
    const std::string& name
) {
    if (evaluated_arguments.size() < 2) {
        throw std::invalid_argument("Error: relation " + name + " takes two or more arguments");
    }
    if (!all_type_of(evaluated_arguments, LispType::Number)) {
        throw std::invalid_argument("Error: relation " + name + " takes numbers");
    }

    using cells_itr = std::vector<LispValue>::const_iterator;
    const cells_itr begin = evaluated_arguments.begin() + 1, end = evaluated_arguments.end();

    int result = 1;
    for (cells_itr itr = begin; itr != end; itr++) {
        if (!relation((itr - 1)->number, itr->number))  {
            result = 0;
            break;
        }
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

inline int _posi(int x) {
    return x;
}

inline int _nega(int x) {
    std::numeric_limits<int> limits;
    if (x == limits.min()) throw std::overflow_error("Error: overflow occurs");
    return -x;
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

inline bool all_type_of(const std::vector<LispValue>& cells, LispType type) {
    for (const LispValue& value : cells) {
        if (value.type != type) return false;
    }
    return true;
}

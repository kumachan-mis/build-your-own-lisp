#include "evaluation.hpp"

#include <algorithm>
#include "builtin.hpp"
#include "lispvalue.hpp"


inline void add_builtin_function(
    const std::string& name,
    const LispBuiltinFunction& function,
    const std::shared_ptr<LispEnvironment>& environment
);
inline LispValue evaluate_symbol(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
);
inline LispValue evaluate_sexpr(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
);
inline LispValue evaluate_lambda_function_call(
    LispValue& lambda_function,
    const std::vector<LispValue>& evaluated_arguments
);


std::shared_ptr<LispEnvironment> global_environment() {
    std::shared_ptr<LispEnvironment> environment(new LispEnvironment());
    add_builtin_function("+", builtin_add, environment);
    add_builtin_function("-", builtin_sub, environment);
    add_builtin_function("*", builtin_mul, environment);
    add_builtin_function("/", builtin_div, environment);
    add_builtin_function("%", builtin_mod, environment);
    add_builtin_function("^", builtin_pow, environment);

    add_builtin_function("list", builtin_list, environment);
    add_builtin_function("eval", builtin_eval, environment);
    add_builtin_function("head", builtin_head, environment);
    add_builtin_function("tail", builtin_tail, environment);
    add_builtin_function("cons", builtin_cons, environment);
    add_builtin_function("join", builtin_join, environment);
    add_builtin_function("len",  builtin_len,  environment);

    add_builtin_function("if", builtin_if,  environment);
    add_builtin_function("==", builtin_eq,  environment);
    add_builtin_function("!=", builtin_neq, environment);
    add_builtin_function(">",  builtin_gt,  environment);
    add_builtin_function(">=", builtin_geq, environment);
    add_builtin_function("<",  builtin_lt,  environment);
    add_builtin_function("<=", builtin_leq, environment);
    add_builtin_function("&&", builtin_and, environment);
    add_builtin_function("||", builtin_or,  environment);
    add_builtin_function("!" , builtin_not, environment);

    add_builtin_function("lambda", builtin_lambda, environment);
    add_builtin_function("def",    builtin_def,    environment);
    add_builtin_function("defun",  builtin_defun,  environment);
    add_builtin_function("del",    builtin_del,    environment);

    add_builtin_function("type", builtin_type, environment);
    add_builtin_function("exit", builtin_exit, environment);

    return environment;
}

LispValue evaluate(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
) {
    switch (value.type) {
        case LispType::Unit:
        case LispType::Number:
        case LispType::String:
            /* End of evaluation */
            return value;
        case LispType::Symbol:
            return evaluate_symbol(value, environment);
        case LispType::BuiltinFunction:
        case LispType::LambdaFunction:
            /* End of evaluation */
            return value;
        case LispType::S_Expression:
            return evaluate_sexpr(value, environment);
        case LispType::Q_Expression:
            /* Stop evaluation */
            return value;
        default:
            throw std::invalid_argument("Error: Unknown type");
    }
}


inline void add_builtin_function(
    const std::string& symbol,
    const LispBuiltinFunction& function,
    const std::shared_ptr<LispEnvironment>& environment
) {
    environment->define(symbol, LispValue(LispType::BuiltinFunction, function, symbol));
}

inline LispValue evaluate_symbol(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
) {
    return environment->resolve(value);
}

inline LispValue evaluate_sexpr(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
) {
    const int num_cells = value.cells.size();
    for (int index = 0; index < num_cells; index++) {
        value.cells[index] = evaluate(value.cells[index], environment);
    }
    if (num_cells == 0) return LispValue();
    if (num_cells == 1) return value.cells[0];

    LispValue function(value.cells[0]);
    if (
        function.type != LispType::BuiltinFunction &&
        function.type != LispType::LambdaFunction
    ) {
        throw std::invalid_argument("Error: S-Expression does not start with function");
    }

    value.cells.erase(value.cells.begin());
    if (value.cells.size() == 1 && value.cells[0].type == LispType::Unit) {
        value.cells.clear();
    }

    if (function.type == LispType::BuiltinFunction) {
        return function.builtin_function(value.cells, environment);
    } else {
        return evaluate_lambda_function_call(function, value.cells);
    }
}

inline LispValue evaluate_lambda_function_call(
    LispValue& lambda_function,
    const std::vector<LispValue>& evaluated_arguments
) {
    std::vector<LispValue>& params(lambda_function.cells[0].cells);
    LispValue& body(lambda_function.cells[1]);
    std::shared_ptr<LispEnvironment>& local_env(lambda_function.local_environment);

    size_t expected_num = params.size(), given_num = evaluated_arguments.size();
    if (expected_num < given_num) {
        throw std::invalid_argument(
            "Error: lambda function takes " + std::to_string(expected_num) + " argument "
            "but " + std::to_string(given_num) + " were given"
        );
    }

    local_env = std::shared_ptr<LispEnvironment>(new LispEnvironment(*local_env));
    for (size_t index = 0; index < given_num; index++) {
        local_env->assign(params[index], evaluated_arguments[index]);
    }

    if (expected_num == given_num) {
        body.type = LispType::S_Expression;
        return evaluate(body, local_env);
    }

    params.erase(params.begin(), params.begin() + given_num);
    return lambda_function;
}

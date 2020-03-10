#ifndef _BUILTIN_HPP_
#define _BUILTIN_HPP_

#include <vector>
#include <memory>
#include "lispvalue.hpp"


LispValue builtin_add(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_sub(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_mul(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_div(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_mod(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_pow(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_if(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_eq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_neq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_gt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_geq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_lt(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_leq(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_list(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_head(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_tail(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_join(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_eval(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_cons(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_len(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_lambda(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_def(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_defun(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_del(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

LispValue builtin_exit(
    std::vector<LispValue>& evaluated_arguments,
    const std::shared_ptr<LispEnvironment>& environment
);

#endif  // _BUILTIN_HPP_

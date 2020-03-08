#ifndef _BUILTIN_HPP_
#define _BUILTIN_HPP_

#include <vector>
#include <string>
#include "lispvalue.hpp"


LispValue builtin_add(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_sub(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_mul(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_div(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_mod(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_pow(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_list(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_head(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_tail(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
    );
LispValue builtin_join(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_eval(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_cons(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_len(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_def(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_exit(
    const std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

#endif  // _BUILTIN_HPP_

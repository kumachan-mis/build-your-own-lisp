#ifndef _BUILTIN_HPP_
#define _BUILTIN_HPP_

#include <vector>
#include <string>
#include "lispvalue.hpp"


LispValue builtin_add(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_sub(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_mul(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_div(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_mod(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_pow(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_list(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_head(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_tail(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
    );
LispValue builtin_join(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_eval(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_cons(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);
LispValue builtin_len(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_def(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_del(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

LispValue builtin_exit(
    std::vector<LispValue>& evaluated_arguments,
    LispEnvironment& environment
);

#endif  // _BUILTIN_HPP_

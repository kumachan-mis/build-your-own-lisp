#ifndef _BUILTIN_HPP_
#define _BUILTIN_HPP_

#include <vector>
#include <string>
#include "lispvalue.hpp"


LispValue builtin_add(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_sub(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_mul(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_div(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_mod(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_pow(const std::vector<LispValue>& evaluated_arguments);

LispValue builtin_list(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_head(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_tail(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_join(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_eval(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_cons(const std::vector<LispValue>& evaluated_arguments);
LispValue builtin_len(const std::vector<LispValue>& evaluated_arguments);

#endif  // _BUILTIN_HPP_

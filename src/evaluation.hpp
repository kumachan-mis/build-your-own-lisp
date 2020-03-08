#ifndef _EVALUATION_HPP_
#define _EVALUATION_HPP_


#include "lispvalue.hpp"


LispEnvironment global_environment();
LispValue evaluate(LispValue& value, LispEnvironment& environment);

#endif  // _EVALUATION_HPP_

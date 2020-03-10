#ifndef _EVALUATION_HPP_
#define _EVALUATION_HPP_


#include "lispvalue.hpp"


std::shared_ptr<LispEnvironment> global_environment();
LispValue evaluate(
    LispValue& value,
    const std::shared_ptr<LispEnvironment>& environment
);

#endif  // _EVALUATION_HPP_

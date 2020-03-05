#ifndef _PARSING_HPP_
#define _PARSING_HPP_


#include "lib/mpc.hpp"
#include "lispvalue.hpp"


LispValue ast_to_lispvalue(mpc_ast_t* ast);

#endif  // _PARSING_HPP_

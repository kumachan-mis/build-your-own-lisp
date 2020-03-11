#ifndef _PARSER_HPP_
#define _PARSER_HPP_


#include <string>
#include "lispvalue.hpp"


LispValue parse(const std::string& input);

#endif  // _PARSER_HPP_

#include <string>
#include <stdexcept>
#include "lib/mpc.hpp"

#include "lispvalue.hpp"

#include <iostream>


LispValue ast_to_lispvalue(mpc_ast_t* ast);
inline LispValue ast_to_number(mpc_ast_t* ast);
inline LispValue ast_to_symbol(mpc_ast_t* ast);


LispValue ast_to_lispvalue(mpc_ast_t* ast) {
    if (std::string(ast->tag).find("number") != std::string::npos) {
        return ast_to_number(ast);
    }
    if (std::string(ast->tag).find("symbol") != std::string::npos) {
        return ast_to_symbol(ast);
    }

    LispValue s_expr;
    int children_num = ast->children_num;
    for (int index = 0; index < children_num; index++) {
        std::string contents(ast->children[index]->contents), tag(ast->children[index]->tag);
        if (contents == "(" || contents == ")" || tag == "regex") continue;
        s_expr.cells.push_back(ast_to_lispvalue(ast->children[index]));
    }
    return s_expr;
}

inline LispValue ast_to_number(mpc_ast_t* ast) {
    try {
        int number = std::stoi(ast->contents);
        return LispValue(number);
    } catch (const std::exception&) {
        throw std::out_of_range("Error: fail to convert to a number");
    }
}

inline LispValue ast_to_symbol(mpc_ast_t* ast) {
    return LispValue(ast->contents);
}

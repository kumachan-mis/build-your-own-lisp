#include <string>
#include <stdexcept>
#include "lib/mpc.hpp"
#include "lispvalue.hpp"


LispValue ast_to_lispvalue(mpc_ast_t* ast) {
     std::string tag(ast->tag);
     if (tag.find("unit") != std::string::npos) {
        return LispValue(LispType::Unit);
    }
    if (tag.find("number") != std::string::npos) {
        return LispValue(LispType::Number, ast->contents);
    }
    if (tag.find("symbol") != std::string::npos) {
        return LispValue(LispType::Symbol, ast->contents);
    }

    LispType expr_type;
    if (tag == ">" || tag.find("sexpr") != std::string::npos) {
        expr_type = LispType::S_Expression;
    } else if (tag.find("qexpr") != std::string::npos) {
        expr_type = LispType::Q_Expression;
    } else {
        throw std::invalid_argument("Error: unexpected tag \"" + tag + "\"");
    }

    LispValue expr(expr_type);
    int children_num = ast->children_num;
    for (int index = 0; index < children_num; index++) {
        std::string child_contents(ast->children[index]->contents);
        std::string child_tag(ast->children[index]->tag);
        if (
            child_contents == "(" || child_contents == ")" ||
            child_contents == "{" || child_contents == "}" ||
            child_tag == "regex"
        ) continue;
        expr.cells.push_back(ast_to_lispvalue(ast->children[index]));
    }
    return expr;
}

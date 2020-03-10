#include "lib/mpc.hpp"
#include "lispvalue.hpp"


LispValue ast_to_lispvalue(mpc_ast_t* ast) {
     std::string tag(ast->tag);
     if (tag.find("unit") != std::string::npos) {
        return LispValue();
    }
    if (tag.find("number") != std::string::npos) {
        int number = 0;
        try {
            number = std::stoi(ast->contents);
        } catch (std::exception&) {
            throw std::invalid_argument("Error: fail to convert to number");
        }
        return LispValue(LispType::Number, number);
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

    std::vector<LispValue> cells;
    int children_num = ast->children_num;
    for (int index = 0; index < children_num; index++) {
        std::string child_contents(ast->children[index]->contents);
        std::string child_tag(ast->children[index]->tag);
        if (
            child_contents == "(" || child_contents == ")" ||
            child_contents == "{" || child_contents == "}" ||
            child_tag == "regex"
        ) continue;
        cells.push_back(ast_to_lispvalue(ast->children[index]));
    }
    return LispValue(expr_type, cells);
}

#include <iostream>
#include <string>
#include <stdexcept>
#include "operation.hpp"
#include "lib/mpc.hpp"

int evaluate(mpc_ast_t* ast);
int evaluate_operator(std::string op, int x, int y);
int evaluate_operator(std::string op, int x);

int evaluate(mpc_ast_t* ast) {
    if (std::string(ast->tag).find("number") != std::string::npos) {
        try {
            int number = std::stoi(ast->contents);
            return number;
        } catch (const std::exception&) {
            throw std::out_of_range("Error: fail to convert to a number");
        }
    }

    std::string op(ast->children[1]->contents);
    int ret = evaluate(ast->children[2]);
    if (ast->children_num == 4) return evaluate_operator(op, ret);
    int children_num = ast->children_num;
    for (int index = 3; index < children_num - 1; index++) {
        ret = evaluate_operator(op, ret, evaluate(ast->children[index]));
    }
    return ret;
}

int evaluate_operator(std::string op, int x, int y) {
    if (op == "+") return safe_add(x, y);
    if (op == "-") return safe_sub(x, y);
    if (op == "*") return safe_mul(x, y);
    if (op == "/") return safe_div(x, y);
    if (op == "%") return safe_mod(x, y);
    if (op == "^") return safe_pow(x, y);
    throw std::invalid_argument("Error: operator \"" + op + "\" is not supported");
}

int evaluate_operator(std::string op, int x) {
    if (op == "+") return  x;
    if (op == "-") return safe_neg(x);
    throw std::invalid_argument("Error: operator \"" + op + "\" is not supported");
}
#include "lib/mpc.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <editline/readline.h>

int main(int argc, char* argv[]) {
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");
  
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                     \
        number   : /-?[0-9]+/ ;                             \
        operator : '+' | '-' | '*' | '/' ;                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
        ",
        Number, Operator, Expr, Lispy);

    std::cout << "Build Your Own Lisp" << std::endl;
    std::cout << "Press ctrl+c to Exit" << std::endl;

    while (true) {
        std::unique_ptr<char> input(readline("lisp>> "));
        add_history(input.get());
        mpc_result_t r;
        if (mpc_parse("<stdin>", input.get(), Lispy, &r)) {
            mpc_ast_print((mpc_ast_t*)r.output);
            mpc_ast_delete((mpc_ast_t*)r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
    return 0;
}

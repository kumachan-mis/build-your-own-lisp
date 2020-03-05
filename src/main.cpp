#include <iostream>
#include <vector>
#include <memory>
#include <editline/readline.h>
#include "lib/mpc.hpp"

#include "evaluation.hpp"


int main(int argc, char* argv[]) {
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr  = mpc_new("sexpr");
    mpc_parser_t* Expr   = mpc_new("expr");
    mpc_parser_t* Lispy  = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                \
        number : /(+|-)?[0-9]+/ ;                    \
        symbol : '+' | '-' | '*' | '/' | '%' | '^' ; \
        sexpr  : '(' <expr>* ')' ;                   \
        expr   : <number> | <symbol> | <sexpr> ;     \
        lispy  : /^/ <expr>* /$/ ;                   \
    ",
    Number, Symbol, Sexpr, Expr, Lispy);

    std::cout << "Build Your Own Lisp" << std::endl;
    std::cout << "Press ctrl+c to Exit" << std::endl;

    while (true) {
        std::unique_ptr<char> input(readline("lisp>> "));
        add_history(input.get());
        mpc_result_t r;
        if (mpc_parse("<stdin>", input.get(), Lispy, &r)) {
            try {
                int result = evaluate((mpc_ast_t*)r.output);
                std::cout << result << std::endl;
            } catch (const std::exception& exception) {
                std::cerr << exception.what() << std::endl;
            }
            mpc_ast_delete((mpc_ast_t*)r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }
    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);
    return 0;
}

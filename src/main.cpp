#include <iostream>
#include <vector>
#include <memory>
#include <editline/readline.h>
#include "lib/mpc.hpp"

#include "lispvalue.hpp"
#include "parsing.hpp"
#include "evaluation.hpp"


int main(int argc, char* argv[]) {
    mpc_parser_t* Unit   = mpc_new("unit");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr  = mpc_new("sexpr");
    mpc_parser_t* Qexpr  = mpc_new("qexpr");
    mpc_parser_t* Expr   = mpc_new("expr");
    mpc_parser_t* Lispy  = mpc_new("lispy");

    mpca_lang(
        MPCA_LANG_DEFAULT,
        "                                                                \
            unit   : '(' ')' ;                                           \
            number : /-?[0-9]+/ ;                                        \
            symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;                  \
            sexpr  : '(' <expr>+ ')' ;                                   \
            qexpr  : '{' <expr>* '}' ;                                   \
            expr   : <unit> | <number> | <symbol> | <sexpr> | <qexpr> ;  \
            lispy  : /^/ <expr>* /$/ ;                                   \
        ",
        Number, Symbol, Unit, Sexpr, Qexpr, Expr, Lispy
    );

    LispEnvironment global_env = global_environment();

    std::cout << "Build Your Own Lisp" << std::endl;
    std::cout << "Press ctrl+c to Exit" << std::endl;

    while (true) {
        std::unique_ptr<char> input(readline("lisp>> "));
        add_history(input.get());
        mpc_result_t r;
        if (mpc_parse("<stdin>", input.get(), Lispy, &r)) {
            try {
                LispValue value = ast_to_lispvalue((mpc_ast_t*)r.output);
                std::cout << evaluate(value, global_env) << std::endl;
            } catch (const std::exception& exception) {
                std::cerr << exception.what() << std::endl;
            }
            mpc_ast_delete((mpc_ast_t*)r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }
    mpc_cleanup(7, Number, Symbol, Unit, Sexpr, Qexpr, Expr, Lispy);
    return 0;
}

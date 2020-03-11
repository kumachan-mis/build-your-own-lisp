#include <iostream>
#include <editline/readline.h>

#include "lispvalue.hpp"
#include "parser.hpp"
#include "evaluation.hpp"


int main(int argc, char* argv[]) {
    std::shared_ptr<LispEnvironment> global_env = global_environment();

    std::cout << "Build Your Own Lisp" << std::endl;
    std::cout << "Press ctrl+c to Exit" << std::endl;

    while (true) {
        std::string input(readline(">>> "));
        add_history(input.c_str());
        LispValue value;
        try {
            value = parse(input);
            value = evaluate(value, global_env);
        } catch (const std::exception& exception) {
            std::cerr << exception.what() << std::endl;
            continue;
        }
        if (value.type != LispType::Unit) {
            std::cout << value << std::endl;
        }
    }
    return 0;
}

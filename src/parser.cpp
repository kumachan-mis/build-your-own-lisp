#include "parser.hpp"

#include <string>
#include "lispvalue.hpp"


const std::string white_spaces(" \t\r\n\f");
const std::string symbol_characters(
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPPQRSTUVWXYZ_+-*/%^=<>&|!");
const std::string number_characters("0123456789");
const char positive_sign = '+', negative_sign = '-';
const char sexpr_lparen = '(', sexpr_rparen = ')';
const char qexpr_lparen = '{', qexpr_rparen = '}';
const char string_paren = '\"', string_escape = '\\';


LispValue parse_lisp(const std::string& input, size_t& pos);
LispValue parse_atom(const std::string& input, size_t& pos);
LispValue parse_string(const std::string& input, size_t& pos);
LispValue parse_expr(const std::string& input, size_t& pos);
inline void skip_whitespaces(const std::string& input, size_t& pos);
inline std::string error_message(size_t pos, const std::string& message, bool is_end = false);


LispValue parse(const std::string& input) {
    size_t pos = 0;
    LispValue result = parse_lisp(sexpr_lparen + input + sexpr_rparen, pos);
    if (pos - 2 != input.length()) {
        throw std::invalid_argument(error_message(pos, "Error: fail to parse input"));
    }
    return result;
}

LispValue parse_lisp(const std::string& input, size_t& pos) {
    skip_whitespaces(input, pos);
    if (pos == input.length()) {
        throw std::invalid_argument(error_message(pos, "Error: unexpected end of input", true));
    }
    
    LispValue result;
    const char c = input[pos];
    if (
        symbol_characters.find(c) != std::string::npos || 
        number_characters.find(c) != std::string::npos
    ) {
        result = parse_atom(input, pos);
    } else if (c == string_paren) {
        result = parse_string(input, pos);
    } else if (c == sexpr_lparen || c == qexpr_lparen) {
        result = parse_expr(input, pos);
    } else {
        throw std::invalid_argument(error_message(pos, "Error: fail to parse input"));
    }
    skip_whitespaces(input, pos);
    return result;
}

LispValue parse_atom(const std::string& input, size_t& pos) {
    size_t top = pos;
    char c = input[pos];
    std::string atom;
    while (
        symbol_characters.find(c) != std::string::npos ||
        number_characters.find(c) != std::string::npos
    ) {
        atom.push_back(input[pos]);
        pos++;
        c = input[pos];
    }

    if (
        number_characters.find(atom[0]) == std::string::npos &&
        (atom.length() < 2 ||
         (atom[0] != positive_sign && atom[0] != negative_sign) ||
         number_characters.find(atom[1]) == std::string::npos)
    ) {
        return LispValue(LispType::Symbol, atom);
    }
    
    for (size_t index = 1; index < atom.length(); index++) {
        if (number_characters.find(atom[index]) == std::string::npos) {
            throw std::invalid_argument(
                error_message(top, "Error: invalid symbol \"" + atom + "\""));
        }
    }
    try {
        return LispValue(LispType::Number, std::stoi(atom));
    } catch (const std::exception&) {
        throw std::invalid_argument(error_message(top, "Error: fail to convert to a number"));
    }
}

LispValue parse_string(const std::string& input, size_t& pos) {
    size_t begin = pos + 1;
    while (true) {
        pos++;
        if (pos == input.length()) {
            throw std::invalid_argument(
                error_message(pos, "Error: unexpected end of input", true));
        }
        if (input[pos] == string_paren) break;
    }
    std::string str(input.substr(begin, pos - begin));
    pos++;
    return LispValue(LispType::String, str);
}

LispValue parse_expr(const std::string& input, size_t& pos) {
    const char lparen = input[pos];
    const char rparen = lparen == sexpr_lparen? sexpr_rparen : qexpr_rparen;
    LispValue result(lparen == sexpr_lparen? LispType::S_Expression : LispType::Q_Expression);
    pos++;
    while (input[pos] != rparen) {
        result.cells.push_back(parse_lisp(input, pos));
    }
    pos++;
    return result;
}

inline void skip_whitespaces(const std::string& input, size_t& pos) {
    char c = input[pos];
    while (pos < input.length() && white_spaces.find(c) != std::string::npos) {
        pos++;
        c = input[pos];
    }
}

inline std::string error_message(size_t pos, const std::string& message, bool is_end) {
    pos -= is_end? 2 : 1;
    const size_t offset = 4;
    std::string ret;
    for (size_t index = 0; index < pos + offset; index++) {
        ret.push_back('~');
    }
    ret += "^\n" + message;
    return ret;
}

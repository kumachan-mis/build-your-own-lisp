#ifndef _LISPVALUE_HPP_
#define _LISPVALUE_HPP_


#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


enum class LispType {
    Unit,
    Number,
    Symbol,
    BuiltinFunction,
    LambdaFunction,
    S_Expression,
    Q_Expression
};

struct LispValue;
struct LispEnvironment;
using  LispBuiltinFunction = std::function<LispValue(std::vector<LispValue>&, LispEnvironment&)>;
class LispLambdaFunction {
    std::vector<LispValue> argument_symbols;
    LispValue* body_qexpr;
    LispEnvironment* local_environment;
};

class LispValue {
    public:
        LispType type;
        int number;
        std::string symbol;
        LispBuiltinFunction builtin_function;
        LispLambdaFunction lambda_function;
        std::vector<LispValue> expr_cells;

        LispValue()
        : type(LispType::Unit), number(), symbol(), builtin_function(), lambda_function(), expr_cells()
        {}

        LispValue(LispType _type, const int value) 
        : type(_type), number(value), symbol(), builtin_function(), lambda_function(), expr_cells()
        {
            if (type != LispType::Number) {
                throw std::invalid_argument("Error: type is not number");
            }
        }

        LispValue(LispType _type, const std::string& value) 
        : type(_type), number(), symbol(value), builtin_function(), lambda_function(), expr_cells()
        {
            if (type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }
        }

        LispValue(LispType _type, const LispBuiltinFunction& value) 
        : type(_type), number(), symbol(), builtin_function(value), lambda_function(), expr_cells()
        {
            if (type != LispType::BuiltinFunction) {
                throw std::invalid_argument("Error: type is not built-in function");
            }
        }

         LispValue(LispType _type, const LispLambdaFunction& value) 
        : type(_type), number(), symbol(), builtin_function(), lambda_function(value), expr_cells()
        {
            if (type != LispType::LambdaFunction) {
                throw std::invalid_argument("Error: type is not lambda function");
            }
        }

        LispValue(LispType _type, const std::vector<LispValue>& value) 
        : type(_type), number(), symbol(), builtin_function(), lambda_function(), expr_cells(value)
        {
            if (type != LispType::S_Expression && type != LispType::Q_Expression) {
                throw std::invalid_argument("Error: type is not expression");
            }
        }
    friend std::ostream& operator<<(std::ostream& os, const LispValue& value);
};

std::ostream& operator<<(std::ostream& os, const LispValue& value);

struct LispEnvironment {
    public:
        /*
        Requires
            symbol: type should be Lisptype::Symbol
        */
        LispValue find(const LispValue& symbol) const {
            auto itr = _map.find(symbol.symbol);
            if (itr == _map.end()) throw std::out_of_range("Error: unbound symbol \"" + symbol.symbol + "\"");
            return itr->second;
        }

        LispValue find(const std::string& symbol) const {
            auto itr = _map.find(symbol);
            if (itr == _map.end()) throw std::out_of_range("Error: unbound symbol \"" + symbol + "\"");
            return itr->second;
        }

        /*
        Requires
            symbol: type should be Lisptype::Symbol
        */
        void define(const LispValue& symbol, const LispValue& value) {
            _map[symbol.symbol] = value;
        }

        void define(const std::string& symbol, const LispValue& value) {
            _map[symbol] = value;
        }

        /*
        Requires
            symbol: type should be Lisptype::Symbol
        */
        void erase(const LispValue& symbol) {
            _map.erase(symbol.symbol);
        }

        void erase(const std::string& symbol) {
            _map.erase(symbol);
        }

    private:
        std::unordered_map<std::string, LispValue> _map;
        LispEnvironment* _parent_environment;
};


#endif // _LISPVALUE_HPP_

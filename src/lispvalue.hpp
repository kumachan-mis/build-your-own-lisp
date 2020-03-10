#ifndef _LISPVALUE_HPP_
#define _LISPVALUE_HPP_


#include <string>
#include <vector>
#include <array>
#include <functional>
#include <unordered_map>
#include <memory>


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
using  LispBuiltinFunction = std::function<LispValue(std::vector<LispValue>&, const std::shared_ptr<LispEnvironment>&)>;

class LispValue {
    public:
        LispType type;
        int number;
        std::string symbol;
        LispBuiltinFunction builtin_function;
        std::shared_ptr<LispEnvironment> local_environment;
        std::vector<LispValue> cells;

        LispValue()
        : type(LispType::Unit), number(), symbol(), builtin_function(), local_environment(), cells()
        {}

        LispValue(LispType _type, const int value) 
        : type(_type), number(value), symbol(), builtin_function(), local_environment(), cells()
        {
            if (type != LispType::Number) {
                throw std::invalid_argument("Error: type is not number");
            }
        }

        LispValue(LispType _type, const std::string& value) 
        : type(_type), number(), symbol(value), builtin_function(), local_environment(), cells()
        {
            if (type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }
        }

        LispValue(LispType _type, const LispBuiltinFunction& value, const std::string& _symbol) 
        : type(_type), number(), symbol(_symbol), builtin_function(value), local_environment(), cells()
        {
            if (type != LispType::BuiltinFunction) {
                throw std::invalid_argument("Error: type is not built-in function");
            }
        }

         LispValue(
             LispType _type,
             const std::vector<LispValue>& args_and_body,
             const std::shared_ptr<LispEnvironment>& environment
        ) 
        : type(_type), number(), symbol(), builtin_function(), local_environment(environment), cells(args_and_body)
        {
            if (type != LispType::LambdaFunction) {
                throw std::invalid_argument("Error: type is not lambda function");
            }
        }

        LispValue(LispType _type, const std::vector<LispValue>& value) 
        : type(_type), number(), symbol(), builtin_function(), local_environment(), cells(value)
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
        LispEnvironment(
            const std::shared_ptr<LispEnvironment>& parent = std::shared_ptr<LispEnvironment>()
        )
        : _map(), _parent_environment(parent)
        {}

        LispValue find(const LispValue& symbol) const {
            if (symbol.type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }
      
            auto itr = _map.find(symbol.symbol);
            if (itr != _map.end()) return itr->second;
            else if (_parent_environment) return _parent_environment->find(symbol);
            throw std::out_of_range("Error: unbound symbol \"" + symbol.symbol + "\"");
        }

        LispValue find(const std::string& symbol) const {
            auto itr = _map.find(symbol);
            if (itr != _map.end()) return itr->second;
            else if (_parent_environment) return _parent_environment->find(symbol);
            throw std::out_of_range("Error: unbound symbol \"" + symbol + "\"");
        }

        void define(const LispValue& symbol, const LispValue& value) {
            if (symbol.type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }

            if (!_parent_environment) {
                _map[symbol.symbol] = value;
            } else {
                _parent_environment->define(symbol, value);
            }
        }

        void define(const std::string& symbol, const LispValue& value) {
            if (!_parent_environment) {
                _map[symbol] = value;
            } else {
                _parent_environment->define(symbol, value);
            }
        }

        void erase(const LispValue& symbol) {
            if (symbol.type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }

            if (!_parent_environment) {
                _map.erase(symbol.symbol);
            } else {
                _parent_environment->erase(symbol);
            }
        }

        void erase(const std::string& symbol) {
            if (!_parent_environment) {
                _map.erase(symbol);
            } else {
                _parent_environment->erase(symbol);
            }
        }

        void assign(const LispValue& symbol, const LispValue& value) {
            if (symbol.type != LispType::Symbol) {
                throw std::invalid_argument("Error: type is not symbol");
            }

            _map[symbol.symbol] = value;
        }

        void assign(const std::string& symbol, const LispValue& value) {
            _map[symbol] = value;
        }

    private:
        std::unordered_map<std::string, LispValue> _map;
        std::shared_ptr<LispEnvironment> _parent_environment;
};


#endif // _LISPVALUE_HPP_

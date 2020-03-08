#ifndef _LISPVALUE_HPP_
#define _LISPVALUE_HPP_


#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


enum struct LispType {
    Unit,
    Number,
    Symbol,
    Function,
    S_Expression,
    Q_Expression
};

struct LispValue;
struct LispEnvironment;
typedef std::function<LispValue(const std::vector<LispValue>&, LispEnvironment&)> LispBuiltinFunction;

struct LispValue {
    LispType type;
    std::string value;
    LispBuiltinFunction function;
    std::vector<LispValue> cells;

    LispValue(const LispType& _type = LispType::Unit): type(_type), value(), cells() {}
    LispValue(const LispType& _type, const std::string& _value): type(_type), value(_value), cells() {}
    int to_int() const {
        if (this->type != LispType::Number) {
            throw std::invalid_argument("Error: argument type is not number");
        }
        try {
            return std::stoi(this->value);
        } catch (...) {
            throw std::invalid_argument("Error: fail to convert argument to number");
        }
    }
};
std::ostream& operator<<(std::ostream& os, const LispValue& value);

struct LispEnvironment {
    /*
    Requires
        symbol: type should be Lisptype::Symbol
    */
    LispValue find(const LispValue& symbol) const {
        auto itr = map.find(symbol.value);
        if (itr == map.end()) throw std::out_of_range("Error: unbound symbol \"" + symbol.value + "\"");
        return itr->second;
    }

    LispValue find(const std::string& symbol) const {
        auto itr = map.find(symbol);
        if (itr == map.end()) throw std::out_of_range("Error: unbound symbol \"" + symbol + "\"");
        return itr->second;
    }

    /*
    Requires
        symbol: type should be Lisptype::Symbol
    */
    void define(const LispValue& symbol, const LispValue& value) {
        map[symbol.value] = value;
    }

    void define(const std::string& symbol, const LispValue& value) {
        map[symbol] = value;
    }

    private:
        std::unordered_map<std::string, LispValue> map;
};


#endif // _LISPVALUE_HPP_

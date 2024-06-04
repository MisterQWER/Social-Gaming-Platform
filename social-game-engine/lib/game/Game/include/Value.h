#pragma once

#include <map>
#include <variant>
#include <string_view>
#include <vector>
#include "Configuration.h"

using std::string_view; using std::vector; using std::string;
using std::map; using std::variant;

// This code segment is required to for the print function
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


struct Value {
    variant<
        map<string_view, string_view> ,map<string_view, Value>, Configuration, IntegerKind
        , BooleanKind, StringKind, EnumKind, pair<int,int>
        , vector<Value>, string_view, string, int, bool>   variantValue;

    map<string_view, Value> valueMap;

    bool isEmpty() const {
        auto isValEmptyVisitor = [] (const auto& arg) -> bool {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, Value>) {
                return arg == Value {};
            }
            return false;
        };
        
        return visit(isValEmptyVisitor, variantValue);
    }

    static void accessValue(Value& val, const string_view key) {
        auto accessVisitor = [&key, &val] (auto&& arg) {
            using T = decay_t<decltype(arg)>;
            
            if constexpr (is_same_v<T, map<string_view, Value>> 
                            || is_same_v<T, map<string_view, string_view>>) {
                if (arg.find(key) != arg.end()) 
                    val = Value { arg[key] };
                else 
                    val = Value {};
            }

            else if constexpr (is_same_v<T, Configuration>) {
                if (key == "name") 
                    val = Value {arg.getName()};
                
                else if (key == "player_range") 
                    val = Value {arg.getPlayerRange()};
                
                else if(key == "rounds") {                
                    auto setup_map = arg.getSetup();
                    val = Value{ get<IntegerKind> (setup_map.at(string(key))) }; 
                }
                else 
                    val = Value{};
            }

            else {
                val = Value{};
            }
        };

        visit(accessVisitor, val.variantValue); 
    }

    void print() const {
        visit(
            overloaded{
                [](const map<string_view, Value>& mapVal) {
                    cout << "Map: {";
                    for (const auto& [key, val] : mapVal) {
                        cout << key << ": ";
                        val.print();
                        cout << ", ";
                    }
                    cout << "}";
                },
                [](const map<string_view, string_view>& mapVal) {
                    cout << "Map<string_view, string_view>: {";
                    for (const auto& [key, val] : mapVal) {
                        std::cout << key << ": " << val << ", ";
                    }
                    cout << "}";
                },
                [](const Configuration& config) { cout << "Configuration"; },
                [](const IntegerKind& integerKind) {
                     cout << "IntegerKind. min: " << integerKind.minVal << " , maxVal: " << integerKind.maxVal; 
                },
                [](const BooleanKind& booleanKind) { cout << "BooleanKind"; },
                [](const StringKind& stringKind) { cout << "StringKind"; },
                [](const EnumKind& enumKind) { cout << "EnumKind"; },
                [](const pair<int, int>& intPair) { 
                    cout << "Pair: (" << intPair.first << ", " << intPair.second << ")";
                },
                [](const vector<Value>& vec) { 
                    cout << "Vector: [";
                    for (const auto& val : vec) {
                        val.print();
                        cout << ", ";
                    }
                    cout << "]";
                },
                [](string_view sv) { cout << "String_view: " << sv; },
                [](const string& str) { cout << "String: " << str; },
                [](int i) { cout << "Int: " << i; },
                [](bool b) { cout << "Bool: " << std::boolalpha << b; },

            },
            variantValue);
    }
};
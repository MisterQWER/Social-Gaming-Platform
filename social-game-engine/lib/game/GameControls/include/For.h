#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <cpp-tree-sitter.h>

#include "BaseRule.h"
#include "RuleContainer.h"
    
using InnerDriver = std::variant<int, bool, string, pair<int, int>, vector<int>>;
using Driver = std::variant<int, bool, string, pair<int, int>, vector<int>, InnerDriver, unordered_map<InnerDriver, InnerDriver>>;

// ======= Fake Data Variant Custom Hash function Implementation section =================
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t seed = v.size();
        for(auto& i : v) {
            // just some hash funciton found online
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        size_t seed = p.first;     
        seed ^= p.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= p.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct InnerDriverHash {
    size_t operator()(const InnerDriver& driver) const {
        return std::visit([this](auto&& value) -> size_t {
            using T = decay_t<decltype(value)>;
            if constexpr (is_same_v<T, vector<int>>) {
                // Use the custom hash function for vectors.
                VectorHash vectorHash;
                return vectorHash(value);
            }
            else if constexpr (is_same_v<T, pair<int, int>>) {
                // Use the custom hash function for vectors.
                PairHash pairhash;
                return pairhash(value);
            }
            // for other types use the the std::hash available
            else return hash<T>{}(value);
        }, driver);
    }
};

namespace std {
    template<> 
    struct hash<InnerDriver> {
        size_t operator()(const InnerDriver& v) const noexcept {
            return InnerDriverHash
        {}(v);
        }
    };
}

struct DriverHash {
    size_t operator()(const Driver& driver) const {
        return visit([&](auto&& value) -> size_t {

            using T = decay_t<decltype(value)>;
            
            if constexpr (is_same_v<T, vector<int>>) {
                // Handle vector<int> specifically if required, like in InnerDriverHash
                VectorHash vectorHash;
                return vectorHash(value);
            }
            else if constexpr (is_same_v<T, pair<int, int>>) {
                // Handle vector<int> specifically if required, like in InnerDriverHash
                PairHash pairHash;
                return pairHash(value);
            }
            else if constexpr (is_same_v<T, unordered_map<InnerDriver, InnerDriver>>) {
                
                //hashing funciton for map
                size_t seed = value.size();
                InnerDriverHash innerDriverHasher;
                for (const auto& element : value) {
                    seed ^= innerDriverHasher(element.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                    seed ^= innerDriverHasher(element.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
                return seed;
            } 
            // other types
            else return hash<T>{}(value);
        }, driver);
    }
};

namespace std {
    template<> 
    struct hash<Driver> {
        size_t operator()(const Driver& v) const noexcept {
            DriverHash driverHash;
            return driverHash(v);
        }
    };
}
// end section


class For : public BaseRule {
    public:
        For(const string_view&, const vector<StringExpressionTypePair>&, RuleContainer);
        void addEventsTo(GameInstance& game) override;

        Driver getForDriver() const noexcept {return  driver;}
        string getVariableString() const noexcept {return list_element;}
        void printForObject() noexcept;

        bool isFinished();
        void step(); 
        
        // For type checking 
        uintptr_t GetType() override {
            return Type();
        } 

        static uintptr_t Type() {
            return reinterpret_cast<uintptr_t>(&type);
        }

        void accept(RuleVisitor& visitor) const override {
            visitor.visit(*this);
        }

        std::optional<std::vector<std::shared_ptr<BaseRule>>> GetSubRules() const override {
            return sub_rules.getSubRules();
        }
    private:

        static constexpr int type = 0;
        Driver driver;
        string list_element;
        vector<StringExpressionTypePair> expression_list; // not needed for the caller
        RuleContainer sub_rules;
        bool isDone;
    
        // Fake data implementation for test
        unordered_map<string, Driver> fake_table;
        unordered_map<string, Driver> fake_var_table;
        unordered_map<InnerDriver, InnerDriver>  fake_config_map;

        // main logic methods
        Driver evaluate_for_exp() noexcept;
        void evaluate_for_arg(const string& builtin_str);
        Driver accessDriverTable(Driver& driver, const string&);
        void cleanUp();
        
        // printing methods
        static void printInnerDriver(const InnerDriver&) noexcept;
        static void printDriver(const Driver&) noexcept;

};

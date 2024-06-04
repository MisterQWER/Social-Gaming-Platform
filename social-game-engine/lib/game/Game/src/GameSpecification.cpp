
#include "GameSpecification.h"

using GS = GameSpecification;

GS::GameSpecification(ts::Node* root, const string& sourcecode) {
    
    // Resolving Configuration object and inserting into table
    ts::Node config_node = root->getChildByFieldName("configuration");
    Configuration config(&config_node, sourcecode);
    Value config_value;
    config_value.variantValue = config;
    
    vector<StringExpressionTypePair> configExpression = {
    {"configuration", ExpressionType::identifier}};
    
    insert_entry_by_keychain(config_value, configExpression);

    // Resolving Constant object and insert its elements to the table
    ts::Node constant_node = root->getChildByFieldName("constants");
    
    // Hardcode for now:
    map<string_view, Value> weapons_map {
    {string_view("Rock"), Value{string_view("Scissors")}},
    {string_view("Paper"), Value{string_view("Rock")}},
    {string_view("Scissors"),  Value{string_view("Paper")}}
    };

    vector<pair<string_view, ExpressionType>> weapons_table_path = {
        {"weapons", ExpressionType::identifier}
    }; 
    insert_entry_by_keychain(Value{weapons_map}, weapons_table_path);

    // Resolving Variable Object and inserting its elements to the table
    ts::Node variable_node = root->getChildByFieldName("variables"); 
    // Variables variable(&variable_node, string(sourcecode));

    // auto var_map = variable.getVariablesMap();
    // for (const auto& entry : var_map) {
    //     Value val;
    //     val.variantValue = entry.second;
    //     insert_value_by_key(val, entry.first);
    // }

    // Initialize players entry of the table to -1 [Empty Player list]
    vector<StringExpressionTypePair> playersExpression = {
    {"players", ExpressionType::identifier}};
    insert_entry_by_keychain(Value{-1}, playersExpression);
}

GS::GameSpecification(const GameSpecification& other) {
    entries = other.entries;
}

GameSpecification& GS::operator=(const GameSpecification& other) {
    if (this != &other) 
        entries = other.entries;
    
    return *this;
}

Result GS::insert_entry_by_keychain(const Value& val, vector<StringExpressionTypePair>& keychain) {
    // Case 1: Empty keychain [Fail]
    if (keychain.empty()) 
        return Result::FAIL;

    // Case 2: the keychain has one element
    if (keychain.size() == 1) {
        
        // Case 2.1: element already exists in table [FAIL]
        if (entries.find(keychain[0].first) != entries.end())
            return Result::FAIL;
        
        // Case 2.2: [SUCCESS]
        entries[keychain[0].first] = val;
        return Result::SUCCESS;
    }

    Value* curr = &entries[keychain[0].first];
    Value** currPtr = &curr;    // currPtr is needed to be able to mutate currPtr in lambda

    for (size_t i = 1; i < keychain.size() - 1 && *currPtr; i++) {
        auto accessVisitor = [currPtr, &keychain, i](auto&& arg) {
            using T = decay_t<decltype(arg)>;

            if constexpr (is_same_v<T, map<string_view, Value>>) {
                if (arg.find(keychain[i].first) != arg.end()) 
                    *currPtr = &arg.find(keychain[i].first)->second;
                else 
                    *currPtr = nullptr;
            } 
            else 
                *currPtr = nullptr;
        };
        visit(accessVisitor, (*currPtr)->variantValue);
    }

    // Case 3: Accessing through the keychain was not valid
    if (!(*currPtr)) return Result::FAIL;

    // curr should be the map where we want to insert the val
    if (auto* targetMap = std::get_if<map<string_view, Value>>(&(*currPtr)->variantValue)) {
        string_view finalKey = keychain.back().first;
        
        // Case 1.1 : keychain was valid but the value is already in table
        if (targetMap->find(finalKey) != targetMap->end()) 
            return Result::FAIL;

        // Case 4: curr is valid and accessing was successful, [Inserting]
        (*targetMap)[finalKey] = val;
        return Result::SUCCESS;
    }

    return Result::FAIL;
}

Result GS::modify_entry_by_keychain(const Value& newVal, vector<StringExpressionTypePair>& keychain) {
    // Case 1: Empty keychain [Fail]
    if (keychain.empty() || entries.find(keychain[0].first) == entries.end()) 
        return Result::FAIL;

    // Case 2: the keychain has one element
    if (keychain.size() == 1) {
        // Case 2.1: element does not exist in table [FAIL]
        if (entries.find(keychain[0].first) == entries.end())
            return Result::FAIL;

        // Case 2.2: Modify the existing value [SUCCESS]
        entries[keychain[0].first] = newVal;
        return Result::SUCCESS;
    }

    Value* curr = &entries[keychain[0].first];
    for (size_t i = 1; i < keychain.size() - 1 && curr; i++) {
        auto accessVisitor = [&curr, &keychain, i](auto&& arg) {
            using T = decay_t<decltype(arg)>;

            if constexpr (is_same_v<T, map<string_view, Value>>) {
                auto it = arg.find(keychain[i].first);
                if (it != arg.end()) 
                    curr = &it->second;
                else 
                    curr = nullptr; // Path broken, key not found
            } else {
                curr = nullptr; // Path broken, not a map
            }
        };


        if (curr) visit(accessVisitor, curr->variantValue);
    }

    // Case 3: Accessing through the keychain was not valid
    if (!curr) return Result::FAIL;

    // curr should be the map where we want to modify the val
    if (auto* targetMap = std::get_if<map<string_view, Value>>(&curr->variantValue)) {
        string_view finalKey = keychain.back().first;
        
        // Case 3.1: Key does not exist in the final target map
        if (targetMap->find(finalKey) == targetMap->end()) 
            return Result::FAIL;

        // Case 4: curr is valid and accessing was successful, [Modifying]
        (*targetMap)[finalKey] = newVal;
        return Result::SUCCESS;
    }

    return Result::FAIL;
}

std::optional<Value> 
GS::get_entry_by_keychain(const vector<StringExpressionTypePair>& keychain) const noexcept {
    
    if (keychain.empty() || entries.find(keychain[0].first) == entries.end()) 
        return std::nullopt;

    Value val = entries.at(keychain[0].first);

    // tempKeys is the vector of identifier string_views of the keychain
    vector<string_view> tempKeys;

    for (int i = 1; i < keychain.size(); i++) 
        if(keychain[i].second == ExpressionType::identifier)
            tempKeys.push_back(keychain[i].first);

    // Accessing the nested values 
    for (const auto& key : tempKeys) 
       Value::accessValue(val, key);     
    
    if(val.isEmpty()) return std::nullopt;
    return val;
}

void GS::print() const {
    cout << "\n >> Print Table <<" << endl;
    for (const auto& entry : entries) {
        cout << entry.first << ": ";
        entry.second.print();
        cout << endl;
    }
    cout << ">> <<" << endl << endl;
}
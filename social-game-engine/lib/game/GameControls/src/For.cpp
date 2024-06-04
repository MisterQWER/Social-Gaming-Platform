#include "For.h"

// TODO for parham:
// 1) replace the fake table with the actual one
// 2) support other builtins collect(list) contains(list) size()

For::For(const string_view& list_elem, const vector<StringExpressionTypePair>& list, RuleContainer sub_rules) :
    list_element(std::string(list_elem)),
    expression_list(list),
    sub_rules(std::move(sub_rules))
{

    string nm = "Rock, Papar, Scis";
    fake_config_map["name"] = nm;
    fake_config_map["player_range"] = vector<int>{2, 4};
    fake_config_map["rounds"] = vector<int>{1, 20};
    // fake_config_map["rounds"] = pair<int, int>(2, 25);

    fake_table["configuration"] = fake_config_map;
    fake_var_table[list_element] = -1;

    string rock = "Rock"; string paper = "Paper"; string scis = "Scissors"; 
    unordered_map<InnerDriver, InnerDriver> weapons_map {
    {rock, scis},
    {paper, rock},
    {scis,  paper}
    };

    fake_table["weapons"] = weapons_map;
    driver = evaluate_for_exp();
    isDone = isFinished();
}


void For::addEventsTo(GameInstance& game) {
    std::cout << "execute for" << std::endl;
    if (!isFinished()) {
        step();

        for (const auto& rule : sub_rules.getSubRules()) {
            rule->addEventsTo(game);
        }
    }
}

Driver For::evaluate_for_exp() noexcept {
    Driver loop_driver = -1; 

    for(StringExpressionTypePair list_item : expression_list) {
        
        switch (list_item.second) {
            
            // Access is required for identifiers
            case ExpressionType::identifier : {
                loop_driver = accessDriverTable(loop_driver, string(list_item.first));
                break;
            }

            case ExpressionType::builtin : {
                evaluate_for_arg(string(list_item.first));
                break;
            }
            default : {}
        }
    }
    return loop_driver;
}

Driver For::accessDriverTable(Driver& driver, const std::string& key) {
    
    return visit([this, &key](auto&& arg) -> Driver {
        using T = std::decay_t<decltype(arg)>;
        
        if constexpr (std::is_same_v<T, int>) {
            if (arg == -1) 
                return Driver {this->fake_table[key]};

            // Handle other integer cases or throw an error
            throw runtime_error("[FOR_ERROR] - Loop Driver is an integer, but not -1");
        }
        else if constexpr (is_same_v<T, unordered_map<InnerDriver, InnerDriver>>) 
            // Accessing deeper levels of the map
            return Driver {arg.at(key)};
        
        else 
            throw runtime_error("[FOR_ERROR] - Loop Driver type not supported");
        
    }, driver);
}

void For::evaluate_for_arg(const string& built_str) {
    
    if(built_str == "upfrom") 
        for(int i = expression_list.size() - 1; i >= 0; i--)  
            if(expression_list[i].second == ExpressionType::number) 
                fake_var_table[list_element] = stoi(string(expression_list.at(i).first)) - 1; 
}

bool For::isFinished() {
    if(!isDone)   {
        isDone = visit([this](auto& arg) -> bool {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, int>) {
                // Error if the driver is of primitive int type
                throw runtime_error("[FOR_ERROR] - Loop Driver is an integer, Wrong usage of for.isFinished() function");
            }
            else if constexpr (is_same_v<T, unordered_map<InnerDriver, InnerDriver>>) 
                // Check to see if the driver map is empty
                return arg.empty();

            // TODO: case where driver is vector<int>. not able to extract vector<int> from the table
            else {
                int elem = std::get<int>(fake_var_table[list_element]);
                return (elem == 20 ? true : false);
            }
        }, driver);

        // if for_loop is finished, then we clear the resources
        if(isDone) cleanUp();
    }

    return isDone;
}

void For::step() {

    visit([this](auto& arg) {
        using DriverType = decay_t<decltype(arg)>;
        // Map type Loop progress
        if constexpr (is_same_v<DriverType, unordered_map<InnerDriver, InnerDriver>>) {
            auto it = arg.begin(); // getting an element of the map (first one as we don't have key)
            
            // assigning the *key* of the element as the *value* of table[list_element]
            fake_var_table[list_element] = it->first;

            // removing the element from the for rule's *Driver* (means the loop processed the element and is done with it)
            arg.erase(it);
        }
        // vector type loop progress
        else  {
            int elem = std::get<int>(fake_var_table[list_element]);

            if (elem == -1)     // start from the beginning (therefore setting the value of list_element to 1)
                fake_var_table[list_element] = 1;
            
            else    // increment the list_element by 1
                fake_var_table[list_element] = elem + 1;
        }
    }, driver);
}


void For::printForObject() noexcept {
    cout << "\n------ Printing For Object ---------" << endl;
    cout << "For Driver: " << endl;
    printDriver(driver);
    cout << "----\n" << "For fresh Varible string: " << list_element << " && its table value is: ";
    printDriver(fake_var_table[list_element]);
    cout << "-------------   --------------\n" << endl;    
}

void For::printInnerDriver(const InnerDriver& inner) noexcept {
    
    visit([](const auto& arg) {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, int>) 
            cout << "int: " << arg << endl;
        else if constexpr (is_same_v<T, bool>) 
            cout << "bool: " << arg << endl;
        else if constexpr (is_same_v<T, string>) 
            cout << "string: " << arg << endl;
        else if constexpr (is_same_v<T, pair<int, int>>) 
            cout << "pair: " << arg.first << " " << arg.second << endl;
        else if constexpr (is_same_v<T, vector<int>>) {
            cout << "vector<int>:";
            for (int i : arg) cout << " " << i;
            cout << endl;
        }
    }, inner);
}

void For::printDriver(const Driver& driver) noexcept {

    visit([](const auto& arg) {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, unordered_map<InnerDriver, InnerDriver>>) {
            cout << "[TYPE] unordered_map<InnerDriver, InnerDriver>:" << endl;
            for (const auto& [key, value] : arg) {
                cout << "Key -> ";
                printInnerDriver(key);
                cout << "Value -> ";
                printInnerDriver(value);
            }
        } 
        // For all other types, we can reuse the printInnerDriver function
        else printInnerDriver(arg);
        
    }, driver);
}

void For::cleanUp() {

    auto it = fake_var_table.find(list_element);
    if (it != fake_var_table.end()) {
        fake_var_table.erase(it);
    }
}

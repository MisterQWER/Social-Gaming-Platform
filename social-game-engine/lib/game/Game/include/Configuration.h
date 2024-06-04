#include <tuple>
#include "Common.h"
#include <variant>
#include "configuration_kinds.hpp"
#include <map>


using ConfigKinds = std::variant<BooleanKind, IntegerKind, StringKind, EnumKind>;

template <typename T>
class RuleSetup {
    public:
        RuleSetup(ts::Node * setup_node, const std::string& source_code);

        std::string getPrompt() const { return prompt; };
        std::pair<int, int> getRange() const { return range; };

    private:
        T kind;
        std::string prompt;
        std::pair<int, int> range;
};


class Configuration {
    public:
        Configuration(ts::Node * config_node, const std::string& source_code);

        std::string getName() const { return name; };
        std::pair<int, int> getPlayerRange() const { return player_range; };
        bool hasAudience() const { return has_audience; };

        //Added getter to retrieve setup info
        std::map<std::string, ConfigKinds> getSetup() const { return setup; };

        // For debugging information
        void printDebugInfo();

    private:
        ts::Node * config_node;
        std::string name;
        std::pair<int, int> player_range;
        bool has_audience;
        std::map<std::string, ConfigKinds> setup; 
};


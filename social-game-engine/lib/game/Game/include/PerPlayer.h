#include <tuple>
#include <map>
#include "VariableResolution.h"


class Player : public VariableResolution {
public:
    Player(ts::Node *player_node, const std::string &source_code);
    std::map<std::string_view, Value> getPlayerMap();

private:
    std::map<std::string_view, Value> playerMap;
};

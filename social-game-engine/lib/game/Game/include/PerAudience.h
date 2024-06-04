#include <tuple>
#include <map>
#include "VariableResolution.h"


class Audience : public VariableResolution {
public:
    Audience(ts::Node *player_node, const std::string &source_code);
    std::map<std::string_view, Value> getAudienceMap();

private:
    std::map<std::string_view, Value> audienceMap;
};

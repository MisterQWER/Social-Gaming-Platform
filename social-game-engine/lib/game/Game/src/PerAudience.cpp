
#include "PerAudience.h"

Audience::Audience(ts::Node *audience_node, const std::string &source_code)
        : VariableResolution(audience_node, source_code) {
    audienceMap = createValueMap();
}

std::map<std::string_view, Value> Audience::getAudienceMap() {
    return audienceMap;
}

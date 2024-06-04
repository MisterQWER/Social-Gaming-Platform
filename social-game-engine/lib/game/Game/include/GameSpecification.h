#include <map>
#include "VariableResolution.h"
#include "optional"
#include "Constants.h"
#include "Variables.h"
#include "BaseRule.h"

enum class Result {
    SUCCESS , FAIL
};

class GameSpecification {

private:
    map <string_view, Value> entries;

public:
    GameSpecification(ts::Node* root, const string& sourcecode);
    GameSpecification(const GameSpecification& other);
    ~GameSpecification() = default;
    GameSpecification& operator=(const GameSpecification& other);

    map <string_view, Value> getTable() const noexcept { return entries; }
    Result insert_entry_by_keychain(const Value& val, vector<StringExpressionTypePair>& keychain);
    Result modify_entry_by_keychain(const Value& newVal, vector<StringExpressionTypePair>& keychain);
    std::optional<Value> get_entry_by_keychain(const vector<StringExpressionTypePair>& keychain) const noexcept;
    void print() const;
};
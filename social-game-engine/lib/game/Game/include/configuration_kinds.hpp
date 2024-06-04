#ifndef CONFIGURATION_KINDS_H
#define CONFIGURATION_KINDS_H

struct BooleanKind {
    std::string prompt;
};

//struct used for range
struct IntegerKind {
    int minVal;
    int maxVal;
    std::string prompt;
};


struct StringKind {
    std::string str;
    std::string prompt;
}; 

struct EnumKind{
    std::unordered_map<std::string, std::string> choices;
    std::string prompt;
};


struct MultipleChoice{
    std::vector<std::string> listExpression;
};

struct QuestionAnswer{
    std::string question;
    std::string answer;
};

struct Json{
    std::string jsonString;
};


#endif  // GAME_CONFIGURATION_H
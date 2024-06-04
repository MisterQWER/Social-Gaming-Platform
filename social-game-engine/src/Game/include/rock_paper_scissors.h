#ifndef ROCK_PAPER_SCISSORS_H
#define ROCK_PAPER_SCISSORS_H
#include <string>
#include <iostream>
#include <string_view>

/*
    Note:
        - This is solely for backend testing. Delete it later
*/

enum Weapon {
    kRock,
    kPaper,
    kScissors,
    kInvalid
};

class RockPaperScissors {
    public:
        RockPaperScissors() = default;
        void ShowThechoices();
        std::string DetermineWinner(const std::string_view& userOneChoice,
        const std::string_view& userTwoChoice);
    private:
        Weapon ConvertToWeapon(const std::string_view& weapon);
};



#endif  // ROCK_PAPER_SCISSORS_H
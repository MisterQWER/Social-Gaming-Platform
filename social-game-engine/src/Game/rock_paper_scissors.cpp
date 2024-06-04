#include "rock_paper_scissors.h"
#include <algorithm>

/*
    Note:
        - This is solely for backend testing. Delete it later
*/

void RockPaperScissors::ShowThechoices() {
    std::cout << "Enter your choice\nrock\npaper\nscissors\n(Please type it): " << std::endl;
}

std::string RockPaperScissors::DetermineWinner(const std::string_view& userOneChoice,
    const std::string_view& userTwoChoice) {
        Weapon player1 = ConvertToWeapon(userOneChoice);
        Weapon player2 = ConvertToWeapon(userTwoChoice);

        if(player1 == kInvalid || player2 == kInvalid) {
            return "Invalid";
        }
        else if(player1 == player2) {
            return "Draw";
        }
        else if(player1 == kRock) {
            return (player2 == kScissors) ? "Player 1 Wins" : "Player 2 Wins";
        }

        else if(player1 == kPaper) {
            return (player2 == kRock) ? "Player 1 Wins" : "Player 2 Wins";
        }

        else if(player1 == kScissors) {
            return (player2 == kPaper) ? "Player 1 Wins" : "Player 2 Wins";
        }
        return "Invalid";
    }

Weapon RockPaperScissors::ConvertToWeapon(const std::string_view& weapon) {
    std::string weaponToLower(weapon);
    std::transform(weaponToLower.begin(), weaponToLower.end(), weaponToLower.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        });

    if (weaponToLower == "rock") {
        return kRock;
    }
    else if(weaponToLower == "paper") {
        return kPaper;
    }
    else if(weaponToLower == "scissors") {
        return kScissors;
    }
    return kInvalid;
}

    
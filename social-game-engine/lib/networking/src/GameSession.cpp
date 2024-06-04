#include "GameSession.h"

RoomCode GameSession::generateRoomCode(const Player& host) {
    std::hash<std::string> hash_code;
    auto current_time = std::chrono::system_clock::now().time_since_epoch().count();

    std::string string_code = host.getPlayerName() + std::to_string(host.getConnection().id)
                                + std::to_string(current_time);
    std::string room_code = std::to_string(hash_code(string_code));
    return RoomCode{room_code};
}

GameSession::GameSession(const Player& host, const GameInstance& game) :
    host(host), 
    game(game),
    room_code(generateRoomCode(host)) {}

// TODO: Add implementation to start the game in that session
void GameSession::startGame() {

}
void GameSession::endGame() {

}

ConnectionResult GameSession::addPlayerToSession(const Player& new_player) {
    auto player_it = std::find_if(players_list.begin(), players_list.end(), [new_player](const Player& player) {
        return player.getConnection() == new_player.getConnection();
    });

    if (player_it != players_list.end()) {
        return ConnectionResult::FAIL;
    }

    players_list.emplace_back(new_player);
    return ConnectionResult::SUCCESS;
}

ConnectionResult GameSession::removePlayerFromSession(const Connection& client) {
    auto player_it = std::remove_if(players_list.begin(), players_list.end(), [client](const Player& player) {
        return player.getConnection() == client;
    });

    if (player_it == players_list.end()) {
        return ConnectionResult::FAIL;
    }

    players_list.erase(player_it, players_list.end());
    return ConnectionResult::SUCCESS;
}

void GameSession::addMessageToLobby(const Message& message) {
    lobby_messages.emplace_back(message);
    std::cout << message.text << "\n";
}


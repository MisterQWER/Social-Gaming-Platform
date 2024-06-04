#include "GameSessionsManager.h"

RoomCode GameSessionsManager::createGameSession(const Player& host, const GameInstance& game_spec) {
    auto game_session = std::make_unique<GameSession>(host, game_spec);
    RoomCode room_code = game_session->getRoomCode();

    game_sessions.emplace(room_code, std::move(game_session));
    return room_code;
}

ConnectionResult GameSessionsManager::joinGameSession(const Player& player, const RoomCode& room_code) {
    auto it = game_sessions.find(room_code);
    if (it != game_sessions.end()) {
        client_to_session.insert(std::make_pair(player, room_code));
        return it->second->addPlayerToSession(player);
    }
    return ConnectionResult::FAIL;
}

ConnectionResult GameSessionsManager::exitGameSession(const Player& player, const RoomCode& room_code) {
    auto it = game_sessions.find(room_code);
    if (it != game_sessions.end()) {
        client_to_session.erase(player);
        return it->second->removePlayerFromSession(player.getConnection());
    }
    return ConnectionResult::FAIL;
}

void GameSessionsManager::sortMessagesToSessions(const Message& message) {
    auto c = message.connection;
    for (const auto& game_session : game_sessions) {
        auto list_of_player = game_session.second->getPlayerList();
        for (const auto& player : list_of_player) {
            if (player.getConnection() == c) {
                game_session.second->addMessageToLobby(message);
            }
        }
    }
}

std::vector<Player> GameSessionsManager::getPlayerListForSession(const Connection& c) {
    std::vector<Player> player_list;
    
    for (const auto& cts : client_to_session) {
        if (cts.first.getConnection() == c){
            RoomCode room_code = cts.second;
            player_list = game_sessions[room_code]->getPlayerList();
        }
    }

    //auto room_it = client_to_session.find(c);
    // if (room_it != client_to_session.end()) {
    //     std::string room_code = room_it->second;
        
    //     player_list = game_sessions[room_code]->getPlayerList();
    // }

    return player_list;
}

std::optional<Player> GameSessionsManager::getHostForSession(const Connection& c) {
    for (const auto& cts : client_to_session) {
        if (cts.first.getConnection() == c){
            RoomCode room_code = cts.second;
            return game_sessions[room_code]->getHost();
        }
    }

    return std::nullopt;
}
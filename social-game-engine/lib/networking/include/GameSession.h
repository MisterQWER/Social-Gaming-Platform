#ifndef NETWORKING_GAME_SESSIONS_H
#define NETWORKING_GAME_SESSIONS_H

#include <vector>
#include <iostream>
#include <chrono>

#include "Player.h"
#include "Server.h"
#include "GameInstance.hpp"

using networking::Server;
using networking::Connection;
using networking::Message;

enum class ConnectionResult {
    SUCCESS, FAIL
};

struct RoomCode {
    std::string room_code;

    bool operator==(RoomCode other) const {
        return room_code == other.room_code;
    }
};

namespace std {
    template <>
    struct hash<RoomCode> {
        size_t operator()(const RoomCode& obj) const {
            return std::hash<std::string>{}(obj.room_code); 
        }
    };
}

class GameSession {
    public:
        GameSession(const Player& host, const GameInstance& game);

        void startGame();
        void endGame();

        ConnectionResult addPlayerToSession(const Player& new_player);
        ConnectionResult removePlayerFromSession(const Connection& client);
        void addMessageToLobby(const Message& message);

        RoomCode getRoomCode() const { return room_code; }
        std::vector<Player> getPlayerList() const { return players_list; }
        Player getHost() const { return host; }

    private:
        Player host;
        std::vector<Player> players_list;
        RoomCode room_code;
        GameInstance game;
        std::vector<Message> lobby_messages;

        RoomCode generateRoomCode(const Player& host);
};

#endif
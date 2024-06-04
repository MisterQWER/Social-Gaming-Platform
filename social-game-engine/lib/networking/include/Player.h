#ifndef NETWORKING_PLAYER_H
#define NETWORKING_PLAYER_H

#include <string>
#include <iostream>

#include "Server.h"

using networking::Connection;

enum class PlayerState {
    CHOOSE_NICKNAME,
    CREATE_OR_JOIN_LOBBY,
    BUILD_LOBBY,
    ENTER_ROOM_CODE,
    PICK_GAME,
    IN_GAME
};

class Player {
    public:
        Player() = default;
        Player(const Connection& client) : client(client) {}
        Player(const std::string& name, const Connection& client) :
            player_name(name), client(client) {}

        std::string getPlayerName() const { return player_name; }
        Connection getConnection() const { return client; }
        PlayerState getState() const { return state; }

        void setState(PlayerState new_state) { state = new_state; }
        void setName(std::string name) { player_name = name; }

        bool operator==(const Player& other) const {
            return other.getConnection().id == client.id;
        }

    private:
        std::string player_name;
        Connection client;
        PlayerState state;
};

namespace std {
    template <>
    struct hash<Player> {
        size_t operator()(const Player& obj) const {
            return std::hash<uintptr_t>()(obj.getConnection().id);
        }
    };
}

#endif
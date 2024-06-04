#ifndef NETWORKING_GAME_SESSIONS_MANAGER_H
#define NETWORKING_GAME_SESSIONS_MANAGER_H

#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include "GameSession.h"
#include "Player.h"
#include "Server.h"

using networking::Server;
using networking::Connection;
using networking::Message;

/**
 * @class GameSessionsManager
 * 
 * The class handles all the game sessions (or game rooms) that exist on one Server.
 * Only the creating player(host) is allowed to create a new game session.
 * For all exisiting game sessions, the manager class also handles players joining/leaving the session.
*/

class GameSessionsManager {
    public:
        GameSessionsManager() = default;

        RoomCode createGameSession(const Player& host, const GameInstance& game_spec);
        ConnectionResult joinGameSession(const Player& player, const RoomCode& room_code);
        ConnectionResult exitGameSession(const Player& player, const RoomCode& room_code);

        void sortMessagesToSessions(const Message& message);
        std::vector<Player> getPlayerListForSession(const Connection& c);
        std::optional<Player> getHostForSession(const Connection& c);

        void startGameSession(std::string room_code);
        void endGameSession(std::string room_code);

    private:
        std::deque<Message> server_messages;
        std::unordered_map<Player, RoomCode> client_to_session;  // string is the room code
        std::unordered_map<RoomCode, std::unique_ptr<GameSession>> game_sessions;
};

#endif
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sstream>

#include "Server.h"
#include "rock_paper_scissors.h"
#include "GameSessionsManager.h"


using networking::Server;
using networking::Connection;
using networking::Message;

struct MessageResult {
  std::string result;
  bool shouldShutdown;
};

class GameServer {
    public:
        GameServer() = default;

        void Start(uint16_t port, std::string HTMLFilePath);

    private:
        std::unique_ptr<Server> server_;
        std::vector<Connection> clients_;
        std::vector<Player> players;
        GameSessionsManager session_manager_;
        std::vector<std::string_view> games_list = {"games/rock-paper-scissors.game", "games/new_game.game"};

        void OnConnect(const Connection& c);
        void OnDisconnect(const Connection& c);

        Message processMessages(const std::deque<Message>& incoming);
        std::deque<Message> buildOutgoing(const Message& log);

        RoomCode createGameSession(const Player& host, const GameInstance& game_spec);
        ConnectionResult joinGameSession(const Player& player, const RoomCode& room_code);
        ConnectionResult exitGameSession(const Player& player, const RoomCode& room_code);

        // Game Lobby Question Setup
        void assignUserName(const Connection &c, std::string nickname);
        void displayGamePrompt(const Connection &c, std::string playerName);
        std::optional<GameInstance> getRequestedGame(const Connection &c, std::string request_game);
        void roomCodePrompt(const Connection &c);
        bool getRoomCode(const Connection &c, std::string roomCode);
        void displayRoomCodeResponse(const Connection &c, bool response);
        void buildLobby(const Connection &c);
        ConnectionResult exitGameLobby(const Connection &c, std::string code);
};

#endif  // GAME_SERVER_H
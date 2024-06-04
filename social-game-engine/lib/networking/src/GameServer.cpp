#include "GameServer.h"

RoomCode GameServer::createGameSession(const Player& host, const GameInstance& game_spec) {
    return session_manager_.createGameSession(host, game_spec);
}

ConnectionResult GameServer::joinGameSession(const Player& player, const RoomCode& room_code){
    return session_manager_.joinGameSession(player, room_code);
}

ConnectionResult GameServer::exitGameSession(const Player& player, const RoomCode& room_code){
    return session_manager_.exitGameSession(player, room_code);
}

void GameServer::OnConnect(const Connection& c) {
    std::cout << "New connection found: " << c.id << std::endl;
    clients_.push_back(c);
    Player player = Player(c);
    player.setState(PlayerState::CHOOSE_NICKNAME);
    players.push_back(player);
    
    std::deque<Message> outgoing;
    std::string askName = "SERVER> Welcome to game server! What is your username:\n";
    outgoing.push_back({c, askName});
    server_->send(outgoing);
}

void GameServer::OnDisconnect(const Connection& c) {
    std::cout << "Connection lost: " << c.id << std::endl;
    auto eraseBegin = std::remove(std::begin(clients_), std::end(clients_), c);
    clients_.erase(eraseBegin, std::end(clients_));

    auto erasePlayer = std::remove_if(players.begin(), players.end(), [c](const Player& player) {        
        return c == player.getConnection();
    });
    players.erase(erasePlayer, players.end());
}

void GameServer::displayGamePrompt(const Connection &c, std::string playerName){
    std::deque<Message> outgoing;
    std::string display_name = "> " + playerName + "\n";
    outgoing.push_back({c, display_name});
    std::string pick_game_question = "SERVER> Hi " + playerName + "! Please pick a game(choose a number):\n\t1. Rock Paper Scissors\n";
    outgoing.push_back({c, pick_game_question});
    server_->send(outgoing);
}

void GameServer::assignUserName(const Connection &c, std::string nickname) {
    
    for(auto& player : players) {
        if(player.getConnection() == c) {
            player.setName(nickname);
            displayGamePrompt(c, player.getPlayerName());
        }
    }
}

std::optional<GameInstance> GameServer::getRequestedGame(const Connection &c, std::string request_game) {
    std::deque<Message> outgoing;
    std::string display_choice = "> " + request_game + "\n";
    outgoing.push_back({c, display_choice});

    if (request_game == "1") {
        // ask clinet to join or create game session
        std::string lobby_question = "SERVER> Do you want to create(c) or join(j) a game lobby?\n";
        outgoing.push_back({c, lobby_question});
        server_->send(outgoing);
        return GameInstance{games_list.at(0)};

    } else {
        // handle if client enters invalid choice
        std::string invalid_question = "SERVER> Invalid game choice. Please re-enter.\n";
        outgoing.push_back({c, invalid_question});
        server_->send(outgoing);
        return std::nullopt;
    }
}

void GameServer::buildLobby(const Connection &c){
    for (auto& player : players) {
        if (player.getConnection() == c){
            std::deque<Message> outgoing;
            GameInstance game{"games/rock-paper-scissors.game"};
            RoomCode room = createGameSession(player, game);
            std::string display_code = "SERVER> Invitation code: " + room.room_code + "\n";
            outgoing.push_back({c, display_code});
            server_->send(outgoing);
            return;
        }
    }
}

void GameServer::roomCodePrompt(const Connection &c){
    //request already j
    for (auto &player : players){
        if (player.getConnection() == c){
            std::deque<Message> outgoing;
            std::string room_code_prompt = "SERVER> Enter your room code:\n";
            outgoing.push_back({c, room_code_prompt});
            server_->send(outgoing);
            return;
        }
    }
}

void GameServer::displayRoomCodeResponse(const Connection &c, bool response){
    std::deque<Message> client_outgoing;
    std::deque<Message> host_outgoing;
    
    if (response){
        std::vector<Player> player_list = session_manager_.getPlayerListForSession(c);
        std::cout << "PLAYER_LIST_SIZE: " <<  player_list.size() << std::endl;
        
        if (session_manager_.getHostForSession(c).has_value()) {
            Player host = session_manager_.getHostForSession(c).value();
            std::string response = "SERVER> Joined lobby with host: " + host.getPlayerName() + "\n";
            client_outgoing.push_back({c, response});
            server_->send(client_outgoing);

            std::string response_to_host = "SERVER> Would you like to start the game with these players. y or n?\n";
            std::string list_of_players;
            for (size_t i = 0; i < player_list.size(); i++){
                list_of_players += std::to_string(i + 1) + ". " + player_list[i].getPlayerName() + "\n";
            }
            response_to_host += list_of_players;
            host_outgoing.push_back({host.getConnection(), response_to_host});
            server_->send(host_outgoing);
        }

    } else {
        std::string response = "SERVER> Unable to find lobby, please try again..\n";
        client_outgoing.push_back({c, response});
        server_->send(client_outgoing);
    }
}

bool GameServer::getRoomCode(const Connection &c, std::string roomCode){
    std::deque<Message> outgoing;
    std::string display_response = "> " + roomCode + "\n";
    outgoing.push_back({c, display_response});
    server_->send(outgoing);

    for (auto &player : players){
        if (player.getConnection() == c){
            // verifying that the player is in the correct game lobby still needs to be done
            if (joinGameSession(player, RoomCode{roomCode}) == ConnectionResult::SUCCESS) {
                return true;
            } else {
                return false;
            }
                
        }
    }
}

ConnectionResult GameServer::exitGameLobby(const Connection &c, std::string code){
    for (auto &player : players){
        if (player.getConnection() == c){
            return exitGameSession(player.getConnection(), RoomCode{code});
        }
    }
}

Message GameServer::processMessages(const std::deque<Message>& incoming) {
    std::string result;
    bool quit = false;
    Connection c;
    for (const auto& message : incoming) {
        auto player_it = std::find_if(players.begin(), players.end(), [message](const Player& player) {
            return message.connection == player.getConnection();
        });

        PlayerState player_state;
        if (player_it != players.end()) {
            player_state = player_it->getState();
        }

        if (player_state == PlayerState::CHOOSE_NICKNAME) {
            assignUserName(player_it->getConnection(), message.text);
            player_it->setState(PlayerState::PICK_GAME);
            
        } 
        else if (player_state == PlayerState::PICK_GAME) {
            std::optional<GameInstance> requested_game = getRequestedGame(player_it->getConnection(), message.text);
            if (requested_game.has_value()) {
                player_it->setState(PlayerState::CREATE_OR_JOIN_LOBBY);
            }
        } 
        else if (player_state == PlayerState::CREATE_OR_JOIN_LOBBY) {
            if (message.text == "c"){
                buildLobby(player_it->getConnection());
            }
            else if (message.text == "j"){
                roomCodePrompt(player_it->getConnection());
                player_it->setState(PlayerState::ENTER_ROOM_CODE);
            }
        }
        else if (player_state == PlayerState::BUILD_LOBBY){
            buildLobby(player_it->getConnection());
        } 
        else if(player_state == PlayerState::ENTER_ROOM_CODE) {
            if (getRoomCode(player_it->getConnection(), message.text)){
                displayRoomCodeResponse(player_it->getConnection(), true);
                player_it->setState(PlayerState::IN_GAME);
            }
            else{
                displayRoomCodeResponse(player_it->getConnection(), false);
            }
           
        }
       
        else if (player_state == PlayerState::IN_GAME) {
            if (message.text == "e"){
                exitGameLobby(player_it->getConnection(), message.text);
            }
            c = player_it->getConnection();
            result = player_it->getPlayerName() + "> " + message.text + "\n";
            
            session_manager_.sortMessagesToSessions(message);
            
        } 
        else if (message.text == "quit") {
            server_->disconnect(message.connection);
            quit = true;
            player_it = players.erase(player_it);
        } 
        else if (message.text == "shutdown") {
            std::cout << "Shutting down.\n";
            quit = true;
        }
    }
    return Message{c, result};
}

std::deque<Message> GameServer::buildOutgoing(const Message& log) {
    std::deque<Message> outgoing;
    auto c = log.connection;
    
    std::vector<Player> player_list = session_manager_.getPlayerListForSession(c);
    if (session_manager_.getHostForSession(c).has_value()) {
        Player host = session_manager_.getHostForSession(c).value();
        outgoing.push_back({host.getConnection(), log.text});
    }
    
    for (const auto &player : player_list) {
        outgoing.push_back({player.getConnection(), log.text});
    }
    return outgoing;
}

void GameServer::Start(uint16_t port, std::string HTMLFilePath) {
    server_ = std::make_unique<Server>(port, HTMLFilePath, [this](networking::Connection& c) {
        OnConnect(c);}, [this](networking::Connection& c) { OnDisconnect(c);});

    while(true) {
        bool errorWhileUpdating = false;
        try {
            server_->update();
        } 
        catch (std::exception& e) {
            std::cerr << "Exception from Server update:\n"
                        << " " << e.what() << "\n\n";
            errorWhileUpdating = true;
        }
        
        const auto incoming = server_->receive();
        const auto log = processMessages(incoming);
        const auto outgoing = buildOutgoing(log);
        server_->send(outgoing);

        // if (shouldQuit || errorWhileUpdating) {
        //     break;
        // }

        sleep(1);
    }
}


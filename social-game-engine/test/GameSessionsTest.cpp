#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Player.h"
#include "Server.h"
#include "GameSession.h"

TEST(GameSessionsTest, roomUniquenessTest) {
    Connection c1;
    Player host(c1);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(host, game);
    RoomCode room_code = game_session->getRoomCode();

    Connection c2;
    Player host2(c2);
    auto game_session2 = std::make_unique<GameSession>(host2, game);
    RoomCode room_code2 = game_session2->getRoomCode();

    EXPECT_NE(room_code.room_code, room_code2.room_code);
}


TEST(GameSessionsTest, addPlayerTest){
    Connection con;
    Player player(con);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player, game);
    auto addPlayer = game_session->addPlayerToSession(player);
    EXPECT_EQ(ConnectionResult::SUCCESS, addPlayer);
}


TEST(GameSessionsTest, addPlayerListUpdateTest)
{
    Connection c1;
    Player player1(c1);
    Connection c2;
    Player player2(c2);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player1, game);
    game_session->addPlayerToSession(player1);

    auto initialSize = game_session->getPlayerList().size();
    game_session->addPlayerToSession(player2);

    auto newSize = game_session->getPlayerList().size();

    EXPECT_EQ(initialSize + 1, newSize);
    
}


TEST(GameSessionsTest, addPlayerTwiceTest)
{
    Connection c1;
    Player player(c1);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player, game);
    
    game_session->addPlayerToSession(player);
    
    ConnectionResult addPlayerAgainResult = game_session->addPlayerToSession(player);
    //adding player twice fails
    EXPECT_EQ(ConnectionResult::FAIL, addPlayerAgainResult);
}

TEST(GameSessionsTest, removePlayerTest) {
    Connection c1, c2, c3;
    c1.id = 1;
    c2.id = 2;
    c3.id = 3;
    Player player1(c1);
    Player player2(c2);
    Player player3(c3);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player1, game);
    game_session->addPlayerToSession(player2);
    game_session->addPlayerToSession(player3);
    auto result = game_session->removePlayerFromSession(c2);
    EXPECT_EQ(ConnectionResult::SUCCESS, result);
}

TEST(GameSessionsTest, removePlayerUpdatesList){
    Connection c1, c2;
    Player player1(c1), player2(c2);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player1, game);
    
    game_session->addPlayerToSession(player1);
    game_session->addPlayerToSession(player2);
    
    game_session->removePlayerFromSession(c2);
    
    // check player list
    auto playerList = game_session->getPlayerList();
    EXPECT_EQ(1, playerList.size());
    EXPECT_NE(playerList.end(), std::find(playerList.begin(), playerList.end(), player1));
    EXPECT_EQ(playerList.end(), std::find(playerList.begin(), playerList.end(), player2));
}


TEST(GameSessionsTest, returnHost) {
    Connection c1, c2, c3;
    c1.id = 1;
    c2.id = 2;
    c3.id = 3;
    // player1 is the host
    Player player1(c1);
    Player player2(c2);
    Player player3(c3);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(player1, game);
    game_session->addPlayerToSession(player2);
    game_session->addPlayerToSession(player3);
    auto result = game_session->getHost();
    EXPECT_EQ(result.getConnection(), c1);
}

TEST(GameSessionsTest, removeHostTest)
{
    Connection c1;
    Player host(c1);
    GameInstance game{"games/rock-paper-scissors.game"};
    auto game_session = std::make_unique<GameSession>(host, game);
    
    ConnectionResult removeResult = game_session->removePlayerFromSession(c1);
    
    EXPECT_EQ(ConnectionResult::FAIL, removeResult);
}
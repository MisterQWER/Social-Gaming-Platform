#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Player.h"
#include "Server.h"



TEST(PlayerTest, chooseNicknameTest){
    Connection c1;
    Player player(c1);

    std::string userName = "Nick";
    player.setName(userName);

    auto result = player.getPlayerName();
    EXPECT_EQ(result, userName);
}


TEST(PlayerTest, initialStateTest){
    Connection c1;
    Player player(c1);
    player.setState(PlayerState::CHOOSE_NICKNAME);
    EXPECT_EQ(player.getState(), PlayerState::CHOOSE_NICKNAME);
}


TEST(PlayerTest, checkStateTest) {
    Connection c1;
    Player host(c1);

    host.setState(PlayerState::CREATE_OR_JOIN_LOBBY);
    EXPECT_EQ(host.getState(), PlayerState::CREATE_OR_JOIN_LOBBY);

    host.setState(PlayerState::BUILD_LOBBY);
    EXPECT_EQ(host.getState(), PlayerState::BUILD_LOBBY);

    host.setState(PlayerState::ENTER_ROOM_CODE);
    EXPECT_EQ(host.getState(), PlayerState::ENTER_ROOM_CODE);
    
    host.setState(PlayerState::PICK_GAME);
    EXPECT_EQ(host.getState(), PlayerState::PICK_GAME);

    host.setState(PlayerState::IN_GAME);
    EXPECT_EQ(host.getState(), PlayerState::IN_GAME);
}

TEST(PlayerTest, equalityOperatorTest) {
    Connection conn1{1}, conn2{2};
    Player player1(conn1), player2(conn1), player3(conn2);
    EXPECT_TRUE(player1 == player2);
    EXPECT_FALSE(player1 == player3);
}



TEST(PlayerTest, hashFunctionTest) {
    Connection conn{1};
    Player player(conn);
    std::hash<Player> player_hash;
    size_t hash1 = player_hash(player);
    // name change shouldn't affect hash
    player.setName("Sumner"); 
    size_t hash2 = player_hash(player);
    EXPECT_EQ(hash1, hash2);
}


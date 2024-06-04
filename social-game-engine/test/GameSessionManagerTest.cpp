#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Server.h"
#include "rock_paper_scissors.h"
#include "GameSessionsManager.h"

TEST(GameSessionsManagerTest, updatePlayerListTest){
    Connection c1, c2;
    Player host(c1), player(c2);
    GameSessionsManager manager;
    GameInstance game{"games/rock-paper-scissors.game"};

    RoomCode room_code = manager.createGameSession(host, game);
    auto initialSize = manager.getPlayerListForSession(c1).size();

    ConnectionResult result = manager.joinGameSession(player, room_code);
    EXPECT_EQ(ConnectionResult::SUCCESS, result);

    auto newSize = manager.getPlayerListForSession(c2).size();

    EXPECT_EQ(initialSize + 1, newSize);
}

TEST(GameSessionsManagerTest, exitAndRejoinTest) {
    Connection c1, c2;
    Player host(c1), player(c2);
    GameSessionsManager manager;
    GameInstance game{"games/rock-paper-scissors.game"};

    RoomCode room_code = manager.createGameSession(host, game);
    manager.joinGameSession(player, room_code);

    //Exit and rejoin
    manager.exitGameSession(player, room_code);
    ConnectionResult result = manager.joinGameSession(player, room_code);

    EXPECT_EQ(ConnectionResult::SUCCESS, result);
}


TEST(GameSessionsManagerTest, correctHostForSessionTest) {
    Connection c1, c2, c3;
    Player host(c1), player1(c2), player2(c3);
    GameSessionsManager manager;
    GameInstance game{"games/rock-paper-scissors.game"};

    RoomCode room_code = manager.createGameSession(host, game);

    manager.joinGameSession(player1, room_code);
    manager.joinGameSession(player2, room_code);

    if (manager.getHostForSession(c1).has_value()){
        Player retrievedHost = manager.getHostForSession(c1).value();  
        EXPECT_EQ(host, retrievedHost);
    }
    
}

TEST(GameSessionsManagerTest, joinInvalidRoomCodeTest) {
    Connection c1;
    Player player(c1);
    GameSessionsManager manager;
    RoomCode invalidRoomCode;

    ConnectionResult result = manager.joinGameSession(player, invalidRoomCode);
    EXPECT_EQ(ConnectionResult::FAIL, result);
}


TEST(GameSessionsManagerTest, hostRemovedAfterExitTest) {
    Connection c1, c2;
    Player host(c1), player(c2);
    GameSessionsManager manager;
    GameInstance game{"games/rock-paper-scissors.game"};

    RoomCode room_code = manager.createGameSession(host, game);
    manager.joinGameSession(player, room_code);
    manager.exitGameSession(host, room_code); 

    if (manager.getHostForSession(c2).has_value()){
        Player retrievedHost = manager.getHostForSession(c2).value();
        EXPECT_EQ(host, retrievedHost);
    }
}

TEST(GameSessionsManagerTest, messageSortingTest) {
    Connection c1, c2;
    Player host(c1), player(c2);
    GameSessionsManager manager;
    GameInstance game{"games/rock-paper-scissors.game"};

    RoomCode room_code = manager.createGameSession(host, game);
    manager.joinGameSession(player, room_code);

    // Send message to sort function
    Message testMessage{c1, "Message Holder"};
    manager.sortMessagesToSessions(testMessage);

}

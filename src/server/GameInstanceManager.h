// The GameInstanceManager only exists on the server side. It stores all currently active games and offers
// functionality to retrieve game instances by id and adding players to games.
// If a new Player requests to join a game but no valid GameInstance is available, then this class
// will generate a new GameInstance and add it to the unordered_map of (active) game instances.

#ifndef CHESS_GAMEINSTANCEMANAGER_H
#define CHESS_GAMEINSTANCEMANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "PlayerManager.h"
#include "GameInstance.h"

class GameInstanceManager {

private:

    inline static std::shared_mutex games_lut_lock;
    static std::unordered_map<std::string, GameInstance*> games_lut;

    static GameInstance* create_new_game();
    static GameInstance* find_joinable_game_instance();
    static bool try_remove_player(Player* player, GameInstance*& game_instance_ptr, std::string& err);

public:

    // returns true if the desired GameInstance 'game_id' was found or false otherwise.
    // The found game instance is written into game_instance_ptr.
    static bool try_get_game_instance(const std::string& game_id, GameInstance*& game_instance_ptr);

    // returns true if the desired Player 'player_id' was found and is connected to a GameInstance.
    // The found Player and GameInstance will be written into 'Player' and 'game_instance_ptr'
    static bool try_get_player_and_game_instance(const std::string& player_id, Player*& player, GameInstance*& game_instance_ptr, std::string& err);

    // Try to add 'Player' to any game. Returns true if 'Player' is successfully added to a GameInstance.
    // The joined GameInstance will be written into 'game_instance_ptr'.
    static bool try_add_player_to_any_game(Player* player, GameInstance*& game_instance_ptr, std::string& err);

    // Try to add 'Player' to the provided 'game_instance_ptr'. Returns true if success and false otherwise.
    static bool try_add_player(Player* player, GameInstance*& game_instance_ptr, std::string& err);

    // Try to remove 'Player' from whatever game they are in.
    static bool try_remove_player(std::string& game_id, std::string& err);

};


#endif //CHESS_GAMEINSTANCEMANAGER_H

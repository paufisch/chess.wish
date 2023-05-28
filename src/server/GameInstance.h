// The GameInstance class is a wrapper around the GameState of an active instance of the game.
// This class contains functions to modify the contained GameState.

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <vector>
#include <string>
#include <mutex>

#include "ServerNetworkManager.h"
#include "../common/game_state/player/Player.h"
#include "../common/game_state/GameState.h"
#include "../common/network/responses/FullStateResponse.h"

class GameInstance {

private:
    GameState* _game_state;
    bool is_player_allowed_to_play(Player* player);
    inline static std::mutex modification_lock;

public:
    GameInstance();
    ~GameInstance() {
        if (_game_state != nullptr) {
            delete _game_state;
        }
        _game_state = nullptr;
    }

    // accessors
    std::string get_id();
    GameState* get_game_state();
    bool is_full();
    bool is_started();
    bool is_finished();

    // game update functions
    bool start_game(Player* player, std::string& err);
    bool try_add_player(Player* new_player, std::string& err);
    bool try_remove_player(Player* player, std::string& err);
    std::vector<std::vector<bool>> legal_moves(Player* player, int coordinate_1, int coordinate_2, std::string& err);
    bool move_piece(Player* player, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2, std::string& err);
    bool resign(Player* player, std::string& err);

};


#endif //CHESS_GAME_H


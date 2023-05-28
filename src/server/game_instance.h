// The game_instance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <vector>
#include <string>
#include <mutex>

#include "server_network_manager.h"
#include "../common/game_state/player/player.h"
#include "../common/game_state/game_state.h"
#include "../common/network/responses/full_state_response.h"

class game_instance {

private:
    game_state* _game_state;
    bool is_player_allowed_to_play(player* player);
    inline static std::mutex modification_lock;

public:
    game_instance();
    ~game_instance() {
        if (_game_state != nullptr) {
            delete _game_state;
        }
        _game_state = nullptr;
    }

    // accessors
    std::string get_id();
    game_state* get_game_state();
    bool is_full();
    bool is_started();
    bool is_finished();

    // game update functions
    bool start_game(player* player, std::string& err);
    bool try_add_player(player* new_player, std::string& err);
    bool try_remove_player(player* player, std::string& err);
    std::vector<std::vector<bool>> legal_moves(player* player, int coordinate_1, int coordinate_2, std::string& err);
    bool move_piece(player* player, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2, std::string& err);
    bool resign(player* player, std::string& err);

};


#endif //CHESS_GAME_H


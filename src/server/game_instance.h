//
// Created by Manuel on 25.01.2021.
//
// The game_instance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#ifndef LAMA_GAME_H
#define LAMA_GAME_H

#include <vector>
#include <string>
#include <mutex>

#include "../common/game_state/player/player.h"
#include "../common/game_state/game_state.h"

class game_instance {

private:
    game_state* _game_state;
    bool is_player_allowed_to_play(player* player);
    inline static std::mutex modification_lock;

    //checks if there are still 2 kings on the board
    //bool is_king_dead();

public:
    game_instance();
    ~game_instance() {
        if (_game_state != nullptr) {
            delete _game_state;
        }
        _game_state = nullptr;
    }
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
    //bool play_card(player* player, const std::string& card_id, std::string& err);
    //bool draw_card(player* player, card*& drawn_card, std::string& err);
    bool resign(player* player, std::string& err);

};


#endif //LAMA_GAME_H


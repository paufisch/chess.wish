//
// Created by Manuel on 25.01.2021.
//
// The game_instance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#include "game_instance.h"

#include "server_network_manager.h"
#include "../common/network/responses/full_state_response.h"

bool game_instance::is_king_dead() {
    int king_count = 0;

    // TODO actually implement this when gamestate and board are done
    //for (i = 0; i < 8; ++i) {
    //    for (j = 0; j < 8; ++j) {
    //        if (_game_state->board.get_piece(i, j).get_kind() == "king") {
    //            ++king_count;
    //        }
    //    }
    //}
    if (king_count == 2) {
        return false;
    } else if (king_count == 1) {
        return true;
    } else if (king_count == 0) {
        throw LamaException("how did you manage to kill both kings?");
    } else {
        throw LamaException("how did you manage to have more than 2 kings on the board?");
    }
}


game_instance::game_instance() {
    _game_state = new game_state();
}

game_state *game_instance::get_game_state() {
    return _game_state;
}

std::string game_instance::get_id() {
    return _game_state->get_id();
}

bool game_instance::is_player_allowed_to_play(player *player) {
    return _game_state->is_allowed_to_play_now(player);
}

bool game_instance::is_full() {
    return _game_state->is_full();
}

bool game_instance::is_started() {
    return _game_state->is_started();
}

bool game_instance::is_finished() {
    return _game_state->is_finished();
}

std::vector<std::vector<bool>> game_instance::legal_moves(player* player, int coordinate_1, int coordinate_2, std::string& err) {
    if (is_player_allowed_to_play(player)) {
        return _game_state->select_piece(coordinate_1, coordinate_2);
    } else {
        throw LamaException("a player tried to find legal moves while it was their oponents turn");
    }
}

bool game_instance::move_piece(player *player, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2, std::string &err) {
    modification_lock.lock();
    if (_game_state->move_piece(coordinate_from_1, coordinate_from_2, coordinate_to_1, coordinate_to_2)) {
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

//bool game_instance::play_card(player *player, const std::string& card_id, std::string& err) {
//    modification_lock.lock();
//    if (_game_state->play_card(player, card_id, err)) {
//        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
//        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
//        modification_lock.unlock();
//        return true;
//    }
//    modification_lock.unlock();
//    return false;
//}

//bool game_instance::draw_card(player *player, card*& drawn_card, std::string& err) {
//    modification_lock.lock();
//    if (_game_state->draw_card(player, err)) {
//        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
//        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
//        modification_lock.unlock();
//        return true;
//    }
//    modification_lock.unlock();
//    return false;
//}

bool game_instance::resign(player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->resign(player, err)) {
        // send state update to all other players
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::start_game(player* player, std::string &err) {
    modification_lock.lock();
    if (_game_state->start_game(err)) {
        // send state update to all other players
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::try_remove_player(player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->remove_player(player, err)) {
        player->set_game_id("");
        // send state update to all other players
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::try_add_player(player *new_player, std::string &err) {
    modification_lock.lock();
    if (_game_state->add_player(new_player, err)) {
        new_player->set_game_id(get_id());
        // send state update to all other players
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), new_player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}


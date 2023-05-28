// The game_instance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#include "game_instance.h"

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
        throw ChessException("a player tried to find legal moves while it was their oponents turn");
    }
}

bool game_instance::move_piece(player *player, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2, std::string &err) {
    modification_lock.lock();
    if (_game_state->is_started() && !_game_state->is_finished()) {
        if (_game_state->move_piece(coordinate_from_1, coordinate_from_2, coordinate_to_1, coordinate_to_2)) {
            // notify the clients
            full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
            server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);
            modification_lock.unlock();
            return true;
        }
    } else {
        err = "The game isn't running!";
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::resign(player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->resign(player)) {
        // notify the clients
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
        // notify the clients
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
        _game_state->set_is_finished(true);
        _game_state->set_loser(player);
        // notify the clients
        full_state_response response = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(response, _game_state->get_players(), player);
        modification_lock.unlock();
        delete player;
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool game_instance::try_add_player(player *new_player, std::string &err) {
    modification_lock.lock();
    if (_game_state->add_player(new_player, err)) {
        new_player->set_game_id(get_id());
        // notify the clients
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), new_player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}


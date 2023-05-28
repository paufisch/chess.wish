// The GameInstance class is a wrapper around the GameState of an active instance of the game.
// This class contains functions to modify the contained GameState.

#include "GameInstance.h"

GameInstance::GameInstance() {
    _game_state = new GameState();
}

GameState *GameInstance::get_game_state() {
    return _game_state;
}

std::string GameInstance::get_id() {
    return _game_state->get_id();
}

bool GameInstance::is_player_allowed_to_play(Player *player) {
    return _game_state->is_allowed_to_play_now(player);
}

bool GameInstance::is_full() {
    return _game_state->is_full();
}

bool GameInstance::is_started() {
    return _game_state->is_started();
}

bool GameInstance::is_finished() {
    return _game_state->is_finished();
}

std::vector<std::vector<bool>> GameInstance::legal_moves(Player* player, int coordinate_1, int coordinate_2, std::string& err) {
    if (is_player_allowed_to_play(player)) {
        return _game_state->select_piece(coordinate_1, coordinate_2);
    } else {
        throw ChessException("a Player tried to find legal moves while it was their oponents turn");
    }
}

bool GameInstance::move_piece(Player *player, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2, std::string &err) {
    modification_lock.lock();
    if (_game_state->is_started() && !_game_state->is_finished()) {
        if (_game_state->move_piece(coordinate_from_1, coordinate_from_2, coordinate_to_1, coordinate_to_2)) {
            // notify the clients
            FullStateResponse state_update_msg = FullStateResponse(this->get_id(), *_game_state);
            ServerNetworkManager::broadcast_message(state_update_msg, _game_state->get_players(), player);
            modification_lock.unlock();
            return true;
        }
    } else {
        err = "The game isn't running!";
    }
    modification_lock.unlock();
    return false;
}

bool GameInstance::resign(Player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->resign(player)) {
        // notify the clients
        FullStateResponse state_update_msg = FullStateResponse(this->get_id(), *_game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool GameInstance::start_game(Player* player, std::string &err) {
    modification_lock.lock();
    if (_game_state->start_game(err)) {
        // notify the clients
        FullStateResponse state_update_msg = FullStateResponse(this->get_id(), *_game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, _game_state->get_players(), player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool GameInstance::try_remove_player(Player *player, std::string &err) {
    modification_lock.lock();
    if (_game_state->remove_player(player, err)) {
        player->set_game_id("");
        _game_state->set_is_finished(true);
        _game_state->set_loser(player);
        // notify the clients
        FullStateResponse response = FullStateResponse(this->get_id(), *_game_state);
        ServerNetworkManager::broadcast_message(response, _game_state->get_players(), player);
        modification_lock.unlock();
        delete player;
        return true;
    }
    modification_lock.unlock();
    return false;
}

bool GameInstance::try_add_player(Player *new_player, std::string &err) {
    modification_lock.lock();
    if (_game_state->add_player(new_player, err)) {
        new_player->set_game_id(get_id());
        // notify the clients
        FullStateResponse state_update_msg = FullStateResponse(this->get_id(), *_game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, _game_state->get_players(), new_player);
        modification_lock.unlock();
        return true;
    }
    modification_lock.unlock();
    return false;
}


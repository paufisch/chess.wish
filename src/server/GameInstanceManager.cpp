// The GameInstanceManager only exists on the server side. It stores all currently active games and offers
// functionality to retrieve game instances by id and adding players to games.
// If a new Player requests to join a game but no valid GameInstance is available, then this class
// will generate a new GameInstance and add it to the unordered_map of (active) game instances.

#include "GameInstanceManager.h"

// Initialize static map
std::unordered_map<std::string, GameInstance*> GameInstanceManager::games_lut = {};

GameInstance *GameInstanceManager::find_joinable_game_instance() {
    std::vector<std::string> to_remove;
    GameInstance* res = nullptr;
    games_lut_lock.lock_shared();
    for (auto it = games_lut.begin(); it != games_lut.end(); it++) {
        if (it->second->is_finished()) {    // also check if there are any finished games that can be removed
            to_remove.push_back(it->first);
        } else if (!it->second->is_full() && !it->second->is_started()) {
            res = it->second; // found a non-full, non-started game
            break;
        }
    }
    games_lut_lock.unlock_shared();

    if (res == nullptr) {
        // couldn't find a non-full, non-started game -> create a new one
        res = create_new_game();
    }

    // remove all finished games
    if (to_remove.size() > 0) {
        games_lut_lock.lock();
        for (auto& game_id : to_remove) {
            games_lut.erase(game_id);
        }
        games_lut_lock.unlock();
    }
    return res;
}

GameInstance* GameInstanceManager::create_new_game() {
    GameInstance* new_game = new GameInstance();
    games_lut_lock.lock();  // exclusive
    GameInstanceManager::games_lut.insert({new_game->get_id(), new_game});
    games_lut_lock.unlock();
    return new_game;
}

bool GameInstanceManager::try_get_game_instance(const std::string& game_id, GameInstance *&game_instance_ptr) {
    game_instance_ptr = nullptr;
    games_lut_lock.lock_shared();
    auto it = GameInstanceManager::games_lut.find(game_id);
    if (it != games_lut.end()) {
        game_instance_ptr = it->second;
    }
    games_lut_lock.unlock_shared();
    return game_instance_ptr != nullptr;
}

bool GameInstanceManager::try_get_player_and_game_instance(const std::string& player_id, Player *&player, GameInstance *&game_instance_ptr, std::string& err) {
    if (PlayerManager::try_get_player(player_id, player)) {
        if (GameInstanceManager::try_get_game_instance(player->get_game_id(), game_instance_ptr)) {
            return true;
        } else {
            err = "Could not find game_id" + player->get_game_id() + " associated with this Player";
        }
    } else {
        err = "Could not find requested Player " + player_id + " in database.";
    }
    return false;
}

bool GameInstanceManager::try_add_player_to_any_game(Player *player, GameInstance*& game_instance_ptr, std::string& err) {

    // check that Player is not already subscribed to another game
    if (player->get_game_id() != "") {
        if (game_instance_ptr != nullptr && player->get_game_id() != game_instance_ptr->get_id()) {
            err = "Could not join game with id " + game_instance_ptr->get_id() + ". Player is already active in a different game with id " + player->get_game_id();
        } else {
            err = "Could not join game. Player is already active in a game";
        }
        return false;
    }

    if (game_instance_ptr == nullptr) {
        // Join any non-full, non-started game
        for (int i = 0; i < 10; i++) {
            // make at most 10 attempts of joining a src (due to concurrency, the game could already be full or started by the time
            // try_add_player_to_any_game() is invoked) But with only few concurrent requests it should succeed in the first iteration.
            game_instance_ptr = find_joinable_game_instance();
            if (try_add_player(player, game_instance_ptr, err)) {
                return true;
            }
        }
        return false;
    }
    else {
        return try_add_player(player, game_instance_ptr, err);
    }
}

bool GameInstanceManager::try_add_player(Player *player, GameInstance *&game_instance_ptr, std::string& err) {
    if (player->get_game_id() != "") {
        if (player->get_game_id() != game_instance_ptr->get_id()) {
            err = "Player is already active in a different src with id " + player->get_game_id();
        } else {
            err = "Player is already active in this src";
        }
        return false;
    }

    if (game_instance_ptr->try_add_player(player, err)) {
        if (game_instance_ptr->get_game_state()->get_players().size() == 2 &&
            game_instance_ptr->get_game_state()->get_players().at(0)->get_color() == player->get_color()) {
            if (player->get_color() == white) {
                player->set_color(black);
            } else {
                player->set_color(white);
            }
        }
        player->set_game_id(game_instance_ptr->get_id());   // mark that this Player is playing in a src
        return true;
    } else {
        return false;
    }
}

bool GameInstanceManager::try_remove_player(std::string& player_id, std::string &err) {
    Player* player = nullptr;
    GameInstance* game_instance = nullptr;

    if (try_get_player_and_game_instance(player_id, player, game_instance, err)) {
        return try_remove_player(player, game_instance, err);
    }
    return false;
}

bool GameInstanceManager::try_remove_player(Player *player, GameInstance *&game_instance_ptr, std::string &err) {
    PlayerManager::remove_player(player->get_id());
    return game_instance_ptr->try_remove_player(player, err);
}


// The PlayerManager only exists on the server side. It stores all connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when they first connect to the server.

#include "PlayerManager.h"

// Initialize static map
std::unordered_map<std::string, Player*> PlayerManager::_players_lut = {};

bool PlayerManager::try_get_player(const std::string& player_id, Player *&player_ptr) {
    player_ptr = nullptr;
    _rw_lock.lock_shared();
    auto it = PlayerManager::_players_lut.find(player_id);
    if (it != _players_lut.end()) {
        player_ptr = it->second;
    }
    _rw_lock.unlock_shared();
    return player_ptr;
}

bool PlayerManager::add_or_get_player(std::string name, const std::string& player_id, Color color, Player *&player_ptr) {
    if (try_get_player(player_id, player_ptr)) {
        return true;
    }
    player_ptr = new Player(player_id, name, color);
    _rw_lock.lock();    // exclusive
    PlayerManager::_players_lut.insert({player_id, player_ptr});
    _rw_lock.unlock();
    return true;
}

void PlayerManager::remove_player(const std::string& player_id) {
    _rw_lock.lock();    // exclusive
    PlayerManager::_players_lut.erase(player_id);
    _rw_lock.unlock();
}


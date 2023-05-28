// The PlayerManager only exists on the server side. It stores all connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when they first connect to the server.

#ifndef CHESS_PLAYERMANAGER_H
#define CHESS_PLAYERMANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "../common/game_state/player/Player.h"

class PlayerManager {

private:

    inline static std::shared_mutex _rw_lock;
    static std::unordered_map<std::string, Player*> _players_lut;

public:
    static bool try_get_player(const std::string& player_id, Player*& player_ptr);
    static bool add_or_get_player(std::string name, const std::string& player_id, Color color, Player*& player_ptr);
    static void remove_player(const std::string& player_id);
};


#endif //CHESS_PLAYERMANAGER_H

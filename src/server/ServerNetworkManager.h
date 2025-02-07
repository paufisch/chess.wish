// The ServerNetworkManager handles all incoming messages and offers functionality to broadcast messages
// to all connected players of a game.

#ifndef CHESS_SERVERNETWORKMANAGER_H
#define CHESS_SERVERNETWORKMANAGER_H

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../common/network/requests/ClientRequest.h"
#include "../common/network/responses/ServerResponse.h"
#include "../common/game_state/player/Player.h"
#include "../common/game_state/GameState.h"

class ServerNetworkManager {
private:

    inline static ServerNetworkManager* _instance;
    inline static std::shared_mutex _rw_lock;
    inline static sockpp::tcp_acceptor _acc;

    inline static std::unordered_map<std::string, std::string> _player_id_to_address;
    inline static std::unordered_map<std::string, std::string> _address_to_player_id;
    inline static std::unordered_map<std::string, sockpp::tcp_socket> _address_to_socket;

    void connect(const std::string& url, const uint16_t  port);

    static void listener_loop();
    static void read_message(sockpp::tcp_socket socket, const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)>& message_handler);
    static void handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address);
    static ssize_t send_message(const std::string& msg, const std::string& address);

public:
    ServerNetworkManager();
    ~ServerNetworkManager();

    // Used to broadcast a ServerResponse (e.g. a FullStateResponse) to all 'players' except 'exclude'
    static void broadcast_message(ServerResponse& msg, const std::vector<Player*>& players, const Player* exclude);

    static void on_player_left(std::string player_id);
};



#endif //CHESS_SERVERNETWORKMANAGER_H

#ifndef CHESS_REQUEST_HANDLER_H
#define CHESS_REQUEST_HANDLER_H


#include "player_manager.h"
#include "game_instance_manager.h"
#include "game_instance.h"

#include "../common/network/requests/client_request.h"
#include "../common/network/requests/join_game_request.h"
#include "../common/network/requests/move_piece_request.h"
#include "../common/network/responses/server_response.h"
#include "../common/network/responses/request_response.h"

class request_handler {
public:
    static server_response* handle_request(const client_request* const req);
};
#endif //CHESS_REQUEST_HANDLER_H

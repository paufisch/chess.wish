#ifndef CHESS_REQUESTHANDLER_H
#define CHESS_REQUESTHANDLER_H


#include "PlayerManager.h"
#include "GameInstanceManager.h"
#include "GameInstance.h"

#include "../common/network/requests/ClientRequest.h"
#include "../common/network/requests/JoinGameRequest.h"
#include "../common/network/requests/MovePieceRequest.h"
#include "../common/network/responses/ServerResponse.h"
#include "../common/network/responses/RequestResponse.h"

class RequestHandler {
public:
    static ServerResponse* handle_request(const ClientRequest* const req);
};
#endif //CHESS_REQUESTHANDLER_H

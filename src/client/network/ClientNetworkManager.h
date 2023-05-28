#ifndef CHESS_CLIENTNETWORKMANAGER_H
#define CHESS_CLIENTNETWORKMANAGER_H


#include <string>
#include "ResponseListenerThread.h"
#include "../../common/network/requests/ClientRequest.h"


class ClientNetworkManager {

public:
    static void init(const std::string& host, const uint16_t port);
    static void sendRequest(const ClientRequest& request);
    static void parseResponse(const std::string& message);

private:
    static bool connect(const std::string& host, const uint16_t port);
    static sockpp::tcp_connector* _connection;
    static bool _connectionSuccess;
    static bool _failedToConnect;

};


#endif //CHESS_CLIENTNETWORKMANAGER_H

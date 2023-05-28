#ifndef CHESS_STARTGAMEREQUEST_H
#define CHESS_STARTGAMEREQUEST_H

#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

#ifdef CHESS_SERVER
#include "../../../server/GameInstanceManager.h"
#include "../../../server/GameInstance.h"
#endif

class StartGameRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit StartGameRequest(base_class_properties);

public:
    StartGameRequest(std::string game_id, std::string player_id);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static StartGameRequest* from_json(const rapidjson::Value& json);
};

#endif //LAMA_START_GAME_REQUEST_H
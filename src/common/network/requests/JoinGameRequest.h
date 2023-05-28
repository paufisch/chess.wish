#ifndef CHESS_JOINGAMEREQUEST_H
#define CHESS_JOINGAMEREQUEST_H

#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

#ifdef CHESS_SERVER
#include <string>
#include "../../../server/GameInstanceManager.h"
#include "../../../server/PlayerManager.h"
#include "../../../server/GameInstance.h"
#endif

class JoinGameRequest : public ClientRequest{

private:

    std::string _player_name;
    static std::string undefined_game_id;

    /*
     * Private constructor for deserialization
     */
    JoinGameRequest(base_class_properties, std::string name);

public:

    [[nodiscard]] std::string get_player_name() const {return this->_player_name;}

    /*
     * Constructor to join any game
     */
    JoinGameRequest(std::string player_id, std::string name);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static JoinGameRequest* from_json(const rapidjson::Value& json);
};


#endif //CHESS_JOINGAMEREQUEST_H

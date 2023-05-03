//
// Created by Manuel on 29.01.2021.
//

#ifndef LAMA_JOIN_GAME_REQUEST_H
#define LAMA_JOIN_GAME_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class join_game_request : public client_request{

private:

    static std::string undefined_game_id;
    /*
     * Private constructor for deserialization
     */
    join_game_request(base_class_properties);

public:

    /*
     * Constructor to join any game
     */
    join_game_request(std::string player_id);

    /*
     * Constructor to join a specific game
     */
    join_game_request(std::string game_id, std::string player_id);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static join_game_request* from_json(const rapidjson::Value& json);
};


#endif //LAMA_JOIN_GAME_REQUEST_H

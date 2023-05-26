//
// Created by Manuel on 29.01.2021.
//

#include "join_game_request.h"

#ifdef CHESS_SERVER
#include <string>
#include "../../../server/game_instance_manager.h"
#include "../../../server/player_manager.h"
#include "../../../server/game_instance.h"
#endif

std::string join_game_request::undefined_game_id {""};

// Public constructor
join_game_request::join_game_request(std::string player_id, std::string name) :
        client_request( client_request::create_base_class_properties(RequestType::join_game, uuid_generator::generate_uuid_v4(), player_id, join_game_request::undefined_game_id) ),
        _player_name(name)
{ }

join_game_request::join_game_request(std::string game_id, std::string player_id, std::string name) :
        client_request( client_request::create_base_class_properties(RequestType::join_game, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
        _player_name(name)
{ }

// private constructor for deserialization
join_game_request::join_game_request(client_request::base_class_properties props, std::string name) :
        client_request(props), _player_name(name)
{ }

void join_game_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("player_name", rapidjson::Value(_player_name, allocator), allocator);
}

join_game_request* join_game_request::from_json(const rapidjson::Value& json) {
    if (json.HasMember("player_name")) {
        return new join_game_request(client_request::extract_base_class_properties(json), json["player_name"].GetString());
    } else {
        throw ChessException("Could not parse join_game_request from json. player_name is missing.");
    }
}


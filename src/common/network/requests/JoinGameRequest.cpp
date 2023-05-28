#include "JoinGameRequest.h"

std::string JoinGameRequest::undefined_game_id {""};

// Public constructor
JoinGameRequest::JoinGameRequest(std::string player_id, std::string name) :
        ClientRequest(ClientRequest::create_base_class_properties(RequestType::join_game, UuidGenerator::generate_uuid_v4(), player_id, JoinGameRequest::undefined_game_id) ),
        _player_name(name)
{ }

// private constructor for deserialization
JoinGameRequest::JoinGameRequest(ClientRequest::base_class_properties props, std::string name) :
        ClientRequest(props), _player_name(name)
{ }

void JoinGameRequest::write_into_json(rapidjson::Value &json,
                                      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    json.AddMember("player_name", rapidjson::Value(_player_name, allocator), allocator);
}

JoinGameRequest* JoinGameRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("player_name")) {
        return new JoinGameRequest(ClientRequest::extract_base_class_properties(json), json["player_name"].GetString());
    } else {
        throw ChessException("Could not parse JoinGameRequest from json. player_name is missing.");
    }
}
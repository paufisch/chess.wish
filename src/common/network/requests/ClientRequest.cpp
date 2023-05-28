//
// Created by Manuel on 28.01.2021.
//

#include "ClientRequest.h"
#include "MovePieceRequest.h"
#include "JoinGameRequest.h"
#include "StartGameRequest.h"
#include "ResignRequest.h"

#include <iostream>

// for deserialization
const std::unordered_map<std::string, RequestType> ClientRequest::_string_to_request_type = {
        {"join_game", RequestType::join_game },
        {"start_game", RequestType::start_game},
        {"move_piece", RequestType::move_piece},
        {"resign", RequestType::resign}
};

// for serialization
const std::unordered_map<RequestType, std::string> ClientRequest::_request_type_to_string = {
        { RequestType::join_game, "join_game" },
        { RequestType::start_game, "start_game"},
        { RequestType::move_piece, "move_piece"},
        {RequestType::resign, "resign"}
};

// protected constructor. only used by subclasses
ClientRequest::ClientRequest(ClientRequest::base_class_properties props) :
        _type(props._type),
        _req_id(props._req_id),
        _player_id(props._player_id),
        _game_id(props._game_id)
{ }

// used by subclasses to retrieve information from the json stored by this superclass
ClientRequest::base_class_properties ClientRequest::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("player_id") && json.HasMember("game_id") && json.HasMember("req_id")) {
        std::string player_id = json["player_id"].GetString();
        std::string game_id = json["game_id"].GetString();
        std::string req_id = json["req_id"].GetString();
        return create_base_class_properties(
                ClientRequest::_string_to_request_type.at(json["type"].GetString()),
                req_id,
                player_id,
                game_id
        );
    }
    else
    {
        throw ChessException("Client Request did not contain player_id or game_id");
    }
}

ClientRequest::base_class_properties ClientRequest::create_base_class_properties(
        RequestType type,
        std::string req_id,
        std::string& player_id,
        std::string& game_id)
{
    ClientRequest::base_class_properties res;
    res._player_id = player_id;
    res._game_id = game_id;
    res._req_id = req_id;
    res._type = type;
    return res;
}

void ClientRequest::write_into_json(rapidjson::Value &json,
                                    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value type_val(_request_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value player_id_val(_player_id.c_str(), allocator);
    json.AddMember("player_id", player_id_val, allocator);

    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);

    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);
}

ClientRequest* ClientRequest::from_json(const rapidjson::Value &json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type = json["type"].GetString();
        const RequestType request_type = ClientRequest::_string_to_request_type.at(type);

        // Check which type of request it is and call the respective from_json constructor
        if (request_type == RequestType::move_piece) {
            return MovePieceRequest::from_json(json);
        }
        else if (request_type == RequestType::resign) {
            return ResignRequest::from_json(json);
        }
        else if (request_type == RequestType::join_game) {
            return JoinGameRequest::from_json(json);
        }
        else if (request_type == RequestType::start_game) {
            return StartGameRequest::from_json(json);
        } else {
            throw ChessException("Encountered unknown ClientRequest type " + type);
        }
    }
    throw ChessException("Could not determine type of ClientRequest. JSON was:\n" + JsonUtils::to_string(&json));
}

std::string ClientRequest::to_string() const {
    return "ClientRequest of type " + ClientRequest::_request_type_to_string.at(_type) + " for playerId " + _player_id + " and gameId " + _game_id;
}
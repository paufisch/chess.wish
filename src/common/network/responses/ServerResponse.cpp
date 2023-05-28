#include "ServerResponse.h"
#include "RequestResponse.h"
#include "FullStateResponse.h"

#include "../../exceptions/ChessException.h"

// for deserialization
const std::unordered_map<std::string, ResponseType> ServerResponse::_string_to_response_type = {
        {"req_response", ResponseType::req_response },
        {"full_state_msg", ResponseType::full_state_msg},
};
// for serialization
const std::unordered_map<ResponseType, std::string> ServerResponse::_response_type_to_string = {
        { ResponseType::req_response,   "req_response" },
        { ResponseType::full_state_msg, "full_state_msg"},
};

ServerResponse::ServerResponse(ServerResponse::base_class_properties params):
        _type(params.type),
        _game_id(params.game_id)
{ }

ResponseType ServerResponse::get_type() const {
    return this->_type;
}

std::string ServerResponse::get_game_id() const {
    return this->_game_id;
}

ServerResponse::base_class_properties ServerResponse::create_base_class_properties(ResponseType type, const std::string &game_id) {
    ServerResponse::base_class_properties params;
    params.type = type;
    params.game_id = game_id;
    return params;
}

ServerResponse::base_class_properties ServerResponse::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("type") && json.HasMember("game_id")) {
        std::string game_id = json["game_id"].GetString();
        return create_base_class_properties(
                ServerResponse::_string_to_response_type.at(json["type"].GetString()),
                game_id
        );
    }
    else
    {
        throw ChessException("Server Response did not contain game_id");
    }
}

ServerResponse *ServerResponse::from_json(const rapidjson::Value& json) {

    if (json.HasMember("type") && json["type"].IsString()) {
        std::string type = json["type"].GetString();
        ResponseType response_type = ServerResponse::_string_to_response_type.at(type);

        if (response_type == ResponseType::req_response) {
            return RequestResponse::from_json(json);
        } else if (response_type == ResponseType::full_state_msg) {
            return FullStateResponse::from_json(json);
        } else {
            throw ChessException("Encountered unknown ServerResponse type " + response_type);
        }
    }
    throw ChessException("Could not determine type of ClientRequest");
}

void ServerResponse::write_into_json(rapidjson::Value &json,
                                     rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value type_val(_response_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);
}
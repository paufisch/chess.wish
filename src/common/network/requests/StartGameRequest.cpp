#include "StartGameRequest.h"

// Public constructor
StartGameRequest::StartGameRequest(std::string game_id, std::string player_id)
        : ClientRequest(ClientRequest::create_base_class_properties(RequestType::start_game, UuidGenerator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
StartGameRequest::StartGameRequest(ClientRequest::base_class_properties props) :
        ClientRequest(props)
{ }

StartGameRequest* StartGameRequest::from_json(const rapidjson::Value& json) {
    return new StartGameRequest(ClientRequest::extract_base_class_properties(json));
}

void StartGameRequest::write_into_json(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
}

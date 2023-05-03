//
// Created by jann on 5/3/23.
//

#include "resign_request.h"

// Public constructor
resign_request::resign_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::resign, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
resign_request::resign_request(client_request::base_class_properties props) :
        client_request(props)
{ }

resign_request* resign_request::from_json(const rapidjson::Value &json) {
    return new resign_request(client_request::extract_base_class_properties((json)));
}

void resign_request::write_into_json(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
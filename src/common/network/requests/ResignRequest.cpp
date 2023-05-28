#include "ResignRequest.h"

// Public constructor
ResignRequest::ResignRequest(std::string game_id, std::string player_id)
        : ClientRequest(ClientRequest::create_base_class_properties(RequestType::resign, UuidGenerator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
ResignRequest::ResignRequest(ClientRequest::base_class_properties props) :
        ClientRequest(props)
{ }

ResignRequest* ResignRequest::from_json(const rapidjson::Value &json) {
    return new ResignRequest(ClientRequest::extract_base_class_properties((json)));
}

void ResignRequest::write_into_json(rapidjson::Value &json,
                                    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
}
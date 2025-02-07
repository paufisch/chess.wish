#include "MovePieceRequest.h"

// Public constructor
MovePieceRequest::MovePieceRequest(std::string game_id, std::string player_id, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2)
        : ClientRequest(ClientRequest::create_base_class_properties(RequestType::move_piece, UuidGenerator::generate_uuid_v4(), player_id, game_id) ),
          _coordinate_from_1(coordinate_from_1), _coordinate_from_2(coordinate_from_2), _coordinate_to_1(coordinate_to_1), _coordinate_to_2(coordinate_to_2)
{ }

// private constructor for deserialization
MovePieceRequest::MovePieceRequest(ClientRequest::base_class_properties props, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2) :
        ClientRequest(props),
        _coordinate_from_1(coordinate_from_1), _coordinate_from_2(coordinate_from_2), _coordinate_to_1(coordinate_to_1), _coordinate_to_2(coordinate_to_2)
{ }

MovePieceRequest* MovePieceRequest::from_json(const rapidjson::Value& json) {
    base_class_properties props = ClientRequest::extract_base_class_properties(json);
    if (json.HasMember("coordinate_from_1") && json.HasMember("coordinate_from_2") && json.HasMember("coordinate_to_1") && json.HasMember("coordinate_to_2")) {
        return new MovePieceRequest(props, json["coordinate_from_1"].GetInt(), json["coordinate_from_2"].GetInt(), json["coordinate_to_1"].GetInt(), json["coordinate_to_2"].GetInt());
    } else {
        throw ChessException("Could not find the coordinates in MovePieceRequest");
    }
}

void MovePieceRequest::write_into_json(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);

    json.AddMember("coordinate_from_1", _coordinate_from_1, allocator);
    json.AddMember("coordinate_from_2", _coordinate_from_2, allocator);
    json.AddMember("coordinate_to_1", _coordinate_to_1, allocator);
    json.AddMember("coordinate_to_2", _coordinate_to_2, allocator);
}
//
// Created by jann on 5/3/23.
//

#include "select_piece_request.h"

// Public constructor
select_piece_request::select_piece_request(std::string game_id, std::string player_id, int coordinate_1, int coordinate_2)
        : client_request(client_request::create_base_class_properties(RequestType::move_piece, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
          _coordinate_1(coordinate_1), _coordinate_2(coordinate_2)
{ }

// private constructor for deserialization
select_piece_request::select_piece_request(client_request::base_class_properties props, int coordinate_1, int coordinate_2) :
        client_request(props),
        _coordinate_1(coordinate_1), _coordinate_2(coordinate_2)
{ }

select_piece_request* select_piece_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("coordinate_1") && json.HasMember("coordinate_2")) {
        return new select_piece_request(props, json["coordinate_1"].GetInt(), json["coordinate_2"].GetInt());
    } else {
        throw LamaException("Could not find the coordinates in select_piece_request");
    }
}

void select_piece_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);

    rapidjson::Value coordinate_1_val(std::to_string(_coordinate_1), allocator);  //std::to_string might cause problems
    json.AddMember("coordinate_1", coordinate_1_val, allocator);

    rapidjson::Value coordinate_2_val(std::to_string(_coordinate_2), allocator);
    json.AddMember("coordinate_2", coordinate_2_val, allocator);

}
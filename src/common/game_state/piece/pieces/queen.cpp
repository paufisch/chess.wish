//
// Created by marcel on 10.05.23.
//

#include "queen.h"
#include "../../../exceptions/LamaException.h"

Queen::Queen(piece::base_class_properties props)
    : piece(props)
{ }

Queen::Queen(std::string piece_ID, Color color, PieceType type)
    : piece(piece_ID, color, type)
{ }

void Queen::write_into_json(rapidjson::Value &json,
                            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    piece::write_into_json(json, allocator);
}

Queen *Queen::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new Queen(piece_ID, piece::_string_to_color.at(color), piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Queen constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> Queen::legal_moves(unsigned int row, unsigned int col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));

    return possible_moves;
}

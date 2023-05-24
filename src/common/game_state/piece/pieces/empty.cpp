//
// Created by marcel on 24.05.23.
//


#include "empty.h"
#include "../../../exceptions/LamaException.h"

Empty::Empty(std::string piece_ID, Color color, PieceType type)
        : Piece(piece_ID, color, type)
{ }

void Empty::write_into_json(rapidjson::Value &json,
                             rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    Piece::write_into_json(json, allocator);
}

Piece* Empty::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new Empty(piece_ID, Piece::_string_to_color.at(color), Piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Bishop constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> Empty::legal_moves(unsigned int init_row, unsigned int init_col, board *_board) {
    throw LamaException("Called legal_moves on an empty piece.");
}

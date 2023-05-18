//
// Created by marcel on 10.05.23.
//

#include "king.h"
#include "../../../exceptions/LamaException.h"

king::king(piece::base_class_properties props)
    : piece(props)
{ }

king::king(std::string piece_ID, Color color, PieceType type)
        : piece(piece_ID, color, type)
{ }

void king::write_into_json(rapidjson::Value &json,
                           rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    piece::write_into_json(json, allocator);
}

class king *king::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new king(piece_ID, piece::_string_to_color.at(color), piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("King constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> king::legal_moves(unsigned int init_row, unsigned int init_col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));
    for (int row = init_row - 1; row < init_row + 2; ++row) {
        for (int col = init_col - 1; col < init_col + 2; ++col) {
            if (!(row == init_row && col == init_col)) {
                if ((-1 < row && row < 8) && (-1 < col && col < 8)) {
                    piece* piece = _board->get_piece(row, col);
                    if (piece == nullptr || piece->get_color() != this->_color) {
                        possible_moves[row][col] = true;
                    }
                }
            }
        }
    }
    return possible_moves;
}

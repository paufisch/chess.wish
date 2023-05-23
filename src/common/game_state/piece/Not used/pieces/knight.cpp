//
// Created by marcel on 10.05.23.
//

#include "knight.h"
#include "../../../../exceptions/LamaException.h"



Knight::Knight(Piece::base_class_properties props)
    : Piece(props)
{}

Knight::Knight(std::string piece_ID, Color color, PieceType type)
    : Piece(piece_ID, color, type)
{ }


void Knight::write_into_json(rapidjson::Value &json,
                             rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    Piece::write_into_json(json, allocator);
}

Piece * Knight::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new Knight(piece_ID, Piece::_string_to_color.at(color), Piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Knight constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

void Knight::position(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col, int row_offset, int col_offset) {
    if (_board->get_piece(init_row + row_offset, init_col + col_offset) != nullptr) {
        Piece* piece = _board->get_piece(init_row + row_offset, init_col + col_offset);
        if (piece->get_color() != this->_color) {
            possible_moves[init_row + row_offset][init_col + col_offset];
        }
    } else {
        possible_moves[init_row + row_offset][init_col + col_offset];
    }
}

std::vector<std::vector<bool>> Knight::legal_moves(unsigned int init_row, unsigned int init_col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));

    if (init_row < 5 && init_col < 7) {
        position(possible_moves, init_row, init_col, 3, 1);
    }

    if (init_row < 5 && init_col > 0) {
        position(possible_moves, init_row, init_col, 3, -1);
    }

    if (init_row > 2 && init_col < 7) {
        position(possible_moves, init_row, init_col, -3, 1);
    }

    if (init_row > 2 && init_col > 0) {
        position(possible_moves, init_row, init_col, -3, -1);
    }

    if (init_row < 7 && init_col < 5) {
        position(possible_moves, init_row, init_col, 1, 3);
    }

    if (init_row > 0 && init_col < 5) {
        position(possible_moves, init_row, init_col, -1, 3);
    }

    if (init_row < 7 && init_col > 2) {
        position(possible_moves, init_row, init_col, 1, -3);
    }

    if (init_row > 0 && init_col > 2) {
        position(possible_moves, init_row, init_col, -1, -3);
    }
    return possible_moves;
}


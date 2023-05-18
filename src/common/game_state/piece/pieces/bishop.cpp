//
// Created by marcel on 10.05.23.
//

#include "bishop.h"
#include "../../../exceptions/LamaException.h"



bishop::bishop(piece::base_class_properties props)
    : piece(props)
{ }

bishop::bishop(std::string piece_ID, Color color, PieceType type)
    : piece(piece_ID, color, type)
{ }

void bishop::write_into_json(rapidjson::Value &json,
                             rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    piece::write_into_json(json, allocator);
}

class bishop* bishop::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new bishop(piece_ID, piece::_string_to_color.at(color), piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Bishop constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> bishop::legal_moves(unsigned int init_row, unsigned int init_col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));

    for (unsigned int i = 1; ((init_row + i < 8) || (init_col + i < 8)); ++i) {
        piece* piece = _board->get_piece(init_row + i, init_col + i);
        if (piece == nullptr) {
            possible_moves[init_row + i][init_col + i] = true;
        } else if (piece->get_color() == this->_color) {
            i += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row + i][init_col + i] = true;
            i += 8;
        }
    }

    for (unsigned int i = 1; ((init_row + i < 8) || (init_col - i > -1)); ++i) {
        piece* piece = _board->get_piece(init_row + i, init_col - i);
        if (piece == nullptr) {
            possible_moves[init_row + i][init_col - i] = true;
        } else if (piece->get_color() == this->_color) {
            i += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row + i][init_col - i] = true;
            i += 8;
        }
    }

    for (unsigned int i = 1; ((init_row - i > -1) || (init_col - i > -1)); ++i) {
        piece* piece = _board->get_piece(init_row - i, init_col - i);
        if (piece == nullptr) {
            possible_moves[init_row - i][init_col - i] = true;
        } else if (piece->get_color() == this->_color) {
            i += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row - i][init_col - i] = true;
            i += 8;
        }
    }

    for (unsigned int i = 1; ((init_row - i > -1) || (init_col + i < 8)); ++i) {
        piece* piece = _board->get_piece(init_row - i, init_col + i);
        if (piece == nullptr) {
            possible_moves[init_row - i][init_col + i] = true;
        } else if (piece->get_color() == this->_color) {
            i += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row - i][init_col + i] = true;
            i += 8;
        }
    }
    return possible_moves;
}

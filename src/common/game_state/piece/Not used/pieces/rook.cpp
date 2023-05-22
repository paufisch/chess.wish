//
// Created by marcel on 10.05.23.
//

#include "rook.h"
#include "../../../../exceptions/LamaException.h"


Rook::Rook(Piece::base_class_properties props)
    : Piece(props)
{ }

Rook::Rook(std::string piece_ID, Color color, PieceType type)
    : Piece(piece_ID, color, type)
{ }

void Rook::write_into_json(rapidjson::Value &json,
                           rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    Piece::write_into_json(json, allocator);
}

Piece* Rook::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new Rook(piece_ID, Piece::_string_to_color.at(color), Piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Rook constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> Rook::legal_moves(unsigned int init_row, unsigned int init_col) {
    //return value
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));

    for (int row = init_row + 1; row < 8; ++row) {
        //piece which is on the current field
        Piece* piece = _board->get_piece(row, init_col);
        if (piece == nullptr) {
            possible_moves[row][init_col] = true;
        } else if (piece->get_color() == this->_color) {
            row += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[row][init_col] = true;
            row += 8;
        }
    }

    for (int row = init_row - 1; row > -1; --row) {
        //piece which is on the current field
        Piece* piece = _board->get_piece(row, init_col);
        if (piece == nullptr) {
            possible_moves[row][init_col] = true;
        } else if (piece->get_color() == this->_color) {
            row -= 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[row][init_col] = true;
            row -= 8;
        }
    }

    for (int col = init_col + 1; col < 8; ++col) {
        //piece which is on the current field
        Piece* piece = _board->get_piece(init_row, col);
        if (piece == nullptr) {
            possible_moves[init_row][col] = true;
        } else if (piece->get_color() == this->_color) {
            col += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row][col] = true;
            col += 8;
        }
    }

    for (int col = init_col - 1; col > -1; --col) {
        //piece which is on the current field
        Piece* piece = _board->get_piece(init_row, col);
        if (piece == nullptr) {
            possible_moves[init_row][col] = true;
        } else if (piece->get_color() == this->_color) {
            col -= 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row][col] = true;
            col -= 8;
        }
    }
    return possible_moves;
}




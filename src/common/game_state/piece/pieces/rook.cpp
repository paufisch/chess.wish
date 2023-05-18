//
// Created by marcel on 10.05.23.
//

#include "rook.h"
#include "../../../exceptions/LamaException.h"


rook::rook(piece::base_class_properties props)
    : piece(props)
{ }

rook::rook(std::string piece_ID, Color color, PieceType type)
    : piece(piece_ID, color, type)
{ }

rook* rook::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new rook(piece_ID, piece::_string_to_color.at(color), piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

void rook::write_into_json(rapidjson::Value &json,
                           rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    piece::write_into_json(json, allocator);
}

std::vector<std::vector<bool>> rook::legal_moves(unsigned int init_row, unsigned int init_col) {
    //return value
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));
    //the initial piece
    piece* init_piece = _board->get_piece(init_row, init_col);

    for (int row = init_row + 1; row < 8; ++row) {
        //piece which is on the current field
        piece* piece = _board->get_piece(row, init_col);
        if (piece == nullptr) {
            possible_moves[row][init_col] = true;
        } else if (piece->get_color() == init_piece->get_color()) {
            row += 8;
        } else if (piece->get_color() != init_piece->get_color()) {
            possible_moves[row][init_col] = true;
            row += 8;
        }
    }

    for (int row = init_row - 1; row > -1; --row) {
        //piece which is on the current field
        piece* piece = _board->get_piece(row, init_col);
        if (piece == nullptr) {
            possible_moves[row][init_col] = true;
        } else if (piece->get_color() == init_piece->get_color()) {
            row -= 8;
        } else if (piece->get_color() != init_piece->get_color()) {
            possible_moves[row][init_col] = true;
            row -= 8;
        }
    }

    for (int col = init_col + 1; col < 8; ++col) {
        //piece which is on the current field
        piece* piece = _board->get_piece(init_row, col);
        if (piece == nullptr) {
            possible_moves[init_row][col] = true;
        } else if (piece->get_color() == init_piece->get_color()) {
            col += 8;
        } else if (piece->get_color() != init_piece->get_color()) {
            possible_moves[init_row][col] = true;
            col += 8;
        }
    }

    for (int col = init_col - 1; col > -1; --col) {
        //piece which is on the current field
        piece* piece = _board->get_piece(init_row, col);
        if (piece == nullptr) {
            possible_moves[init_row][col] = true;
        } else if (piece->get_color() == init_piece->get_color()) {
            col -= 8;
        } else if (piece->get_color() != init_piece->get_color()) {
            possible_moves[init_row][col] = true;
            col -= 8;
        }
    }
}




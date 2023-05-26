//
// Created by marcel on 10.05.23.
//

#include "queen.h"
#include "../../../../exceptions/ChessException.h"


Queen::Queen(Piece::base_class_properties props)
    : Piece(props)
{ }


Queen::Queen(std::string piece_ID, Color color, PieceType type)
    : Piece(piece_ID, color, type)
{ }


void Queen::write_into_json(rapidjson::Value &json,
                            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    Piece::write_into_json(json, allocator);
}



Piece* Queen::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new Queen(piece_ID, Piece::_string_to_color.at(color), Piece::_string_to_piece_type.at(type));
    } else {
        throw ChessException("Queen constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}


void Queen::bishop_capabilities(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col) {
    for (unsigned int i = 1; ((init_row + i < 8) || (init_col + i < 8)); ++i) {
        Piece* piece = _board->get_piece(init_row + i, init_col + i);
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
        Piece* piece = _board->get_piece(init_row + i, init_col - i);
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
        Piece* piece = _board->get_piece(init_row - i, init_col - i);
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
        Piece* piece = _board->get_piece(init_row - i, init_col + i);
        if (piece == nullptr) {
            possible_moves[init_row - i][init_col + i] = true;
        } else if (piece->get_color() == this->_color) {
            i += 8;
        } else if (piece->get_color() != this->_color) {
            possible_moves[init_row - i][init_col + i] = true;
            i += 8;
        }
    }
}

void Queen::rook_capabilities(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col) {
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
}


std::vector<std::vector<bool>> Queen::legal_moves(unsigned int init_row, unsigned int init_col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));
    bishop_capabilities(possible_moves, init_row, init_col);
    rook_capabilities(possible_moves, init_row, init_col);
    return possible_moves;
}

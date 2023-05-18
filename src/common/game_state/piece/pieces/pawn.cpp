//
// Created by marcel on 10.05.23.
//

#include "pawn.h"
#include "../../../exceptions/LamaException.h"


pawn::pawn(piece::base_class_properties props)
    : piece(props)
{ }

pawn::pawn(std::string piece_ID, Color color, PieceType type)
    : piece(piece_ID, color, type)
{ }

void pawn::write_into_json(rapidjson::Value &json,
                           rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    piece::write_into_json(json, allocator);
}

class pawn* pawn::from_json(const rapidjson::Value &json) {
    if (json.HasMember("piece_ID") && json.HasMember("color") && json.HasMember("type") &&
        json["piece_ID"].IsString() && json["color"].IsString() && json["type"].IsString()){
        std::string piece_ID = json["piece_ID"].GetString();
        std::string color = json["color"].GetString();
        std::string type = json["type"].GetString();
        return new pawn(piece_ID, piece::_string_to_color.at(color), piece::_string_to_piece_type.at(type));
    } else {
        throw LamaException("Pawn constructor did not get all variables. JSON was:\n" + json_utils::to_string(&json));
    }
}

std::vector<std::vector<bool>> pawn::legal_moves(unsigned int init_row, unsigned int init_col) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));

    //Decide if the color is white or black because of the direction the pawns can go ("up" or "down")
    if (this->_color == white) {
        //Checking if a piece is in front of the pawn
        //We can go left or right but just when a piece from the other color is there
        if (_board->get_piece(init_row + 1, init_col) != nullptr && init_row < 7) {
            //If a piece is on the left
            if (_board->get_piece(init_row + 1, init_col - 1) != nullptr && init_col > 0) {
                //Need the piece to find out its color
                piece* piece = _board->get_piece(init_row + 1, init_col - 1);
                if (piece->get_color() != white) {
                    possible_moves[init_row + 1][init_col - 1] = true;
                }
            }
            //If a piece is on the right
            if (_board->get_piece(init_row + 1, init_col + 1) != nullptr && init_col < 7) {
                //Need the piece to find out its color
                piece* piece = _board->get_piece(init_row + 1, init_col + 1);
                if (piece->get_color() != white) {
                    possible_moves[init_row + 1][init_col + 1] = true;
                }
            }
        } else if (init_row < 7){
            possible_moves[init_row + 1][init_col] = true;
            if (init_row == 1) {
                if (_board->get_piece(init_row + 2, init_col) == nullptr) {
                    possible_moves[init_row + 2][init_col] = true;
                }
            }
        }
    } else if (this->_color == black) {
        //Checking if a piece is in front of the pawn
        //We can go left or right but just when a piece from the other color is there
        if (_board->get_piece(init_row - 1, init_col) != nullptr && init_row > 0) {
            //If a piece is on the left
            if (_board->get_piece(init_row - 1, init_col - 1) != nullptr && init_col > 0) {
                //Need the piece to find out its color
                piece* piece = _board->get_piece(init_row - 1, init_col - 1);
                if (piece->get_color() != black) {
                    possible_moves[init_row - 1][init_col - 1] = true;
                }
            }
            //If a piece is on the right
            if (_board->get_piece(init_row - 1, init_col + 1) != nullptr && init_col < 7) {
                //Need the piece to find out its color
                piece* piece = _board->get_piece(init_row - 1, init_col + 1);
                if (piece->get_color() != black) {
                    possible_moves[init_row - 1][init_col + 1] = true;
                }
            }
        } else if (init_row > 0){
            possible_moves[init_row - 1][init_col] = true;
            if (init_row == 6) {
                if (_board->get_piece(init_row - 2, init_col) == nullptr) {
                    possible_moves[init_row - 2][init_col] = true;
                }
            }
        }
    }
    return possible_moves;
}



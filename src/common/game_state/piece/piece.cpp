//
// Created by marcel on 04.05.23.
//

#include "piece.h"
#include "../../exceptions/LamaException.h"
//#include "pieces/queen.h"
//#include "pieces/king.h"
//#include "pieces/pawn.h"
//#include "pieces/knight.h"
//#include "pieces/bishop.h"
//#include "pieces/rook.h"


// for deserialization
const std::unordered_map<std::string, PieceType> Piece::_string_to_piece_type = {
        {"rook", PieceType::rook},
        {"knight", PieceType::knight},
        {"bishop", PieceType::bishop},
        {"king", PieceType::king},
        {"queen", PieceType::queen},
        {"pawn", PieceType::pawn}
};


const std::unordered_map<std::string, Color> Piece::_string_to_color = {
        {"white", Color::white},
        {"black", Color::black},
};


// for serialization
const std::unordered_map<PieceType, std::string> Piece::_piece_type_to_string = {
        { PieceType::rook, "rook" },
        { PieceType::knight, "knight"},
        { PieceType::bishop, "bishop"},
        { PieceType::king, "king"},
        { PieceType::queen, "queen"},
        { PieceType::pawn, "pawn"}
};


const std::unordered_map<Color, std::string> Piece::_color_to_string = {
        { Color::white, "white" },
        { Color::black, "black"},

};


// used by subclasses to retrieve information from the json stored by this superclass
Piece::base_class_properties Piece::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("piece_ID") && json.HasMember("color")) {
        std::string piece_ID = json["piece_ID"].GetString();
        Color color = _string_to_color.at(json["color"].GetString());

        return create_base_class_properties(
                Piece::_string_to_piece_type.at(json["type"].GetString()),
                piece_ID,
                color
        );
    }
    else
    {
        throw LamaException("Client Request did not contain piece_ID or color");
    }
}


Piece::base_class_properties Piece::create_base_class_properties(
        PieceType type,
        std::string piece_ID,
        Color color)
{
    Piece::base_class_properties res;
    res._piece_ID = piece_ID;
    res._color = color;
    res._type = type;
    return res;
}


Piece::Piece(std::string id) : unique_serializable(id) { }


Piece::Piece(std::string piece_ID, Color color, PieceType type)
        : _piece_ID(piece_ID), _color(color), _type(type)
{ }


// protected constructor. only used by subclasses
Piece::Piece(Piece::base_class_properties props) :
        _type(props._type),
        _piece_ID(props._piece_ID),
        _color(props._color)
        //_board(props._board)
{}

Piece::~Piece(){}

void Piece::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {

    rapidjson::Value type_val(_piece_type_to_string.at(get_type()).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value piece_ID_val(_piece_ID.c_str(), allocator);
    json.AddMember("piece_ID", piece_ID_val, allocator);

    rapidjson::Value color_val(_color_to_string.at(get_color()).c_str(), allocator);
    json.AddMember("color", piece_ID_val, allocator);
}


Piece* Piece::from_json(const rapidjson::Value& json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type = json["type"].GetString();
        const PieceType piece_type = Piece::_string_to_piece_type.at(type);

        const std::string color_type = json["color"].GetString();
        const Color color = Piece::_string_to_color.at(color_type);

        std::string piece_ID = json["piece_ID"].GetString();



        if (piece_type == PieceType::rook) {
            return new Piece(piece_ID, color, piece_type);
        }
        else if (piece_type == PieceType::knight) {
            return new Piece(piece_ID, color, piece_type);
        }
        else if (piece_type == PieceType::bishop) {
            return new Piece(piece_ID, color, piece_type);
        }
        else if (piece_type == PieceType::king) {
            return new Piece(piece_ID, color, piece_type);
        }
        else if (piece_type == PieceType::queen) {
            return new Piece(piece_ID, color, piece_type);
        }
        else if (piece_type == PieceType::pawn) {
            return new Piece(piece_ID, color, piece_type);
        } else {
            throw LamaException("Encountered unknown Piece type " + type);
        }
    }
    throw LamaException("Could not determine type of Piece. JSON was:\n" + json_utils::to_string(&json));
}

void Piece::position(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col, int row_offset, int col_offset, board* _board) {
    if (_board->get_piece(init_row + row_offset, init_col + col_offset) != nullptr) {
        Piece* piece = _board->get_piece(init_row + row_offset, init_col + col_offset);
        if (piece->get_color() != this->_color) {
            possible_moves[init_row + row_offset][init_col + col_offset];
        }
    } else {
        possible_moves[init_row + row_offset][init_col + col_offset];
    }
}

std::vector<std::vector<bool>> Piece::pawn_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
    //Decide if the color is white or black because of the direction the pawns can go ("up" or "down")
    if (this->_color == white) {
        //Checking if a piece is in front of the pawn
        //We can go left or right but just when a piece from the other color is there
        if (_board->get_piece(init_row + 1, init_col) != nullptr && init_row < 7) {
            //If a piece is on the left
            if (_board->get_piece(init_row + 1, init_col - 1) != nullptr && init_col > 0) {
                //Need the piece to find out its color
                Piece* piece = _board->get_piece(init_row + 1, init_col - 1);
                if (piece->get_color() != white) {
                    possible_moves[init_row + 1][init_col - 1] = true;
                }
            }
            //If a piece is on the right
            if (_board->get_piece(init_row + 1, init_col + 1) != nullptr && init_col < 7) {
                //Need the piece to find out its color
                Piece* piece = _board->get_piece(init_row + 1, init_col + 1);
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
                Piece* piece = _board->get_piece(init_row - 1, init_col - 1);
                if (piece->get_color() != black) {
                    possible_moves[init_row - 1][init_col - 1] = true;
                }
            }
            //If a piece is on the right
            if (_board->get_piece(init_row - 1, init_col + 1) != nullptr && init_col < 7) {
                //Need the piece to find out its color
                Piece* piece = _board->get_piece(init_row - 1, init_col + 1);
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
std::vector<std::vector<bool>> Piece::rook_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
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
std::vector<std::vector<bool>> Piece::knight_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
    if (init_row < 5 && init_col < 7) {
        position(possible_moves, init_row, init_col, 3, 1, _board);
    }

    if (init_row < 5 && init_col > 0) {
        position(possible_moves, init_row, init_col, 3, -1, _board);
    }

    if (init_row > 2 && init_col < 7) {
        position(possible_moves, init_row, init_col, -3, 1, _board);
    }

    if (init_row > 2 && init_col > 0) {
        position(possible_moves, init_row, init_col, -3, -1, _board);
    }

    if (init_row < 7 && init_col < 5) {
        position(possible_moves, init_row, init_col, 1, 3, _board);
    }

    if (init_row > 0 && init_col < 5) {
        position(possible_moves, init_row, init_col, -1, 3, _board);
    }

    if (init_row < 7 && init_col > 2) {
        position(possible_moves, init_row, init_col, 1, -3, _board);
    }

    if (init_row > 0 && init_col > 2) {
        position(possible_moves, init_row, init_col, -1, -3, _board);
    }
    return possible_moves;
}
std::vector<std::vector<bool>> Piece::bishop_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
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
    return possible_moves;
}
std::vector<std::vector<bool>> Piece::king_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
    for (int row = init_row - 1; row < init_row + 2; ++row) {
        for (int col = init_col - 1; col < init_col + 2; ++col) {
            if (!(row == init_row && col == init_col)) {
                if ((-1 < row && row < 8) && (-1 < col && col < 8)) {
                    Piece* piece = _board->get_piece(row, col);
                    if (piece == nullptr || piece->get_color() != this->_color) {
                        possible_moves[row][col] = true;
                    }
                }
            }
        }
    }
    return possible_moves;
}
std::vector<std::vector<bool>> Piece::queen_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves, board* _board) {
    possible_moves = rook_moves(init_row, init_col, possible_moves, _board);
    return bishop_moves(init_row, init_col, possible_moves, _board);
}

std::vector<std::vector<bool>> Piece::legal_moves(unsigned int init_row, unsigned int init_col, board* _board) {
    std::vector<std::vector<bool>> possible_moves(8, std::vector<bool>(8, false));
    switch (_type) {
        case PieceType::rook:
            return rook_moves(init_row, init_col, possible_moves, _board);
            break;
        case PieceType::knight:
            return knight_moves(init_row, init_col, possible_moves, _board);
            break;
        case PieceType::bishop:
            return bishop_moves(init_row, init_col, possible_moves, _board);
            break;
        case PieceType::king:
            return king_moves(init_row, init_col, possible_moves, _board);
            break;
        case PieceType::queen:
            return queen_moves(init_row, init_col, possible_moves, _board);
            break;
        case PieceType::pawn:
            return pawn_moves(init_row, init_col, possible_moves, _board);
            break;
    }
}












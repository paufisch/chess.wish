//
// Created by marcel on 04.05.23.
//

#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "vector"
#include <unordered_map>
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/json_utils.h"
#include "../color.h"
#include "../board.h"

#include "pieces/queen.h"
#include "pieces/king.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/empty.h"


enum PieceType {
    rook,
    knight,
    bishop,
    king,
    queen,
    pawn,
    empty
};

class Piece : public unique_serializable{
protected:

    PieceType _type;
    std::string _piece_ID;
    Color _color;

    // for deserialization
    static const std::unordered_map<std::string, PieceType> _string_to_piece_type;
    static const std::unordered_map<std::string, Color> _string_to_color;
    // for serialization
    static const std::unordered_map<PieceType, std::string> _piece_type_to_string;
    static const std::unordered_map<Color, std::string> _color_to_string;

public:

    Piece(std::string piece_ID, Color color, PieceType type);

    [[nodiscard]] PieceType get_type() const { return this->_type; }
    [[nodiscard]] std::string get_piece_ID() const { return this->_piece_ID; }
    [[nodiscard]] Color get_color() const { return this->_color; }

    // serializable interface
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;
    static Piece* from_json(const rapidjson::Value& json);

    // checks for legal moves
    virtual std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col, board* board) = 0;

};

#endif //PIECE_H
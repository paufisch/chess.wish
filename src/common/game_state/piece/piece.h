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

    struct base_class_properties {
        PieceType _type;
        std::string _piece_ID;
        Color _color;
        class board* _board;
    };

    PieceType _type;
    std::string _piece_ID;
    Color _color;
    class board* _board;

    explicit Piece(base_class_properties); // base constructor
    static base_class_properties create_base_class_properties(PieceType type, std::string piece_ID, Color color);
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

    // for deserialization
    static const std::unordered_map<std::string, PieceType> _string_to_piece_type;
    static const std::unordered_map<std::string, Color> _string_to_color;
    // for serialization
    static const std::unordered_map<PieceType, std::string> _piece_type_to_string;
    static const std::unordered_map<Color, std::string> _color_to_string;

public:

    //from_diff constructor
    Piece(std::string id);
    //deserialization constructor
    Piece(std::string piece_ID, Color color, PieceType type);

    virtual ~Piece();

    [[nodiscard]] PieceType get_type() const { return this->_type; }
    [[nodiscard]] std::string get_piece_ID() const { return this->_piece_ID; }
    [[nodiscard]] Color get_color() const { return this->_color; }


    // serializable interface ??????????????????????????????????????
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;
    static Piece* from_json(const rapidjson::Value& json);

    void position(std::vector<std::vector<bool>> &possible_moves, unsigned int init_row, unsigned int init_col, int row_offset, int col_offset);

    // checks for legal moves
    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col);

    std::vector<std::vector<bool>> bishop_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
    std::vector<std::vector<bool>> knight_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
    std::vector<std::vector<bool>> rook_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
    std::vector<std::vector<bool>> pawn_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
    std::vector<std::vector<bool>> king_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
    std::vector<std::vector<bool>> queen_moves(unsigned int init_row, unsigned int init_col, std::vector<std::vector<bool>>& possible_moves);
};

#endif //PIECE_H
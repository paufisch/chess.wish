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
    pawn
};

class piece : public unique_serializable {
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

    explicit piece(base_class_properties); // base constructor
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
    piece(std::string id);
    //deserialization constructor
    piece(std::string piece_ID, Color color, PieceType type);

    ~piece();


    [[nodiscard]] PieceType get_type() const { return this->_type; }
    [[nodiscard]] std::string get_piece_ID() const { return this->_piece_ID; }
    [[nodiscard]] Color get_color() const { return this->_color; }



    // checks for legal moves
    virtual std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) = 0;

    // serializable interface ??????????????????????????????????????
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static piece* from_json(const rapidjson::Value& json);
};

#endif //PIECE_H

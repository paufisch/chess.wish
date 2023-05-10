//
// Created by marcel on 04.05.23.
//

#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "vector"
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"


class piece : public unique_serializable {
public:
    enum PieceType {
        rook,
        king,
        queen,
        bishop,
        knight,
        pawn
    };

    //from_diff constructor
    piece(std::string id);
    //deserialization constructor

    static piece* create_piece(PieceType type,
                               std::string id,
                               serializable_value<std::string>* piece_ID,
                               serializable_value<std::string>* color);

    // checks for legal moves
    virtual std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) = 0;

private:
    piece(std::string id,
          serializable_value<std::string>* piece_ID,
          serializable_value<std::string>* color,
          serializable_value<PieceType> type);

    serializable_value<std::string>* piece_ID;
    serializable_value<std::string>* color;
    serializable_value<PieceType>* type;

    // serializable interface ??????????????????????????????????????
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static piece* from_json(const rapidjson::Value& json);
};

#endif //PIECE_H

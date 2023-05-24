//
// Created by marcel on 24.05.23.
//

#ifndef LAMA_EMPTY_H
#define LAMA_EMPTY_H



#include "../piece.h"

class Empty : public Piece {
public:

    Empty(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col, board* _board) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Piece* from_json(const rapidjson::Value& json);

};



#endif //LAMA_EMPTY_H
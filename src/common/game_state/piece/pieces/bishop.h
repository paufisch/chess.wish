//
// Created by marcel on 10.05.23.
//

#ifndef BISHOP_H
#define BISHOP_H


#include "../piece.h"

class Bishop : public Piece {
private:

    Bishop(base_class_properties);

public:

    Bishop(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Piece* from_json(const rapidjson::Value& json);
};



#endif //BISHOP_H

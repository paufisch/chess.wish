//
// Created by marcel on 10.05.23.
//

#ifndef KING_H
#define KING_H

#include "../../piece.h"


class King : public Piece{
private:

    King(base_class_properties);

public:

    //virtual ~king();

    King(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col);

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Piece * from_json(const rapidjson::Value& json);
};


#endif //KING_H

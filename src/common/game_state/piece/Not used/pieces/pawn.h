//
// Created by marcel on 10.05.23.
//

#ifndef PAWN_H
#define PAWN_H


#include "../../Piece.h"


class Pawn : public Piece {
private:

    Pawn(base_class_properties);


public:

    Pawn(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col);

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Piece * from_json(const rapidjson::Value& json);

};



#endif //PAWN_H

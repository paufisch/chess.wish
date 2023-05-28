//
// Created by marcel on 10.05.23.
//

#ifndef KNIGHT_H
#define KNIGHT_H


#include "../../Piece.h"



class Knight : public Piece {
private:

    Knight(base_class_properties);
    void position(std::vector<std::vector<bool>>& possible_moves, unsigned init_row, unsigned init_col, int row_offset, int col_offset);

public:

    Knight(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col);

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Piece * from_json(const rapidjson::Value& json);

};



#endif //KNIGHT_H

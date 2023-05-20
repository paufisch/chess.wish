//
// Created by marcel on 10.05.23.
//

#ifndef QUEEN_H
#define QUEEN_H


#include "../piece.h"
//#include "../piece.cpp"

class Queen : public piece {
private:

    Queen(base_class_properties);
    void bishop_capabilities(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col);
    void rook_capabilities(std::vector<std::vector<bool>>& possible_moves, unsigned int init_row, unsigned int init_col);


public:

    Queen(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static piece* from_json(const rapidjson::Value& json);
};



#endif //QUEEN_H

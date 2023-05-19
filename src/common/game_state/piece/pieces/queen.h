//
// Created by marcel on 10.05.23.
//

#ifndef QUEEN_H
#define QUEEN_H


#include "../piece.h"

class Queen : public piece{
private:

    Queen(base_class_properties);

public:

    Queen(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Queen* from_json(const rapidjson::Value& json);
};



#endif //QUEEN_H

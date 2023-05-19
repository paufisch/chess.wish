//
// Created by marcel on 10.05.23.
//

#ifndef KNIGHT_H
#define KNIGHT_H


#include "../piece.h"
#include <string>
#include "../../../../../rapidjson/include/rapidjson/document.h"
#include "../../board.h"

class Knight : public piece{
private:

    Knight(base_class_properties);

public:

    Knight(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Knight* from_json(const rapidjson::Value& json);

};



#endif //KNIGHT_H

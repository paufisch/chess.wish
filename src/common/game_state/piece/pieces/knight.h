//
// Created by marcel on 10.05.23.
//

#ifndef KNIGHT_H
#define KNIGHT_H


#include "../piece.h"
#include <string>
#include "../../../../../rapidjson/include/rapidjson/document.h"
#include "../../board.h"

class knight : public piece{
private:

    knight(base_class_properties);

public:

    knight(std::string piece_ID, Color color, PieceType type);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static knight* from_json(const rapidjson::Value& json);

};



#endif //KNIGHT_H

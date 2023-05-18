//
// Created by marcel on 10.05.23.
//

#ifndef BISHOP_H
#define BISHOP_H


#include "../piece.h"
#include <string>
#include "../../../../../rapidjson/include/rapidjson/document.h"
#include "../../board.h"

class bishop : public piece{
private:

    bishop(base_class_properties);
    bishop(std::string piece_ID, Color color, PieceType type);

public:

    std::vector<std::vector<bool>> legal_moves(unsigned init_row, unsigned init_col) override;

    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static bishop* from_json(const rapidjson::Value& json);
};



#endif //BISHOP_H

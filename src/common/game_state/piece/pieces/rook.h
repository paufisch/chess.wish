//
// Created by marcel on 10.05.23.
//

#ifndef ROOK_H
#define ROOK_H


#include "../piece.h"
#include <string>
#include "../../../../../rapidjson/include/rapidjson/document.h"

class rook : public piece{
private:
    std::vector<std::vector<bool>> _legal_moves;
    Color _color;
    std::string _piece_ID;

public:
    rook(std::string id, std::string pieceId, Color color1, PieceType type, std::string piece_ID, Color color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static rook* from_json(const rapidjson::Value& json);
};



#endif //ROOK_H

//
// Created by marcel on 10.05.23.
//

#ifndef PAWN_H
#define PAWN_H


#include "../piece.h"

class pawn : public piece{
public:
    pawn(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};



#endif //PAWN_H

//
// Created by marcel on 10.05.23.
//

#ifndef ROOK_H
#define ROOK_H


#include "../piece.h"

class rook : public piece{
public:
    rook(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};



#endif //ROOK_H

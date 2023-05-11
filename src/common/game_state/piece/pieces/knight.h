//
// Created by marcel on 10.05.23.
//

#ifndef KNIGHT_H
#define KNIGHT_H


#include "../piece.h"

class knight : public piece{
public:
    knight(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};



#endif //KNIGHT_H

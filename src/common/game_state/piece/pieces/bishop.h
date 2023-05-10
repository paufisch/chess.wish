//
// Created by marcel on 10.05.23.
//

#ifndef BISHOP_H
#define BISHOP_H


#include "../piece.h"

class bishop : public piece{
public:
    bishop(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};



#endif //BISHOP_H

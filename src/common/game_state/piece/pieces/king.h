//
// Created by marcel on 10.05.23.
//

#ifndef KING_H
#define KING_H

#include "../piece.h"

class king : public piece{
public:
    king(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};


#endif //KING_H

//
// Created by marcel on 10.05.23.
//

#ifndef QUEEN_H
#define QUEEN_H


#include "../piece.h"

class Queen : public piece{
public:
    Queen(std::string id, serializable_value<std::string>* piece_ID, serializable_value<std::string>* color);

    std::vector<std::vector<bool>> legal_moves(unsigned row, unsigned col) override;
};



#endif //QUEEN_H

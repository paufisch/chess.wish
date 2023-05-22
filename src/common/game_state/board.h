//
// Created by Fabian 18.05.2023
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "../../rapidjson/include/rapidjson/document.h"
#include "player/player.h"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"
#include "piece/piece.h"
#include "piece/pieces/queen.h"
#include "piece/pieces/king.h"
#include "piece/pieces/pawn.h"
#include "piece/pieces/knight.h"
#include "piece/pieces/bishop.h"
#include "piece/pieces/rook.h"



class board {
private:

    std::vector<std::vector<Piece*>> _board_layout;

    // from_diff constructor
    //board(std::string id);

    // deserialization constructor
    board(std::string id, std::vector<std::vector<Piece*>>& board_layout);

public:
    static std::vector<std::vector<Piece*>> vector_to_board(const std::vector<Piece*> vector) ;

    std::vector<piece*> board_to_vector(std::vector<std::vector<piece*>> board) const;


    board();
    ~board();

// accessors
    piece* get_piece(int i, int j);

    void set_piece(int i, int j, piece* piece);

// fill_board (0,0) = a1, (8,0) = a8
    void fill_white_king();
    void fill_black_king();

    void fill_white_pawns();
    void fill_black_pawns();

    void fill_white_rooks();
    void fill_black_rooks();

    void fill_white_knights();
    void fill_black_knights();

    void fill_white_queen();
    void fill_black_queen();

    void fill_white_bishops();
    void fill_black_bishops();

// serializable interface
    static board* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;

};


#endif //BOARD_H

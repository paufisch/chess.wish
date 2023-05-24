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




class board {
private:

    std::vector<std::vector<class Piece*>> _board_layout;

    // from_diff constructor
    //board(std::string id);

    // deserialization constructor
    board(std::vector<std::vector<class Piece*>>& board_layout);

public:
    static std::vector<std::vector<Piece*>> vector_to_board(std::vector<Piece*> vector) ;

    std::vector<Piece*> board_to_vector(std::vector<std::vector<Piece*>> board) const;


    board();
    ~board();

// accessors
    Piece* get_piece(int i, int j);

    void set_piece(int i, int j, Piece* piece);
    void set_board_layout(std::vector<std::vector<Piece*>> board);

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

    void fill_all();

// serializable interface
    static board* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;

};


#endif //BOARD_H

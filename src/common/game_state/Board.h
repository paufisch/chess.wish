#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "../../rapidjson/include/rapidjson/document.h"
#include "player/Player.h"
#include "../serialization/Serializable.h"
#include "../serialization/SerializableValue.h"
#include "../serialization/UniqueSerializable.h"
#include "piece/Piece.h"

class Board {
private:

    std::vector<std::vector<class Piece*>> _board_layout;

    // deserialization constructor
    Board(std::vector<std::vector<class Piece*>>& board_layout);

public:

    // helper functions for serialization and deserialization
    static std::vector<std::vector<Piece*>> vector_to_board(std::vector<Piece*> vector) ;
    std::vector<Piece*> board_to_vector(std::vector<std::vector<Piece*>> board) const;

    Board();
    ~Board();

    // accessor
    Piece* get_piece(int i, int j);

    // setters
    void set_piece(int i, int j, Piece* piece);
    void set_board_layout(std::vector<std::vector<Piece*>> board);

    // reset Board
    void clear_board();

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

    // Serializable interface
    static Board* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;

};


#endif //BOARD_H

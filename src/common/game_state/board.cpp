//
// Created by Fabian 18.05.2023
//

#include "board.h"

#include "../exceptions/LamaException.h"
#include "../serialization/vector_utils.h"


board::board() : unique_serializable() {
    this->_board_layout = std::vector<std::vector<piece*>> ( 8, std::vector<piece*>(8, nullptr) );
}

board::board(std::string id, std::vector<std::vector<piece *>>& board_layout )
        : unique_serializable(id)
{ }

board::board(std::string id) : unique_serializable(id) {
    this->_board_layout = std::vector<std::vector<piece*>> ( 8, std::vector<piece*>(8, nullptr) );
}

board::~board() {
// delete each piece pointer
// for loop
// fill nullptr
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            //piece* piece = _board_layout[row][col];
            //delete piece;
            //it wants to call the specific destructor not only the base class destructor
            //delete _board_layout[row][col];
            _board_layout[row][col] = nullptr;
        }
    }
}

// accessors
piece* board::get_piece(int i, int j){
    return _board_layout[i][j];
}

// fill_board (0,0) = a1, (8,0) = a8
void board::fill_white_king(){
    _board_layout[0][4] = new king(piece_ID, white, king);
}
void board::fill_black_king(){
    _board_layout[7][4] = new king(piece_ID, black, king);
}

void board::fill_white_pawns(){
    _board_layout[1][0] = new piece(piece_ID, white, pawn);
    _board_layout[1][1] = new piece(piece_ID, white, pawn);
    _board_layout[1][2] = new piece(piece_ID, white, pawn);
    _board_layout[1][3] = new piece(piece_ID, white, pawn);
    _board_layout[1][4] = new piece(piece_ID, white, pawn);
    _board_layout[1][5] = new piece(piece_ID, white, pawn);
    _board_layout[1][6] = new piece(piece_ID, white, pawn);
    _board_layout[1][7] = new piece(piece_ID, white, pawn);
}
void board::fill_black_pawns(){
    _board_layout[6][0] = new piece(piece_ID, black, pawn);
    _board_layout[6][1] = new piece(piece_ID, black, pawn);
    _board_layout[6][2] = new piece(piece_ID, black, pawn);
    _board_layout[6][3] = new piece(piece_ID, black, pawn);
    _board_layout[6][4] = new piece(piece_ID, black, pawn);
    _board_layout[6][5] = new piece(piece_ID, black, pawn);
    _board_layout[6][6] = new piece(piece_ID, black, pawn);
    _board_layout[6][7] = new piece(piece_ID, black, pawn);
}

void board::fill_white_rooks(){
    _board_layout[0][0] = new piece("1", white, rook);
    _board_layout[0][7] = new piece(piece_ID, white, rook);
}
void board::fill_black_rooks(){
    _board_layout[7][0] = new piece(piece_ID, black, rook);
    _board_layout[7][7] = new piece(piece_ID, black, rook);
}

void board::fill_white_knights() {
    _board_layout[0][1] = new piece(piece_ID, white, knight);
    _board_layout[0][6] = new piece(piece_ID, white, knight);
}
void board::fill_black_knights() {
    _board_layout[7][1] = new piece(piece_ID, black, knight);
    _board_layout[7][6] = new piece(piece_ID, black, knight);
}

void board::fill_white_queen(){
    _board_layout[0][3] = new piece(piece_ID, white, queen);
}
void board::fill_black_queen(){
    _board_layout[7][3] = new piece(piece_ID, black, queen);
}

void board::fill_white_bishops(){
    _board_layout[0][2] = new piece(piece_ID, white, bishop);
    _board_layout[0][5] = new piece(piece_ID, white, bishop);
}
void board::fill_black_bishops(){
    _board_layout[7][2] = new piece(piece_ID, black, bishop);
    _board_layout[7][5] = new piece(piece_ID, black, bishop);
}


//json
std::vector<std::vector<piece*>> board::vector_to_board(const std::vector<piece*> vector) {
    std::vector<std::vector<piece*>> board (8, std::vector<piece*>(8, nullptr));
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            board[row][col] = vector[row*8 + col];
        }
    }
    return board;
}

std::vector<piece*> board::board_to_vector(std::vector<std::vector<piece*>> board) const {
    std::vector<piece*> vector (64, nullptr);
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            vector[row*8 + col] = board[row][col];
        }
    }
    return vector;
}



// Serializable interface
void board::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    std::vector<piece*> vector = board_to_vector(_board_layout);
    json.AddMember("board_layout", vector_utils::serialize_vector(vector, allocator), allocator);
}


board* board::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("board_layout"))
    {
        std::vector<piece*> deserialized_board_layout;
        for (auto &serialized_board_layout : json["board_layout"].GetArray()) {

            deserialized_board_layout.push_back(piece::from_json(serialized_board_layout.GetObject()));
        }
        std::vector<std::vector<piece*>> _board_layout = vector_to_board(deserialized_board_layout);
        return new board(json["id"].GetString(), _board_layout );


    } else {
        throw LamaException("Failed to deserialize board. Required entries were missing.");
    }
}


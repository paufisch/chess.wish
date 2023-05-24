//
// Created by Fabian 18.05.2023
//

#include "board.h"

#include "../exceptions/LamaException.h"
#include "../serialization/vector_utils.h"


board::board() {
    this->_board_layout = std::vector<std::vector<Piece*>> ( 8, std::vector<Piece*>(8, nullptr) );
}

board::board(std::vector<std::vector<Piece *>>& board_layout )
        : _board_layout(board_layout)
{ }

//board::board(std::string id) :  {
//    this->_board_layout = std::vector<std::vector<piece*>> ( 8, std::vector<piece*>(8, nullptr) );
//}

board::~board() {
// delete each piece pointer
// for loop
// fill nullptr
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            //piece* piece = _board_layout[row][col];
            //delete piece;
            //it wants to call the specific destructor not only the base class destructor
            delete _board_layout[row][col];
            _board_layout[row][col] = nullptr;
        }
    }
}

// accessors
Piece* board::get_piece(int i, int j){
    return _board_layout[i][j];
}

void board::set_piece(int i, int j, Piece* piece) {
    _board_layout[i][j] = piece;
}

void board::set_board_layout(std::vector<std::vector<Piece*>> board) {
    _board_layout = board;
}

// fill_board (0,0) = a1, (8,0) = a8
void board::fill_white_king(){
    _board_layout[0][4] = new Piece("5", white, king, this);
}
void board::fill_black_king(){
    _board_layout[7][4] = new Piece("29", black, king, this);
}

void board::fill_white_pawns(){
    _board_layout[1][0] = new Piece("9", white, pawn, this);
    _board_layout[1][1] = new Piece("10", white, pawn, this);
    _board_layout[1][2] = new Piece("11", white, pawn, this);
    _board_layout[1][3] = new Piece("12", white, pawn, this);
    _board_layout[1][4] = new Piece("13", white, pawn, this);
    _board_layout[1][5] = new Piece("14", white, pawn, this);
    _board_layout[1][6] = new Piece("15", white, pawn, this);
    _board_layout[1][7] = new Piece("16", white, pawn, this);
}
void board::fill_black_pawns(){
    _board_layout[6][0] = new Piece("17", black, pawn, this);
    _board_layout[6][1] = new Piece("18", black, pawn, this);
    _board_layout[6][2] = new Piece("19", black, pawn, this);
    _board_layout[6][3] = new Piece("20", black, pawn, this);
    _board_layout[6][4] = new Piece("21", black, pawn, this);
    _board_layout[6][5] = new Piece("22", black, pawn, this);
    _board_layout[6][6] = new Piece("23", black, pawn, this);
    _board_layout[6][7] = new Piece("24", black, pawn, this);
}

void board::fill_white_rooks(){
    _board_layout[0][0] = new Piece("1", white, rook, this);
    _board_layout[0][7] = new Piece("8", white, rook, this);
}
void board::fill_black_rooks(){
    _board_layout[7][0] = new Piece("25", black, rook, this);
    _board_layout[7][7] = new Piece("32", black, rook, this);
}

void board::fill_white_knights() {
    _board_layout[0][1] = new Piece("2", white, knight, this);
    _board_layout[0][6] = new Piece("7", white, knight, this);
}
void board::fill_black_knights() {
    _board_layout[7][1] = new Piece("26", black, knight, this);
    _board_layout[7][6] = new Piece("31", black, knight, this);
}

void board::fill_white_queen(){
    _board_layout[0][3] = new Piece("4", white, queen, this);
}
void board::fill_black_queen(){
    _board_layout[7][3] = new Piece("28", black, queen, this);
}

void board::fill_white_bishops(){
    _board_layout[0][2] = new Piece("3", white, bishop, this);
    _board_layout[0][5] = new Piece("6", white, bishop, this);
}
void board::fill_black_bishops(){
    _board_layout[7][2] = new Piece("27", black, bishop, this);
    _board_layout[7][5] = new Piece("30", black, bishop, this);
}

void board::fill_all() {
    fill_white_king();
    fill_black_king();

    fill_white_pawns();
    fill_black_pawns();

    fill_white_rooks();
    fill_black_rooks();

    fill_white_knights();
    fill_black_knights();

    fill_white_queen();
    fill_black_queen();

    fill_white_bishops();
    fill_black_bishops();
}


//json
std::vector<std::vector<Piece*>> board::vector_to_board(std::vector<Piece*> vector) {
    std::vector<std::vector<Piece*>> board (8, std::vector<Piece*>(8, nullptr));
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if (vector[row*8 + col]->get_type() == empty) {
                delete vector[row*8 + col];
                vector[row*8 + col] = nullptr;
                board[row][col] = nullptr;
            } else {
                board[row][col] = vector[row*8 + col];
            }
        }
    }
    return board;
}

std::vector<Piece*> board::board_to_vector(std::vector<std::vector<Piece*>> board) const {
    std::vector<Piece*> vector (64, nullptr);
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if (board[row][col] == nullptr) {
                vector[row*8 + col] = new Piece("empty", white, empty, nullptr);
            } else {
                vector[row*8 + col] = board[row][col];
            }
        }
    }
    return vector;
}



// Serializable interface
void board::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    std::vector<Piece*> vector = board_to_vector(_board_layout);
    json.AddMember("board_layout", vector_utils::serialize_vector(vector, allocator), allocator);
}


board* board::from_json(const rapidjson::Value &json) {

    if (json.HasMember("board_layout"))
    {
        std::vector<Piece*> deserialized_board_layout;

        board* _board = new board();

        for (auto &serialized_board_layout : json["board_layout"].GetArray()) {

            deserialized_board_layout.push_back(Piece::from_json(serialized_board_layout.GetObject(), _board));
        }
        std::vector<std::vector<Piece*>> _board_layout = vector_to_board(deserialized_board_layout);
        _board->set_board_layout(_board_layout);
        return _board;


    } else {
        throw LamaException("Failed to deserialize board. Required entries were missing.");
    }
}


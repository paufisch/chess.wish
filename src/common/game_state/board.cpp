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
            delete _board_layout[row][col];
            _board_layout[row][col] = nullptr;
        }
    }
}

// accessors
piece* board::get_piece(int i, int j){
    return _board_layout[i][j];
}

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

    json.AddMember("board_layout", vector_utils::serialize_vector(board_to_vector(_board_layout), allocator), allocator);
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


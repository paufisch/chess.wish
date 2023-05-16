//
// Created by Manuel on 27.01.2021.
//

#include "board.h"

#include "../exceptions/LamaException.h"
#include "../serialization/vector_utils.h"


board::board() : unique_serializable() {
    this->_board_layout() = new std::vector<std::vector<piece*>> ( 8, std::vector<piece*>(8, nullptr) );
}

board::board(std::string id, std::vector<std::vector<piece *>> &board_layout (8, std::vector<piece*>(8, nullptr)))
        : unique_serializable(id)
{ }

board::board(std::string id) : unique_serializable(id) {
    this->_board_layout() = new std::vector<std::vector<piece*>> ( 8, std::vector<piece*>(8, nullptr) );
}

board::~board() {
// delete piece for loop
// fill nullptr
}

// accessors
piece* board::get_piece(int i, int j){
    return _board_layout[i][j];
}

#ifdef LAMA_SERVER

#endif


// Serializable interface
void board::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);

    json.AddMember("board_layout", vector_utils::serialize_vector(_board_layout, allocator), allocator);
}


board* board::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("board_layout"))
    {
        std::vector<std::vector<piece*>> deserialized_board_layout(8, std::vector<piece*>(8, nullptr));
        for (auto &serialized_board_layout : json["board_layout"].GetArray()) {
            deserialized_board_layout.push_back(player::from_json(serialized_board_layout.GetObject()));
        }
        return new board(json["id"].GetString(), deserialized_board_layout);


    } else {
        throw LamaException("Failed to deserialize board. Required entries were missing.");
    }
}


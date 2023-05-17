//
// Created by Manuel on 27.01.2021.
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

class board : public unique_serializable {
private:
    std::vector<std::vector<piece*>> _board_layout;

    // from_diff constructor
    board(std::string id);

    // deserialization constructor
    board(std::string id, std::vector<std::vector<piece*>>& board_layout);
public:
    static std::vector<std::vector<piece*>> vector_to_board(const std::vector<piece*> vector) ;

    std::vector<piece*> board_to_vector(std::vector<std::vector<piece*>> board) const;


    board();
    ~board();

// accessors
    piece* get_piece(int i, int j);

// serializable interface
    static board* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};


#endif //BOARD_H

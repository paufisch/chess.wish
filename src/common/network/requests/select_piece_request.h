//
// Created by jann on 5/3/23.
//

#ifndef CHESS_SELECT_PIECE_REQUEST_H
#define CHESS_SELECT_PIECE_REQUEST_H

#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class select_piece_request : public client_request {

private:

    int _coordinate_1;
    int _coordinate_2;

    //Private constructor for deserialization
    select_piece_request(base_class_properties, int coordinate_1, int coordinate_2);

public:
    [[nodiscard]] int get_coordinate_1() const { return this->_coordinate_1; }
    [[nodiscard]] int get_coordinate_2() const { return this->_coordinate_2; }

    select_piece_request(std::string game_id, std::string player_id, int coordinate_1, int coordinate_2);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static select_piece_request* from_json(const rapidjson::Value& json);
};

#endif //CHESS_SELECT_PIECE_REQUEST_H

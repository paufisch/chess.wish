//
// Created by jann on 5/3/23.
//

#ifndef LAMA_MOVE_PIECE_REQUEST_H
#define LAMA_MOVE_PIECE_REQUEST_H

#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class move_piece_request : public client_request {

private:

    int _coordinate_from_1;
    int _coordinate_from_2;
    int _coordinate_to_1;
    int _coordinate_to_2;

    //Private constructor for deserialization
    move_piece_request(base_class_properties, int coordinate_form_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2);

public:
    [[nodiscard]] int get_coordinate_from_1() const { return this->_coordinate_from_1; }
    [[nodiscard]] int get_coordinate_from_2() const { return this->_coordinate_from_2; }
    [[nodiscard]] int get_coordinate_to_1() const { return this->_coordinate_to_1; }
    [[nodiscard]] int get_coordinate_to_2() const { return this->_coordinate_to_2; }

    move_piece_request(std::string game_id, std::string player_id, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static move_piece_request* from_json(const rapidjson::Value& json);
};

#endif //LAMA_MOVE_PIECE_REQUEST_H

#ifndef CHESS_MOVEPIECEREQUEST_H
#define CHESS_MOVEPIECEREQUEST_H

#include "ClientRequest.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class MovePieceRequest : public ClientRequest {

private:

    int _coordinate_from_1;
    int _coordinate_from_2;
    int _coordinate_to_1;
    int _coordinate_to_2;

    //Private constructor for deserialization
    MovePieceRequest(base_class_properties, int coordinate_form_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2);

public:
    [[nodiscard]] int get_coordinate_from_1() const { return this->_coordinate_from_1; }
    [[nodiscard]] int get_coordinate_from_2() const { return this->_coordinate_from_2; }
    [[nodiscard]] int get_coordinate_to_1() const { return this->_coordinate_to_1; }
    [[nodiscard]] int get_coordinate_to_2() const { return this->_coordinate_to_2; }

    MovePieceRequest(std::string game_id, std::string player_id, int coordinate_from_1, int coordinate_from_2, int coordinate_to_1, int coordinate_to_2);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static MovePieceRequest* from_json(const rapidjson::Value& json);
};

#endif //CHESS_MOVEPIECEREQUEST_H

//
// Created by jann on 5/18/23.
//

#ifndef LAMA_SELECT_PIECE_RESPONSE_H
#define LAMA_SELECT_PIECE_RESPONSE_H

#include <string>
#include <vector>
#include "server_response.h"


class select_piece_response : public server_response {
private:
    bool _success;
    std::vector<std::vector<bool>> _legal_moves;

    select_piece_response(base_class_properties props, bool success, std::vector<std::vector<bool>> legal_moves);

    static std::vector<std::vector<bool>> string_to_matrix (std::string);
    std::string matrix_to_string (std::vector<std::vector<bool>>) const;

public:

    select_piece_response(std::string game_id, std::vector<std::vector<bool>> legal_moves);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static select_piece_response* from_json(const rapidjson::Value& json);
    virtual void Process() const override;
};

#endif //LAMA_SELECT_PIECE_RESPONSE_H

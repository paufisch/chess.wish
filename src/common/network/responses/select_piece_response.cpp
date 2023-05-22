//
// Created by jann on 5/18/23.
//

#include "select_piece_response.h"
#include "../../serialization/vector_utils.h"
#include "../../exceptions/LamaException.h"


select_piece_response::select_piece_response(server_response::base_class_properties props, bool success, std::vector<std::vector<bool>> legal_moves) :
    server_response(props),
    _success(success),
    _legal_moves(legal_moves)
{ }

std::vector<std::vector<bool>> select_piece_response::string_to_matrix (const std::string& string) {
    std::vector<std::vector<bool>> matrix (8, std::vector<bool>(8, false));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (string.at(8 * i + j) == '1') {
                matrix.at(i).at(j) = true;
            }
        }
    }
    return matrix;
}

std::string select_piece_response::matrix_to_string (const std::vector<std::vector<bool>>& matrix) const {
    std::string string = "0000000000000000000000000000000000000000000000000000000000000000";
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (matrix.at(i).at(j)) {
                string.at(8 * i + j) = '1';
            }
        }
    }
    return string;
}

select_piece_response::select_piece_response(std::string game_id, std::vector<std::vector<bool>> legal_moves) :
    server_response(server_response::create_base_class_properties(ResponseType::select_piece_msg, game_id)),
    _legal_moves(legal_moves)
{ }

void select_piece_response::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {

    server_response::write_into_json(json, allocator);

    json.AddMember("legal_moves", rapidjson::Value(matrix_to_string(_legal_moves), allocator), allocator);
}

select_piece_response *select_piece_response::from_json(const rapidjson::Value &json) {
    if (json.HasMember("success")) {

        return new select_piece_response(
                server_response::extract_base_class_properties(json),
                json["success"].GetBool(),
                string_to_matrix(json["legal_moves"].GetString()));
    } else {
        throw LamaException("Could not parse request_response from json. success is missing.");
    }
}
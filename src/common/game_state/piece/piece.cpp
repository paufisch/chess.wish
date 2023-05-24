//
// Created by marcel on 04.05.23.
//

#include "piece.h"
#include "../../exceptions/LamaException.h"

// for deserialization
const std::unordered_map<std::string, PieceType> Piece::_string_to_piece_type = {
        {"rook", PieceType::rook},
        {"knight", PieceType::knight},
        {"bishop", PieceType::bishop},
        {"king", PieceType::king},
        {"queen", PieceType::queen},
        {"pawn", PieceType::pawn},
        {"empty", PieceType::empty}
};


const std::unordered_map<std::string, Color> Piece::_string_to_color = {
        {"white", Color::white},
        {"black", Color::black},
};


// for serialization
const std::unordered_map<PieceType, std::string> Piece::_piece_type_to_string = {
        { PieceType::rook, "rook" },
        { PieceType::knight, "knight"},
        { PieceType::bishop, "bishop"},
        { PieceType::king, "king"},
        { PieceType::queen, "queen"},
        { PieceType::pawn, "pawn"},
        { PieceType::empty, "empty"}
};


const std::unordered_map<Color, std::string> Piece::_color_to_string = {
        { Color::white, "white" },
        { Color::black, "black"},

};

Piece::Piece(std::string piece_ID, Color color, PieceType type)
        : _piece_ID(piece_ID), _color(color), _type(type)
{ }

void Piece::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value type_val(_piece_type_to_string.at(get_type()).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value piece_ID_val(_piece_ID.c_str(), allocator);
    json.AddMember("piece_ID", piece_ID_val, allocator);

    rapidjson::Value color_val(_color_to_string.at(get_color()).c_str(), allocator);
    json.AddMember("color", piece_ID_val, allocator);
}


Piece* Piece::from_json(const rapidjson::Value& json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string piece_ID = json["piece_ID"].GetString();

        const std::string color_type = json["color"].GetString();
        const Color color = Piece::_string_to_color.at(color_type);

        const std::string type = json["type"].GetString();
        const PieceType piece_type = Piece::_string_to_piece_type.at(type);

        if (piece_type == PieceType::rook) {
            return Rook::from_json(json);
        }
        else if (piece_type == PieceType::knight) {
            return Knight::from_json(json);
        }
        else if (piece_type == PieceType::bishop) {
            return Bishop::from_json(json);
        }
        else if (piece_type == PieceType::king) {
            return King::from_json(json);
        }
        else if (piece_type == PieceType::queen) {
            return Queen::from_json(json);
        }
        else if (piece_type == PieceType::pawn) {
            return Pawn::from_json(json);
        }
        else if (piece_type == PieceType::empty) {
            return Empty::from_json(json);
        } else {
            throw LamaException("Encountered unknown Piece type " + type);
        }
    }
    throw LamaException("Could not determine type of Piece. JSON was:\n" + json_utils::to_string(&json));
}










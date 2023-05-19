//
// Created by marcel on 04.05.23.
//

#include "piece.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/bishop.h"
#include <iostream>
#include "../../exceptions/LamaException.h"


// for deserialization
const std::unordered_map<std::string, PieceType> piece::_string_to_piece_type = {
        {"rook", PieceType::rook},
        {"knight", PieceType::knight},
        {"bishop", PieceType::bishop},
        {"king", PieceType::king},
        {"queen", PieceType::queen},
        {"pawn", PieceType::pawn}
};


const std::unordered_map<std::string, Color> piece::_string_to_color = {
        {"white", Color::white},
        {"black", Color::black},
};


// for serialization
const std::unordered_map<PieceType, std::string> piece::_piece_type_to_string = {
        { PieceType::rook, "rook" },
        { PieceType::knight, "knight"},
        { PieceType::bishop, "bishop"},
        { PieceType::king, "king"},
        { PieceType::queen, "queen"},
        { PieceType::pawn, "pawn"}
};


const std::unordered_map<Color, std::string> piece::_color_to_string = {
        { Color::white, "white" },
        { Color::black, "black"},

};


// used by subclasses to retrieve information from the json stored by this superclass
piece::base_class_properties piece::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("piece_ID") && json.HasMember("color")) {
        std::string piece_ID = json["piece_ID"].GetString();
        Color color = _string_to_color.at(json["color"].GetString());

        return create_base_class_properties(
                piece::_string_to_piece_type.at(json["type"].GetString()),
                piece_ID,
                color
        );
    }
    else
    {
        throw LamaException("Client Request did not contain piece_ID or color");
    }
}


piece::base_class_properties piece::create_base_class_properties(
        PieceType type,
        std::string piece_ID,
        Color color)
{
    piece::base_class_properties res;
    res._piece_ID = piece_ID;
    res._color = color;
    res._type = type;
    return res;
}

piece::~piece() {
    _board = nullptr;
}

piece::piece(std::string id) : unique_serializable(id) { }


piece::piece(std::string piece_ID, Color color, PieceType type)
        : _piece_ID(piece_ID), _color(color), _type(type)
{ }


// protected constructor. only used by subclasses
piece::piece(piece::base_class_properties props) :
        _type(props._type),
        _piece_ID(props._piece_ID),
        _color(props._color),
        _board(props._board)
{}

/*
std::vector<std::vector<bool>> piece::legal_moves(unsigned row, unsigned col) {
    // Default implementation, to be overridden by subclasses
    return std::vector<std::vector<bool>> legalMoves(8, std::vector<bool>(8, false));
}
*/


void piece::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {

    rapidjson::Value type_val(_piece_type_to_string.at(get_type()).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value piece_ID_val(_piece_ID.c_str(), allocator);
    json.AddMember("piece_ID", piece_ID_val, allocator);

    rapidjson::Value color_val(_color_to_string.at(get_color()).c_str(), allocator);
    json.AddMember("color", piece_ID_val, allocator);
}


piece* piece::from_json(const rapidjson::Value& json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type = json["type"].GetString();
        const PieceType piece_type = piece::_string_to_piece_type.at(type);

        if (piece_type == PieceType::rook) {
            return rook::from_json(json);
        }
        else if (piece_type == PieceType::knight) {
            return knight::from_json(json);
        }
        else if (piece_type == PieceType::bishop) {
            return bishop::from_json(json);
        }
        else if (piece_type == PieceType::king) {
            return king::from_json(json);
        }
        else if (piece_type == PieceType::queen) {
            return queen::from_json(json);
        }
        else if (piece_type == PieceType::pawn) {
            return pawn::from_json(json);
        } else {
            throw LamaException("Encountered unknown Piece type " + type);
        }
    }
    throw LamaException("Could not determine type of Piece. JSON was:\n" + json_utils::to_string(&json));
}














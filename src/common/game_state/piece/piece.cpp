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



//Maybe it has to be changed
#include "../../exceptions/LamaException.h"


piece::piece(std::string id) : unique_serializable(id) { }

piece::piece(std::string id,
             serializable_value<std::string>* piece_ID,
             serializable_value<std::string>* color,
             serializable_value<PieceType>* pieceType)

        : unique_serializable(id), piece_ID(piece_ID), color(color), type(pieceType)
{ }

std::vector<std::vector<bool>> piece::legal_moves(unsigned row, unsigned col) {
    // Default implementation, to be overridden by subclasses
    std::vector<std::vector<bool>> legalMoves(8, std::vector<bool>(8, false));
    return legalMoves;
}




//void piece::write_into_json(rapidjson::Value &json,
//                            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
//    unique_serializable::write_into_json(json, allocator);
//}

//piece *piece::from_json(const rapidjson::Value &json) {
//    if (json.HasMember("id") && json.HasMember("value")) {
//        return new piece(json["id"].GetString(), serializable_value<int>::from_json(json["value"].GetObject()));
//    } else {
//        throw LamaException("Could not parse json of card. Was missing 'id' or 'val'.");
//    }
//}

void piece::write_into_json(rapidjson::Value &json,
                            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    //unique_serializable::write_into_json(json, allocator);

    json.SetObject();
    json.AddMember("id", rapidjson::Value(get_id().c_str(), allocator).Move(), allocater);
    json.AddMember("type", rapidjson::Value(get_piece_type_name().c_str(), allocator).Move(), allocator);
    json.AddMember("color", rapidjson::Value(get_color().c_str(), allocator).Move(), allocator);
}

piece* piece::from_json(const rapidjson::Value& json) {
    std::string id = json["id"].GetString();
    std::string type = json["type"].GetString();
    std::string color = json["color"].GetString();

    PieceType pieceType = get_piece_type_from_name(type);

    switch(pieceType) {
        case PieceType::rook:
            return new rook(id, piece_ID, color);
        case PieceType::king:
            return new king(id, color);
        case PieceType::queen:
            return new queen(id, color);
        case PieceType::bishop:
            return new bishop(id, color);
        case PieceType::knight:
            return new knight(id, color);
        case PieceType::pawn:
            return new pawn(id, color);
    }

    return nullptr; // Should never happen
}

std::string piece::get_piece_type_name() const {
    switch(pieceType) {
        case PieceType::rook:comment
            return "rook";
        case PieceType::king:
            return "king";
        case PieceType::queen:
            return "queen";
        case PieceType::bishop:
            return "bishop";
        case PieceType::knight:
            return "knight";
        case PieceType::pawn:
            return "pawn";
    }
}

PieceType piece::get_piece_type_from_name(const std::string& name) {
    if(name == "rook") {
        return PieceType::rook;
    } else if(name == "king") {
        return PieceType::king;
    } else if(name == "queen") {
        return PieceType::queen;
    } else if(name == "bishop") {
        return PieceType::bishop;
    } else if(name == "knight") {
        return PieceType::knight;
    } else if(name == "pawn") {
        return PieceType::pawn;
    } else {
        // Should never happen
        throw std::invalid_argument("Unknown piece type: " + name);
    }
}










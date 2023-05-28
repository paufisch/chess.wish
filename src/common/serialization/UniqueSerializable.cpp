//
// Created by Manuel on 19.02.2021.
//
// Used to serialize GameState objects that need to be identifiable by a unique id.

#include "UniqueSerializable.h"

#include "UuidGenerator.h"
#include "../exceptions/ChessException.h"


UniqueSerializable::UniqueSerializable()
    : _id(UuidGenerator::generate_uuid_v4())
{ }

UniqueSerializable::UniqueSerializable(std::string id)
    : _id(id)
{ }

std::string UniqueSerializable::get_id() const {
    return this->_id;
}

void UniqueSerializable::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

}

//
// Created by Manuel on 03.02.2021.
//
// Used to serialize GameState objects that need to be identifiable by a unique id.

#ifndef CHESS_UNIQUESERIALIZABLE_H
#define CHESS_UNIQUESERIALIZABLE_H


#include "Serializable.h"

class UniqueSerializable : public Serializable {
protected:

    std::string _id;       // unique identifier

    UniqueSerializable();
    UniqueSerializable(std::string id);

public:
// accessors
    std::string get_id() const;

// Serializable interface
    virtual void write_into_json(rapidjson::Value& json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

};

#endif //CHESS_UNIQUESERIALIZABLE_H

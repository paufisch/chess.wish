//
// Created by Manuel on 04.02.2021.
//
// A Serializable value. Supported value types are:
//  bool
//  int
//  unsigned int
//  int64_t
//  uint64_t
//  float
//  double
//  string

#ifndef CHESS_SERIALIZABLEVALUE_H
#define CHESS_SERIALIZABLEVALUE_H


#include <vector>
#include <iostream>
#include <functional>

#include "UniqueSerializable.h"
#include "value_type_helpers.h"
#include "../../../rapidjson/include/rapidjson/document.h"

template <class T>
class SerializableValue : public Serializable {

private:
    T _value;


public:

    SerializableValue(T val) : Serializable(), _value(val) { }

    T get_value() const { return this->_value; }

    void set_value(T val) {
        if (this->_value != val) {
            this->_value = val;
        }
    }

// Serializable interface
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override {
        json.AddMember("value", value_type_helpers::get_json_value<decltype(_value)>(_value, allocator), allocator);
    }

    static SerializableValue<T>* from_json(const rapidjson::Value& json) {
        if (json.HasMember("value")) {
            T val = json["value"].Get<T>();
            return new SerializableValue<T>(val);
        }
        return nullptr;
    }
};



#endif //CHESS_SERIALIZABLEVALUE_H

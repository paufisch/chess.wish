// Base class for all messages sent from the server to the client.
// It offers a function to deserialize a ServerResponse subclass from a valid json.

#ifndef CHESS_SERVERRESPONSE_H
#define CHESS_SERVERRESPONSE_H

#include <string>
#include <unordered_map>

#include "../../serialization/Serializable.h"

// Identifier for the different response types.
// The ResponseType is sent with every ServerResponse to identify the type of ServerResponse
// during deserialization on the client side.
enum ResponseType {
    req_response,
    full_state_msg,
};

class ServerResponse : public Serializable {
private:

    // for deserialization
    static const std::unordered_map<std::string, ResponseType> _string_to_response_type;
    // for serialization
    static const std::unordered_map<ResponseType, std::string> _response_type_to_string;

protected:
    std::string _game_id;
    ResponseType _type;

    struct base_class_properties {
        std::string game_id;
        ResponseType type;
    };

    explicit ServerResponse(base_class_properties); // base constructor
    static base_class_properties create_base_class_properties(ResponseType type, const std::string& game_id);
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

public:
    ResponseType get_type() const;
    std::string get_game_id() const;

    // Tries to create the specific ServerResponse from the provided json.
    // Throws exception if parsing fails -> Use only inside "try{ }catch()" block
    static ServerResponse* from_json(const rapidjson::Value& json);

    // Serializes the ServerResponse into a json object that can be sent over the network
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

#ifdef CHESS_CLIENT
    virtual void Process() const = 0;
#endif
};


#endif //CHESS_SERVERRESPONSE_H

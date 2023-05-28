// ClientRequest is the base-class for all requests that are being sent from the client to the server.
// It offers a function to deserialize a ClientRequest subclass from a valid json.

#ifndef CHESS_CLIENTREQUEST_H
#define CHESS_CLIENTREQUEST_H

#include <string>
#include <unordered_map>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/Serializable.h"
#include "../../exceptions/ChessException.h"
#include "../../serialization/UuidGenerator.h"
#include "../../serialization/JsonUtils.h"



// Identifier for the different request types.
// The RequestType is sent with every ClientRequest to identify the type of ClientRequest
// during deserialization on the server side.
enum RequestType {
    join_game,
    start_game,
    move_piece,
    resign,
};

class ClientRequest : public Serializable {
protected:

    struct base_class_properties {
        RequestType _type;
        std::string _req_id;
        std::string _player_id;
        std::string _game_id;
    };

    RequestType _type;
    std::string _req_id;
    std::string _player_id;
    std::string _game_id;

    explicit ClientRequest(base_class_properties); // base constructor
    static base_class_properties create_base_class_properties(RequestType type, std::string req_id, std::string& player_id, std::string& game_id);
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

private:

    // for deserialization
    static const std::unordered_map<std::string, RequestType> _string_to_request_type;
    // for serialization
    static const std::unordered_map<RequestType, std::string> _request_type_to_string;

public:
    virtual ~ClientRequest() {}

    [[nodiscard]] RequestType get_type() const { return this->_type; }
    [[nodiscard]] std::string get_req_id() const { return this->_req_id; }
    [[nodiscard]] std::string get_game_id() const { return this->_game_id; }
    [[nodiscard]] std::string get_player_id() const { return this->_player_id; }

    // Tries to create the specific ClientRequest from the provided json.
    // Throws exception if parsing fails -> Use only in "try{ }catch()" block
    static ClientRequest* from_json(const rapidjson::Value& json);

    // Serializes the ClientRequest into a json object that can be sent over the network
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

    [[nodiscard]] virtual std::string to_string() const;
};


#endif //CHESS_CLIENTREQUEST_H

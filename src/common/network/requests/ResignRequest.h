#ifndef CHESS_RESIGNREQUEST_H
#define CHESS_RESIGNREQUEST_H

#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class ResignRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit ResignRequest(base_class_properties);

public:
    ResignRequest(std::string game_id, std::string player_id);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static ResignRequest* from_json(const rapidjson::Value& json);
};

#endif //CHESS_RESIGNREQUEST_H

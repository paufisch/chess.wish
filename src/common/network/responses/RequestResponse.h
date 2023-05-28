#ifndef CHESS_REQUESTRESPONSE_H
#define CHESS_REQUESTRESPONSE_H

#include <string>
#include "ServerResponse.h"

#include "../../serialization/JsonUtils.h"
#include "../../exceptions/ChessException.h"
#include "../../game_state/GameState.h"

#ifdef CHESS_CLIENT
#include "../../../client/GameController.h"
#endif

class RequestResponse : public ServerResponse {
private:
    bool _success;
    std::string _err;
    std::string _req_id;
    rapidjson::Value* _state_json = nullptr;

    RequestResponse(base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string& err);

public:

    RequestResponse(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err);
    ~RequestResponse();

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static RequestResponse* from_json(const rapidjson::Value& json);

#ifdef CHESS_CLIENT
    virtual void Process() const override;
#endif
};


#endif //CHESS_REQUESTRESPONSE_H

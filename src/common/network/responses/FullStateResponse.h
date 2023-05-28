#ifndef CHESS_FULLSTATERESPONSE_H
#define CHESS_FULLSTATERESPONSE_H

#include "ServerResponse.h"
#include "../../game_state/GameState.h"

#include "../../exceptions/ChessException.h"

#ifdef CHESS_CLIENT
#include "../../../client/GameController.h"
#endif

class FullStateResponse : public ServerResponse {
private:
    rapidjson::Value* _state_json;

    /*
     * Private constructor for deserialization
     */
    FullStateResponse(base_class_properties props, rapidjson::Value* state_json);

public:

    FullStateResponse(std::string game_id, const GameState& state);
    ~FullStateResponse();

    rapidjson::Value* get_state_json() const;

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static FullStateResponse* from_json(const rapidjson::Value& json);

#ifdef CHESS_CLIENT
    virtual void Process() const override;
#endif
};


#endif //CHESS_FULLSTATERESPONSE_H

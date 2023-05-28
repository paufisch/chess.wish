#include "RequestResponse.h"

RequestResponse::RequestResponse(ServerResponse::base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string &err) :
        ServerResponse(props),
        _req_id(req_id),
        _state_json(state_json),
        _success(success),
        _err(err)
{ }

RequestResponse::RequestResponse(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err):
        ServerResponse(ServerResponse::create_base_class_properties(ResponseType::req_response, game_id)),
        _req_id(req_id),
        _state_json(state_json),
        _success(success),
        _err(err)
{ }

RequestResponse::~RequestResponse() {
    if (_state_json != nullptr) {
        delete _state_json;
        _state_json = nullptr;
    }
}

void RequestResponse::write_into_json(rapidjson::Value &json,
                                      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ServerResponse::write_into_json(json, allocator);

    rapidjson::Value err_val(_err.c_str(), allocator);
    json.AddMember("err", err_val, allocator);

    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);

    json.AddMember("success", _success, allocator);

    if (_state_json != nullptr) {
        json.AddMember("state_json", *_state_json, allocator);
    }
}

RequestResponse *RequestResponse::from_json(const rapidjson::Value& json) {
    if (json.HasMember("err") && json.HasMember("success")) {
        std::string err = json["err"].GetString();

        rapidjson::Value* state_json = nullptr;
        if (json.HasMember("state_json")) {
            state_json = JsonUtils::clone_value(json["state_json"].GetObject());
        }
        return new RequestResponse(
                ServerResponse::extract_base_class_properties(json),
                json["req_id"].GetString(),
                json["success"].GetBool(),
                state_json,
                err);
    } else {
        throw ChessException("Could not parse RequestResponse from json. err or success is missing.");
    }
}

#ifdef CHESS_CLIENT

void RequestResponse::Process() const {
    if (_success) {
        if (this->_state_json != nullptr) {
            GameState* state = GameState::from_json(*_state_json);
            GameController::updateGameState(state);

        } else {
            GameController::showError("Network error", "Expected a state as JSON inside the RequestResponse. But there was none.");
        }
    } else {
        GameController::showError("Not possible", _err);
    }

}

#endif

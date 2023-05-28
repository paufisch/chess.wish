#include "FullStateResponse.h"

FullStateResponse::FullStateResponse(ServerResponse::base_class_properties props, rapidjson::Value* state_json) :
        ServerResponse(props),
        _state_json(state_json)
{ }

FullStateResponse::FullStateResponse(std::string game_id, const GameState& state) :
        ServerResponse(ServerResponse::create_base_class_properties(ResponseType::full_state_msg, game_id))
{
    this->_state_json = state.to_json();
}

void FullStateResponse::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ServerResponse::write_into_json(json, allocator);
    json.AddMember("state_json", *_state_json, allocator);
}

FullStateResponse *FullStateResponse::from_json(const rapidjson::Value& json) {
    if (json.HasMember("state_json")) {
        return new FullStateResponse(ServerResponse::extract_base_class_properties(json),
                                     JsonUtils::clone_value(json["state_json"].GetObject()));
    } else {
        throw ChessException("Could not parse FullStateResponse from json. state is missing.");
    }
}

FullStateResponse::~FullStateResponse() {
    if (_state_json != nullptr) {
        delete _state_json;
        _state_json = nullptr;
    }
}

rapidjson::Value* FullStateResponse::get_state_json() const {
    return _state_json;
}

#ifdef CHESS_CLIENT

void FullStateResponse::Process() const {
    try {
        GameState* state = GameState::from_json(*_state_json);
        GameController::updateGameState(state);

    } catch(std::exception& e) {
        std::cerr << "Failed to extract GameState from FullStateResponse" << std::endl
                  << e.what() << std::endl;
    }
}

#endif

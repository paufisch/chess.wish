#include "Player.h"

#include "../../exceptions/ChessException.h"

const std::unordered_map<std::string, Color> Player::_string_to_color = {
        {"white", Color::white},
        {"black", Color::black},
};

const std::unordered_map<Color, std::string> Player::_color_to_string = {
        { Color::white, "white" },
        { Color::black, "black"},
};

Player::Player(std::string name) : UniqueSerializable() {
    this->_player_name = new SerializableValue<std::string>(name);
    if (name == "white"){
        this->_color = white;
    } else {
        this->_color = black;
    }
}

Player::Player(std::string name, Color color) : UniqueSerializable() {
    this->_player_name = new SerializableValue<std::string>(name);
    this->_color = color;
}

Player::Player(std::string id, SerializableValue<std::string>* name, Color color) :
        UniqueSerializable(id),
        _player_name(name),
        _color(color)
{ }

Player::Player(std::string id, SerializableValue<std::string>* name, std::string color) :
        UniqueSerializable(id),
        _player_name(name)
{
    _color = (color == "white" ? white : black);
}

Player::~Player() {
    if (_player_name != nullptr) {
        delete _player_name;

        _player_name = nullptr;
    }
}

#ifdef CHESS_SERVER
Player::Player(std::string id, std::string name, Color color) :
        UniqueSerializable(id)
{
    this->_player_name = new SerializableValue<std::string>(name);
    this->_color = color;
}

std::string Player::get_game_id() {
    return _game_id;
}

void Player::set_game_id(std::string game_id) {
    _game_id = game_id;
}
#endif

std::string Player::get_player_name() const noexcept {
    return this->_player_name->get_value();
}

bool Player::get_color() const noexcept {
    return this->_color;
}

void Player::set_color(Color color) {
    _color = color;
}

void Player::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    UniqueSerializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value name_val(rapidjson::kObjectType);
    _player_name->write_into_json(name_val, allocator);
    json.AddMember("player_name", name_val, allocator);

    //rapidjson::Value color_val(rapidjson::kObjectType);
    json.AddMember("color", rapidjson::Value(_color_to_string.at(_color), allocator), allocator);
}


Player *Player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("player_name")
        && json.HasMember("color"))
    {
        return new Player(
                json["id"].GetString(),
                SerializableValue<std::string>::from_json(json["player_name"].GetObject()),
                _string_to_color.at(json["color"].GetString()));
    } else {
        throw ChessException("Failed to deserialize Player from json. Required json entries were missing.");
    }
}
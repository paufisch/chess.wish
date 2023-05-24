//
// Created by Manuel on 25.01.2021.
//

#include "player.h"

#include "../../exceptions/LamaException.h"

const std::unordered_map<std::string, Color> player::_string_to_color = {
        {"white", Color::white},
        {"black", Color::black},
};

const std::unordered_map<Color, std::string> player::_color_to_string = {
        { Color::white, "white" },
        { Color::black, "black"},
};

player::player(std::string name) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    if (name == "white"){
        this->_color = white;
    } else {
        this->_color = black;
    }

}

player::player(std::string name, Color color) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    this->_color = color;
}

player::player(std::string id, serializable_value<std::string>* name, Color color) :
        unique_serializable(id),
        _player_name(name),
        _color(color)
{ }




player::player(std::string id, serializable_value<std::string>* name, std::string color) :
        unique_serializable(id),
        _player_name(name)
{
    _color = (color == "white" ? white : black);
}

player::~player() {
    if (_player_name != nullptr) {
        delete _player_name;

        _player_name = nullptr;
    }
}

#ifdef LAMA_SERVER
player::player(std::string id, std::string name, Color color) :
        unique_serializable(id)
{
    this->_player_name = new serializable_value<std::string>(name);
    this->_color = color;
}

std::string player::get_game_id() {
    return _game_id;
}

void player::set_game_id(std::string game_id) {
    _game_id = game_id;
}
#endif

std::string player::get_player_name() const noexcept {
    return this->_player_name->get_value();
}

bool player::get_color() const noexcept {
    return this->_color;
}

void player::set_color(Color color) {
    _color = color;
}


#ifdef LAMA_SERVER
void player::setup_round(std::string& err) {
    //_color = ?
}

void player::wrap_up_round(std::string &err) {
    // ?????
}

//bool player::fold(std::string &err) {
//    if (has_folded()) {
//        err = "This player has already folded.";
//        return false;
//    }
//    _has_folded->set_value(true);
//   return true;
//}

#endif


void player::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value name_val(rapidjson::kObjectType);
    _player_name->write_into_json(name_val, allocator);
    json.AddMember("player_name", name_val, allocator);

    //rapidjson::Value color_val(rapidjson::kObjectType);
    json.AddMember("color", rapidjson::Value(_color_to_string.at(_color), allocator), allocator);
}


player *player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("player_name")
        && json.HasMember("color"))
    {
        return new player(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["player_name"].GetObject()),
                _string_to_color.at(json["color"].GetString()));
    } else {
        throw LamaException("Failed to deserialize player from json. Required json entries were missing.");
    }
}

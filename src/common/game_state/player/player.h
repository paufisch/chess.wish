//
// Created by Manuel on 25.01.2021.
//

#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H


#include <string>
#include "../color.h"
#include "../../serialization/uuid_generator.h"
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"

class player : public unique_serializable {
private:
    serializable_value<std::string>* _player_name;
    Color _color;

#ifdef CHESS_SERVER
    std::string _game_id;
#endif

    /*
     * Deserialization constructor
     */

    player(std::string id, serializable_value<std::string>* name, Color color);

    player(std::string id, serializable_value<std::string>* name, std::string color);

    static const std::unordered_map<std::string, Color> _string_to_color;
    static const std::unordered_map<Color, std::string> _color_to_string;


public:
// constructors
    explicit player(std::string name, Color color);   // for client
    player(std::string name);
    ~player();

#ifdef CHESS_SERVER
    player(std::string id, std::string name, Color color);  // for server

    std::string get_game_id();
    void set_game_id(std::string game_id);
#endif

    // accessors
    std::string get_player_name() const noexcept;
    bool get_color() const noexcept;
    void set_color(Color color);

#ifdef CHESS_SERVER
    // state update functions
    //bool fold(std::string& err);

    void wrap_up_round(std::string& err);
    void setup_round(std::string& err);
#endif


    // serialization
    static player* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};


#endif //CHESS_PLAYER_H

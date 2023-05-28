#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H

#include <string>
#include "../Color.h"
#include "../../serialization/UuidGenerator.h"
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/UniqueSerializable.h"
#include "../../serialization/SerializableValue.h"

class Player : public UniqueSerializable {
private:
    SerializableValue<std::string>* _player_name;
    Color _color;

#ifdef CHESS_SERVER
    std::string _game_id;
#endif

    Player(std::string id, SerializableValue<std::string>* name, Color color);

    Player(std::string id, SerializableValue<std::string>* name, std::string color);

    static const std::unordered_map<std::string, Color> _string_to_color;
    static const std::unordered_map<Color, std::string> _color_to_string;

public:
// constructors
    explicit Player(std::string name, Color color);   // for client
    Player(std::string name);
    ~Player();

#ifdef CHESS_SERVER
    Player(std::string id, std::string name, Color color);  // for server

    std::string get_game_id();
    void set_game_id(std::string game_id);
#endif

    // accessors
    std::string get_player_name() const noexcept;
    bool get_color() const noexcept;
    void set_color(Color color);




    // serialization
    static Player* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};


#endif //CHESS_PLAYER_H

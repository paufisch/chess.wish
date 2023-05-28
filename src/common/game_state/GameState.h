#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

#include <vector>
#include <string>
#include "../../rapidjson/include/rapidjson/document.h"
#include "player/Player.h"
#include "../serialization/Serializable.h"
#include "../serialization/SerializableValue.h"
#include "../serialization/UniqueSerializable.h"
#include "Board.h"


class GameState : public UniqueSerializable {
private:

    static const int _max_nof_players = 2;
    static const int _min_nof_players = 2;

    std::vector<Player*> _players;
    Board* _board;
    Player* _loser;
    SerializableValue<bool>* _is_started;
    SerializableValue<bool>* _is_finished;
    SerializableValue<bool>* _is_resigned;
    SerializableValue<int>* _round_number;
    SerializableValue<int>* _current_player_idx;
    SerializableValue<int>* _starting_player_idx;
    int _max_number_rounds = 1000;

    // from_diff constructor
    GameState(std::string id);

    // returns the index of 'Player' in the '_players' vector
    int get_player_index(Player* player) const;
public:

    // deserialization constructor
    GameState(
            std::string id,
            std::vector<Player*>& players,
            Board *board,
            Player *loser,
            SerializableValue<bool>* is_started,
            SerializableValue<bool>* is_finished,
            SerializableValue<bool>* is_resigned,
            SerializableValue<int>* current_player_idx,
            SerializableValue<int>* round_number,
            SerializableValue<int>* starting_player_idx);

    GameState();
    ~GameState();

    // accessors
    bool is_full() const;
    bool is_started() const;
    bool is_finished() const;
    bool is_resigned() const;
    bool is_allowed_to_play_now(Player* player) const;
    std::vector<Player*>& get_players();
    int get_round_number() const;
    Board* get_board();
    Player* get_current_player() const;
    Player* get_loser();
    int get_max_number_rounds();

    std::vector<std::vector<bool>> select_piece(int i, int j);

    // state update functions
    bool move_piece(int i_from, int j_from, int i_to, int j_to);
    Player* resign(Player* loser);
    void next_turn();
    void set_is_finished(bool finished);
    void set_loser(Player* loser);

#ifdef CHESS_SERVER
// server-side state update functions
    void setup_board();   // server side initialization
    bool remove_player(Player* Player, std::string& err);
    bool add_player(Player* Player, std::string& err);
    bool start_game(std::string& err);

    // end of round functions
    void update_current_player(std::string& err);

#endif

    // Serializable interface
    static GameState* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};


#endif //CHESS_GAMESTATE_H


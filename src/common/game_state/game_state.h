#ifndef CHESS_GAME_STATE_H
#define CHESS_GAME_STATE_H

#include <vector>
#include <string>
#include "../../rapidjson/include/rapidjson/document.h"
#include "player/player.h"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"
#include "board.h"


class game_state : public unique_serializable {
private:

    static const int _max_nof_players = 2;
    static const int _min_nof_players = 2;

    std::vector<player*> _players;
    board* _board;
    player* _loser;
    serializable_value<bool>* _is_started;
    serializable_value<bool>* _is_finished;
    serializable_value<bool>* _is_resigned;
    serializable_value<int>* _round_number;
    serializable_value<int>* _current_player_idx;
    serializable_value<int>* _starting_player_idx;
    int _max_number_rounds = 1000;

    // from_diff constructor
    game_state(std::string id);

    // returns the index of 'player' in the '_players' vector
    int get_player_index(player* player) const;
public:

    // deserialization constructor
    game_state(
            std::string id,
            std::vector<player*>& players,
            board *board,
            player *loser,
            serializable_value<bool>* is_started,
            serializable_value<bool>* is_finished,
            serializable_value<bool>* is_resigned,
            serializable_value<int>* current_player_idx,
            serializable_value<int>* round_number,
            serializable_value<int>* starting_player_idx);

    game_state();
    ~game_state();

    // accessors
    bool is_full() const;
    bool is_started() const;
    bool is_finished() const;
    bool is_resigned() const;
    bool is_allowed_to_play_now(player* player) const;
    std::vector<player*>& get_players();
    int get_round_number() const;
    board* get_board();
    player* get_current_player() const;
    player* get_loser();
    int get_max_number_rounds();

    std::vector<std::vector<bool>> select_piece(int i, int j);

    // state update functions
    bool move_piece(int i_from, int j_from, int i_to, int j_to);
    player* resign(player* loser);
    void next_turn();
    void set_is_finished(bool finished);
    void set_loser(player* loser);

#ifdef CHESS_SERVER
// server-side state update functions
    void setup_board();   // server side initialization
    bool remove_player(player* player, std::string& err);
    bool add_player(player* player, std::string& err);
    bool start_game(std::string& err);

    // end of round functions
    void update_current_player(std::string& err);

#endif

    // serializable interface
    static game_state* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};


#endif //CHESS_GAME_STATE_H


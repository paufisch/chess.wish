//
// Created by Fabian 18.05.2023
//

#ifndef LAMA_GAME_STATE_H
#define LAMA_GAME_STATE_H


#include "player/player.h"
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
    serializable_value<int>* _round_number;
    serializable_value<int>* _current_player_idx;
    serializable_value<int>* _starting_player_idx;

    // from_diff constructor
    game_state(std::string id);

    // deserialization constructor
    game_state(
            std::string id,
            std::vector<player*>& players,
            board *board,
            player *loser,
            serializable_value<bool>* is_started,
            serializable_value<bool>* is_finished,
            serializable_value<int>* current_player_idx,
            serializable_value<int>* round_number,
            serializable_value<int>* starting_player_idx);

    // returns the index of 'player' in the '_players' vector
    int get_player_index(player* player) const;
public:
    game_state();
    ~game_state();

// accessors
    bool is_full() const;
    bool is_started() const;
    bool is_finished() const;
    bool is_player_in_game(player* player) const;
    bool is_allowed_to_play_now(player* player) const;
    std::vector<player*>& get_players();
    int get_round_number() const;

    player* get_current_player() const;
    std::vector<std::vector<bool>> select_piece(int i, int j);
    bool move_piece(int i_from, int j_from, int i_to, int j_to);
    player* resign(player* loser);
    board* get_board();
    void next_turn();

#ifdef LAMA_SERVER
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


#endif //LAMA_GAME_STATE_H


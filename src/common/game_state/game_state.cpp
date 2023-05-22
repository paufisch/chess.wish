//
// Created by Fabian 18.05.2023
//

#include "game_state.h"
#include "piece/piece.h"
#include "board.h"
#include "piece/pieces/queen.h"

#include "piece/pieces/queen.h"

#include "../exceptions/LamaException.h"
#include "../serialization/vector_utils.h"

game_state::game_state(std::string id) : unique_serializable(id) {
    this->_players = std::vector<player*>();
    this->_board = new board;
    this->_loser = nullptr;
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_round_number = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}

game_state::game_state(std::string id,
                       std::vector<player *> &players,
                       board *board,
                       player *loser,
                       serializable_value<bool> *is_started,
                       serializable_value<bool> *is_finished,
                       serializable_value<int> *current_player_idx,
                       serializable_value<int> *round_number,
                       serializable_value<int> *starting_player_idx)
        : unique_serializable(id),

          _players(players),
          _board(board),
          _loser(loser),
          _is_started(is_started),
          _is_finished(is_finished),
          _current_player_idx(current_player_idx),
          _round_number(round_number),
          _starting_player_idx(starting_player_idx)
{ }

game_state::game_state() : unique_serializable() {
    this->_players = std::vector<player*>();
    this->_board = new board;
    this->_loser = nullptr;
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_round_number = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
}


game_state::~game_state() {
    if (_is_started != nullptr) {
        delete _is_started;
        delete _is_finished;
        delete _board;
        delete _current_player_idx;
        delete _starting_player_idx;
        delete _round_number;

        _is_started = nullptr;
        _is_finished = nullptr;
        _current_player_idx = nullptr;
        _starting_player_idx = nullptr;
        _round_number = nullptr;
        _loser = nullptr;
    }
}

// accessors
player* game_state::get_current_player() const {
    if(_current_player_idx == nullptr || _players.size() == 0) {
        return nullptr;
    }
    return _players[_current_player_idx->get_value()];
}

bool game_state::is_full() const {
    return _players.size() == _max_nof_players;
}

bool game_state::is_started() const {
    return _is_started->get_value();
}

bool game_state::is_finished() const {
    return _is_finished->get_value();
}

int game_state::get_round_number() const {
    return _round_number->get_value();
}

int game_state::get_player_index(player *player) const {
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end()) {
        return -1;
    } else {
        return it - _players.begin();
    }
}

bool game_state::is_player_in_game(player *player) const {
    return std::find(_players.begin(), _players.end(), player) < _players.end();
}

//Maybe further restrictions needed !resign !king_taken !round_limit
bool game_state::is_allowed_to_play_now(player *player) const {
    return (player == get_current_player()) && (0 == (_is_finished->get_value()));
}

std::vector<player*>& game_state::get_players() {
    return _players;
}

std::vector<std::vector<bool>> game_state::select_piece(int i, int j){
    return _board->get_piece(i,j)->legal_moves(i,j);
}

bool game_state::move_piece(int i_from, int j_from, int i_to, int j_to){
    piece* _moving_piece = _board->get_piece(i_from,j_from);
    auto _legal_moves = _moving_piece->legal_moves(i_from,j_from);
    if(_legal_moves[i_to][j_to]){

        //checks if we want to overwrite a king
        if( (_board->get_piece(i_to,j_to) != nullptr) && (king == _board->get_piece(i_to,j_to)->get_type()) ){
            _is_finished->set_value(true);
        }

        //the piece pointer at _to gets overwritten with the piece pointer at _from
        if (_board->get_piece(i_to, j_to) != nullptr) {
            delete _board->get_piece(i_to, j_to);
        }
        _board->set_piece(i_to, j_to, _moving_piece);

        //the piece pointer at _from is now empty
        _board->set_piece(i_from, j_from, nullptr);

        //checks if we want to move a pawn to the base line
        if( (i_to == 7 || i_to == 0) && (pawn == _moving_piece->get_type()) ){
            //the piece pointer at _to gets overwritten with the piece pointer at _from
            delete _board->get_piece(i_to, j_to);
            _board->set_piece(i_to, j_to, new Queen(_moving_piece->get_piece_ID(), _moving_piece->get_color(), queen));
        }

        _round_number++;

        return true;
    }
    else {
        return false;
    }
}

player* game_state::resign(player* loser){
    _loser = loser;
    _is_finished->set_value(true);
    return _loser;
}

board* game_state::get_board(){
    return _board;
}

//the round number increase is made inside move_piece
void game_state::next_turn() {
    if (_current_player_idx->get_value() == 1)
        _current_player_idx->set_value(0);
    else if (_current_player_idx->get_value() == 0)
        _current_player_idx->set_value(1);
}


#ifdef LAMA_SERVER

// state modification functions without diff
void game_state::setup_board() {
    /*

    _board->fill_white_king();
    _board->fill_black_king();

    _board->fill_white_pawns();
    _board->fill_black_pawns();

    _board->fill_white_rooks();
    _board->fill_black_rooks();

    _board->fill_white_knights();
    _board->fill_black_knights();

    _board->fill_white_queen();
    _board->fill_black_queen();

    _board->fill_white_bishops();
    _board->fill_black_bishops();

    */
}

//for securitiy reasons here, no new funtionality
void game_state::update_current_player(std::string& err) {
    next_turn();
}

bool game_state::start_game(std::string &err) {
    if (_players.size() < _min_nof_players) {
        err = "You need at least " + std::to_string(_min_nof_players) + " players to start the game.";
        return false;
    }

    if (!_is_started->get_value()) {
        this->setup_board();
        this->_is_started->set_value(true);
        return true;
    } else {
        err = "Could not start game, as the game was already started";
        return false;
    }
}

bool game_state::remove_player(player *player_ptr, std::string &err) {
    int idx = get_player_index(player_ptr);
    if (idx != -1) {
        if (idx < _current_player_idx->get_value()) {
            // reduce current_player_idx if the player who left had a lower index
            _current_player_idx->set_value(_current_player_idx->get_value() - 1);
        }
        _players.erase(_players.begin() + idx);
        return true;
    } else {
        err = "Could not leave game, as the requested player was not found in that game.";
        return false;
    }
}


bool game_state::add_player(player* player_ptr, std::string& err) {
    if (_is_started->get_value()) {
        err = "Could not join game, because the requested game is already started.";
        return false;
    }
    if (_is_finished->get_value()) {
        err = "Could not join game, because the requested game is already finished.";
        return false;
    }
    if (_players.size() >= _max_nof_players) {
        err = "Could not join game, because the max number of players is already reached.";
        return false;
    }
    if (std::find(_players.begin(), _players.end(), player_ptr) != _players.end()) {
        err = "Could not join game, because this player is already subscribed to this game.";
        return false;
    }

    _players.push_back(player_ptr);
    return true;
}

#endif


// Serializable interface
void game_state::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value is_finished_val(rapidjson::kObjectType);
    _is_finished->write_into_json(is_finished_val, allocator);
    json.AddMember("is_finished", is_finished_val, allocator);

    rapidjson::Value is_started_val(rapidjson::kObjectType);
    _is_started->write_into_json(is_started_val, allocator);
    json.AddMember("is_started", is_started_val, allocator);

    rapidjson::Value current_player_idx_val(rapidjson::kObjectType);
    _current_player_idx->write_into_json(current_player_idx_val, allocator);
    json.AddMember("current_player_idx", current_player_idx_val, allocator);

    rapidjson::Value starting_player_idx_val(rapidjson::kObjectType);
    _starting_player_idx->write_into_json(starting_player_idx_val, allocator);
    json.AddMember("starting_player_idx", starting_player_idx_val, allocator);

    rapidjson::Value round_number_val(rapidjson::kObjectType);
    _round_number->write_into_json(round_number_val, allocator);
    json.AddMember("round_number", round_number_val, allocator);

    json.AddMember("players", vector_utils::serialize_vector(_players, allocator), allocator);

    rapidjson::Value board_val(rapidjson::kObjectType);
    _board->write_into_json(board_val, allocator);
    json.AddMember("board", board_val, allocator);

    rapidjson::Value loser_val(rapidjson::kObjectType);
    _board->write_into_json(loser_val, allocator);
    json.AddMember("loser", board_val, allocator);

}


game_state* game_state::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("is_finished")
        && json.HasMember("is_started")
        && json.HasMember("current_player_idx")
        && json.HasMember("round_number")
        && json.HasMember("starting_player_idx")
        && json.HasMember("players")
        && json.HasMember("loser")
        && json.HasMember("board"))
    {
        std::vector<player*> deserialized_players;
        for (auto &serialized_player : json["players"].GetArray()) {
            deserialized_players.push_back(player::from_json(serialized_player.GetObject()));
        }

        return new game_state(json["id"].GetString(),
                              deserialized_players,
                              board::from_json(json["loser"].GetObject()),
                              player::from_json(json["board"].GetObject()),
                              serializable_value<bool>::from_json(json["is_started"].GetObject()),
                              serializable_value<bool>::from_json(json["is_finished"].GetObject()),
                              serializable_value<int>::from_json(json["current_player_idx"].GetObject()),
                              serializable_value<int>::from_json(json["round_number"].GetObject()),
                              serializable_value<int>::from_json(json["starting_player_idx"].GetObject()));


    } else {
        throw LamaException("Failed to deserialize game_state. Required entries were missing.");
    }
}


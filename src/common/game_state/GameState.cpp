#include "GameState.h"

#include "../exceptions/ChessException.h"
#include "../serialization/vector_utils.h"

GameState::GameState(std::string id) : UniqueSerializable(id) {
    this->_players = std::vector<Player*>();
    this->_board = new Board();
    this->_loser = nullptr;
    this->_is_started = new SerializableValue<bool>(false);
    this->_is_finished = new SerializableValue<bool>(false);
    this->_current_player_idx = new SerializableValue<int>(0);
    this->_round_number = new SerializableValue<int>(0);
    this->_starting_player_idx = new SerializableValue<int>(0);
}

GameState::GameState(std::string id,
                     std::vector<Player *> &players,
                     Board *board,
                     Player *loser,
                     SerializableValue<bool> *is_started,
                     SerializableValue<bool> *is_finished,
                     SerializableValue<bool> *is_resigned,
                     SerializableValue<int> *current_player_idx,
                     SerializableValue<int> *round_number,
                     SerializableValue<int> *starting_player_idx)
        : UniqueSerializable(id),

          _players(players),
          _board(board),
          _loser(loser),
          _is_started(is_started),
          _is_finished(is_finished),
          _is_resigned(is_resigned),
          _current_player_idx(current_player_idx),
          _round_number(round_number),
          _starting_player_idx(starting_player_idx)
{ }

GameState::GameState() : UniqueSerializable() {
    this->_players = std::vector<Player*>();
    this->_board = new Board();
    this->_loser = nullptr;
    this->_is_started = new SerializableValue<bool>(false);
    this->_is_finished = new SerializableValue<bool>(false);
    this->_is_resigned = new SerializableValue<bool>(false);
    this->_current_player_idx = new SerializableValue<int>(0);
    this->_round_number = new SerializableValue<int>(0);
    this->_starting_player_idx = new SerializableValue<int>(0);

    #ifdef CHESS_SERVER
        setup_board();
    #endif
}

GameState::~GameState() {
    if (_is_started != nullptr) {
        delete _is_started;
        delete _is_finished;
        delete _is_resigned;
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

Player* GameState::get_current_player() const {
    if(_current_player_idx == nullptr || _players.size() == 0) {
        return nullptr;
    }
    return _players[_current_player_idx->get_value()];
}

bool GameState::is_full() const {
    return _players.size() == _max_nof_players;
}

bool GameState::is_started() const {
    return _is_started->get_value();
}

bool GameState::is_finished() const {
    return _is_finished->get_value();
}

bool GameState::is_resigned() const {
    return _is_resigned->get_value();
}

int GameState::get_round_number() const {
    return _round_number->get_value();
}

int GameState::get_player_index(Player *player) const {
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end()) {
        return -1;
    } else {
        return it - _players.begin();
    }
}

bool GameState::is_allowed_to_play_now(Player *player) const {
    return (player == get_current_player()) && (0 == (_is_finished->get_value()));
}

std::vector<Player*>& GameState::get_players() {
    return _players;
}

std::vector<std::vector<bool>> GameState::select_piece(int i, int j){
    return _board->get_piece(i,j)->legal_moves(i,j);
}

bool GameState::move_piece(int i_from, int j_from, int i_to, int j_to){
    Piece* _moving_piece = _board->get_piece(i_from,j_from);
    auto _legal_moves = _moving_piece->legal_moves(i_from,j_from);
    if(_legal_moves[i_to][j_to]){

        //checks if we want to overwrite a king
        if( (_board->get_piece(i_to,j_to) != nullptr) && (king == _board->get_piece(i_to,j_to)->get_type()) ){
            _is_finished->set_value(true);
            int _current_player_id = _current_player_idx->get_value();
            _loser = _players[(_current_player_id +1)%2];
        }

        //the piece pointer at _to gets overwritten with the piece pointer at _from
        if (_board->get_piece(i_to, j_to) != nullptr) {
            delete _board->get_piece(i_to, j_to);
        }
        _board->set_piece(i_to, j_to, _moving_piece);

        //the piece pointer at _from is now empty
        _board->set_piece(i_from, j_from, nullptr);

        //checks if we want to move a pawn to the baseline
        if(i_to==0 && _moving_piece->get_type()==pawn && _moving_piece->get_color()==black){
            std::string id = _moving_piece->get_piece_ID();
            delete _moving_piece;
            Piece* new_queen = new Piece(id, black, queen, _board);
            _board->set_piece(i_to, j_to, new_queen);
        } else if(i_to==7 && _moving_piece->get_type()==pawn && _moving_piece->get_color()==white) {
            std::string id = _moving_piece->get_piece_ID();
            delete _moving_piece;
            Piece* new_queen = new Piece(id, white, queen, _board);
            _board->set_piece(i_to, j_to, new_queen);
        }

        if (_round_number->get_value() == _max_number_rounds) {
            _is_finished->set_value(true);
            _loser = get_players().at(rand()%2);
        }
        _round_number->set_value(_round_number->get_value() + 1);

        next_turn();

        return true;
    }
    else {
        return false;
    }
}

Player* GameState::resign(Player* loser){
    _loser = loser;
    _is_finished->set_value(true);
    _is_resigned->set_value(true);
    return _loser;
}

Board* GameState::get_board(){
    return _board;
}

//the round number increase is made inside move_piece
void GameState::next_turn() {
    if (_current_player_idx->get_value() == 1)
        _current_player_idx->set_value(0);
    else if (_current_player_idx->get_value() == 0)
        _current_player_idx->set_value(1);
}

Player* GameState::get_loser() {
    return _loser;
}

int GameState::get_max_number_rounds() {
    return _max_number_rounds;
}

void GameState::set_is_finished(bool finished) {
    _is_finished->set_value(finished);
}

void GameState::set_loser(Player* loser) {
    _loser = loser;
}

#ifdef CHESS_SERVER

// state modification functions without diff
void GameState::setup_board() {

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
}

void GameState::update_current_player(std::string& err) {
    next_turn();
}

bool GameState::start_game(std::string &err) {
    if (_players.size() < _min_nof_players) {
        err = "You need " + std::to_string(_min_nof_players) + " players to start the game.";
        return false;
    }

    if (!_is_started->get_value()) {
        setup_board();
        _is_started->set_value(true);
        return true;
    } else if (_is_finished->get_value()) {
        _board->clear_board();
        setup_board();
        _loser = nullptr;
        _is_finished->set_value(false);
        _is_resigned->set_value(false);
        _round_number->set_value(0);
        if (_players[0]->get_color() == white) {
            _current_player_idx->set_value(1);
            _starting_player_idx->set_value(1);
            _players[0]->set_color(black);
            _players[1]->set_color(white);
        } else {
            _current_player_idx->set_value(0);
            _starting_player_idx->set_value(0);
            _players[0]->set_color(white);
            _players[1]->set_color(black);
        }
        return true;
    } else if (get_round_number() == 0 || get_round_number() == 1) {
        return true;
    } else {
        err = "Could not start game, as the game was already started";
        return false;
    }
}

bool GameState::remove_player(Player *player_ptr, std::string &err) {
    int idx = get_player_index(player_ptr);
    if (idx != -1) {
        if (idx < _current_player_idx->get_value()) {
            // reduce current_player_idx if the Player who left had a lower index
            _current_player_idx->set_value(_current_player_idx->get_value() - 1);
        }
        _players.erase(_players.begin() + idx);
        return true;
    } else {
        err = "Could not leave game, as the requested Player was not found in that game.";
        return false;
    }
}

bool GameState::add_player(Player* player_ptr, std::string& err) {
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
        err = "Could not join game, because this Player is already subscribed to this game.";
        return false;
    }

    _players.push_back(player_ptr);
    return true;
}

#endif

void GameState::write_into_json(rapidjson::Value &json,
                                rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    UniqueSerializable::write_into_json(json, allocator);

    rapidjson::Value is_resigned_val(rapidjson::kObjectType);
    _is_resigned->write_into_json(is_resigned_val, allocator);
    json.AddMember("is_resigned", is_resigned_val, allocator);

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
    json.AddMember("Board", board_val, allocator);

    if (_is_finished->get_value()) {
        rapidjson::Value loser_val(rapidjson::kObjectType);
        _loser->write_into_json(loser_val, allocator);
        json.AddMember("loser", loser_val, allocator);
    }

}

GameState* GameState::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("is_finished")
        && json.HasMember("is_resigned")
        && json.HasMember("is_started")
        && json.HasMember("current_player_idx")
        && json.HasMember("round_number")
        && json.HasMember("starting_player_idx")
        && json.HasMember("players")
        && json.HasMember("Board"))
    {
        std::vector<Player*> deserialized_players;
        for (auto &serialized_player : json["players"].GetArray()) {
            deserialized_players.push_back(Player::from_json(serialized_player.GetObject()));
        }

        Player* loser;
        if (SerializableValue<bool>::from_json(json["is_finished"].GetObject()) && json.HasMember("loser")){
            loser = Player::from_json(json["loser"].GetObject());
        } else {
            loser = nullptr;
        }

        return new GameState(json["id"].GetString(),
                             deserialized_players,
                             Board::from_json(json["Board"].GetObject()),
                             loser,
                             SerializableValue<bool>::from_json(json["is_started"].GetObject()),
                             SerializableValue<bool>::from_json(json["is_finished"].GetObject()),
                             SerializableValue<bool>::from_json(json["is_resigned"].GetObject()),
                             SerializableValue<int>::from_json(json["current_player_idx"].GetObject()),
                             SerializableValue<int>::from_json(json["round_number"].GetObject()),
                             SerializableValue<int>::from_json(json["starting_player_idx"].GetObject()));


    } else {
        throw ChessException("Failed to deserialize GameState. Required entries were missing.");
    }
}


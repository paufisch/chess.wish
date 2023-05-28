//
// Created by Fabian on 24.05.2023
//

#include "gtest/gtest.h"
#include "../src/common/exceptions/ChessException.h"
#include "../src/common/game_state/piece/Piece.h"
#include "../src/common/game_state/Board.h"
#include "../src/common/game_state/GameState.h"
#include "../src/common/serialization/JsonUtils.h"
#include <vector>

/* A test fixture allows to reuse the same configuration of objects for all
 * tests in a test suite. The name of the fixture must match the test suite.
 *
 * For each test defined with TEST_F(), googletest will create a fresh test
 * fixture at runtime, immediately initialize it via SetUp(), run the test,
 * clean up by calling TearDown(), and then delete the test fixture.
 * Note that different tests in the same test suite have different test fixture
 * objects, and googletest always deletes a test fixture before it creates the
 * next one. googletest does not reuse the same test fixture for multiple
 * tests. Any changes one test makes to the fixture do not affect other tests.
 *
 * Move_Test_3 is based on Piece_Test_3 in order to have a working setup with fitting legal_moves.
 *
 */
class Move_Test_3 : public ::testing::Test {

protected:

    Board* _board;
    GameState* _game_state;

    std::vector<std::vector<bool>> _legal_moves_output;
    std::vector<std::vector<bool>> _legal_moves_expected;

    //Hint: The set_piece lines can be copy pasted into GameState::setup_board() for the display of this setup.
    virtual void SetUp() {

        _board = new Board;

        _legal_moves_output   = std::vector<std::vector<bool>> (8, std::vector<bool>(8, false));
        _legal_moves_expected = std::vector<std::vector<bool>> (8, std::vector<bool>(8, false));

        //commented out pieces are taken in this test case
        _board->set_piece(1, 5, new Piece( "5", white, king, _board));
        _board->set_piece(7, 7, new Piece("29", black, king, _board));

        _board->set_piece(3, 0, new Piece( "9", white, pawn, _board));
        _board->set_piece(1, 1, new Piece("10", white, pawn, _board));
        _board->set_piece(1, 2, new Piece("11", white, pawn, _board));
        //_board->set_piece(1, 3, new Piece("12", white, pawn, _board));
        //_board->set_piece(1, 4, new Piece("13", white, pawn, _board));
        //_board->set_piece(1, 5, new Piece("14", white, pawn, _board));
        _board->set_piece(1, 6, new Piece("15", white, pawn, _board));
        _board->set_piece(2, 7, new Piece("16", white, pawn, _board));

        _board->set_piece(4, 0, new Piece("17", black, pawn, _board));
        //_board->set_piece(6, 1, new Piece("18", black, pawn, _board));
        //_board->set_piece(6, 2, new Piece("19", black, pawn, _board));
        //_board->set_piece(6, 3, new Piece("20", black, pawn, _board));
        _board->set_piece(1, 4, new Piece("21", black, pawn, _board));
        _board->set_piece(5, 5, new Piece("22", black, pawn, _board));
        //_board->set_piece(6, 6, new Piece("23", black, pawn, _board));
        //_board->set_piece(6, 7, new Piece("24", black, pawn, _board));

        _board->set_piece(6, 3, new Piece( "1", white, rook, _board));
        //_board->set_piece(0, 7, new Piece( "8", white, rook, _board));

        //_board->set_piece(7, 0, new Piece("25", black, rook, _board));
        _board->set_piece(7, 6, new Piece("32", black, rook, _board));

        //_board->set_piece(0, 1, new Piece( "2", white, knight, _board));
        //_board->set_piece(0, 6, new Piece( "7", white, knight, _board));

        //_board->set_piece(7, 1, new Piece("26", black, knight, _board));
        _board->set_piece(4, 6, new Piece("31", black, knight, _board));

        _board->set_piece(5, 7, new Piece( "4", white, queen, _board));
        //_board->set_piece(7, 3, new Piece("28", black, queen, _board));

        //_board->set_piece(0, 2, new Piece( "3", white, bishop, _board));
        //_board->set_piece(0, 5, new Piece( "6", white, bishop, _board));

        //_board->set_piece(7, 2, new Piece("27", black, bishop, _board));
        //_board->set_piece(7, 5, new Piece("30", black, bishop, _board));

        std::string id;
        Player *player_1;
        Player *player_2;
        std::vector<Player *> players = {player_1, player_2};
        Player *loser = nullptr;
        SerializableValue<bool> *is_started              = new SerializableValue<bool>(false);
        SerializableValue<bool> *is_finished             = new SerializableValue<bool>(false);
        SerializableValue<bool> *is_resigned             = new SerializableValue<bool>(false);
        SerializableValue<int>  *current_player_idx      = new SerializableValue<int>(0);
        SerializableValue<int>  *round_number            = new SerializableValue<int>(0);
        SerializableValue<int>  *starting_player_idx     = new SerializableValue<int>(0);

        _game_state = new GameState(id,
                                    players,
                                    _board,
                                    loser,
                                    is_started,
                                    is_finished,
                                    is_resigned,
                                    current_player_idx,
                                    round_number,
                                    starting_player_idx);

    }

    virtual void TearDown() {
        delete _game_state;
    }

};

//Queen from (5,7) to (7,7)
TEST_F(Move_Test_3, Queen_Move_1){

    Piece* moving_piece = _game_state->get_board()->get_piece(5,7);
    EXPECT_EQ(_game_state->move_piece(5,7,7,7), true);
    EXPECT_EQ(_game_state->is_finished(), true);
    EXPECT_EQ(_game_state->get_loser() != nullptr, true);
    EXPECT_EQ(_game_state->get_round_number(), 1);
    EXPECT_EQ(_game_state->get_board()->get_piece(5,7) == nullptr, true);
    EXPECT_EQ(_game_state->get_board()->get_piece(7,7) == moving_piece, true);

}

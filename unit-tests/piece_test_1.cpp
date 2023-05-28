//
// Created by Fabian on 24.05.2023
//

#include "gtest/gtest.h"
#include "../src/common/exceptions/ChessException.h"
#include "../src/common/game_state/piece/Piece.h"
#include "../src/common/game_state/Board.h"
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
 */
class Piece_Test_1 : public ::testing::Test {

protected:

    Board* _board;

    std::vector<std::vector<bool>> _legal_moves_output;
    std::vector<std::vector<bool>> _legal_moves_expected;

    //This Board setup is the game start setup.
    virtual void SetUp() {

        _board = new Board;

        _legal_moves_output   = std::vector<std::vector<bool>> (8, std::vector<bool>(8, false));
        _legal_moves_expected = std::vector<std::vector<bool>> (8, std::vector<bool>(8, false));

        _board->set_piece(0, 4, new Piece( "5", white, king, _board));
        _board->set_piece(7, 4, new Piece("29", black, king, _board));

        _board->set_piece(1, 0, new Piece( "9", white, pawn, _board));
        _board->set_piece(1, 1, new Piece("10", white, pawn, _board));
        _board->set_piece(1, 2, new Piece("11", white, pawn, _board));
        _board->set_piece(1, 3, new Piece("12", white, pawn, _board));
        _board->set_piece(1, 4, new Piece("13", white, pawn, _board));
        _board->set_piece(1, 5, new Piece("14", white, pawn, _board));
        _board->set_piece(1, 6, new Piece("15", white, pawn, _board));
        _board->set_piece(1, 7, new Piece("16", white, pawn, _board));

        _board->set_piece(6, 0, new Piece("17", black, pawn, _board));
        _board->set_piece(6, 1, new Piece("18", black, pawn, _board));
        _board->set_piece(6, 2, new Piece("19", black, pawn, _board));
        _board->set_piece(6, 3, new Piece("20", black, pawn, _board));
        _board->set_piece(6, 4, new Piece("21", black, pawn, _board));
        _board->set_piece(6, 5, new Piece("22", black, pawn, _board));
        _board->set_piece(6, 6, new Piece("23", black, pawn, _board));
        _board->set_piece(6, 7, new Piece("24", black, pawn, _board));

        _board->set_piece(0, 0, new Piece( "1", white, rook, _board));
        _board->set_piece(0, 7, new Piece( "8", white, rook, _board));

        _board->set_piece(7, 0, new Piece("25", black, rook, _board));
        _board->set_piece(7, 7, new Piece("32", black, rook, _board));

        _board->set_piece(0, 1, new Piece( "2", white, knight, _board));
        _board->set_piece(0, 6, new Piece( "7", white, knight, _board));

        _board->set_piece(7, 1, new Piece("26", black, knight, _board));
        _board->set_piece(7, 6, new Piece("31", black, knight, _board));

        _board->set_piece(0, 3, new Piece( "4", white, queen, _board));
        _board->set_piece(7, 3, new Piece("28", black, queen, _board));

        _board->set_piece(0, 2, new Piece( "3", white, bishop, _board));
        _board->set_piece(0, 5, new Piece( "6", white, bishop, _board));

        _board->set_piece(7, 2, new Piece("27", black, bishop, _board));
        _board->set_piece(7, 5, new Piece("30", black, bishop, _board));
    }

    virtual void TearDown() {
        delete _board;
    }

    bool compare(std::vector<std::vector<bool>>& legal_moves_output,
                 std::vector<std::vector<bool>>& legal_moves_expected){

        for(int i = 0; i < 8; i++ ){
            for(int j = 0; j < 8; j++ ){
                if(legal_moves_output[i][j] != legal_moves_expected[i][j]){
                    return false;
                }
            }
        }

        return true;
    }


};


TEST_F(Piece_Test_1, Pawn) {
    _legal_moves_expected[2][4] = true;
    _legal_moves_expected[3][4] = true;
    _legal_moves_output = _board->get_piece(1,4)->legal_moves(1,4);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);

}

TEST_F(Piece_Test_1, Rook) {
    _legal_moves_output = _board->get_piece(0,0)->legal_moves(0,0);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);
}

TEST_F(Piece_Test_1, King) {
    _legal_moves_output = _board->get_piece(7,4)->legal_moves(7,4);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);
}

TEST_F(Piece_Test_1, Queen) {
    _legal_moves_output = _board->get_piece(0,3)->legal_moves(0,3);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);
}

TEST_F(Piece_Test_1, Bishop) {
    _legal_moves_output = _board->get_piece(7,5)->legal_moves(7,5);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);
}

TEST_F(Piece_Test_1, Knight) {
    _legal_moves_expected[5][7] = true;
    _legal_moves_expected[5][5] = true;
    _legal_moves_output = _board->get_piece(7,6)->legal_moves(7,6);

    ASSERT_EQ(compare(_legal_moves_output, _legal_moves_expected), true);
}
//
// Created by Fabian on 24.05.2023
//

//Currently commented out in the main CMakeList.txt inside src

#include "gtest/gtest.h"
#include "../src/common/exceptions/LamaException.h"
#include "../src/common/game_state/piece/piece.h"
#include "../src/common/game_state/board.h"
#include "../src/common/serialization/json_utils.h"

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
class Piece_Test : public ::testing::Test {

protected:
    virtual void SetUp() {
        cards.resize(8);
        for (int i = 1; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                cards[i].push_back(new card(i));
            }
        }
    }

    /* Any object and subroutine declared here can be accessed in the tests */

    // cards[i][j] holds a pointer to the j-th copy of a card of value i
    std::vector<std::vector<card*>> cards;
    hand player_hand;
    std::string err;
};

TEST_F(Piece_Test, Pawn) {

}


#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../GameController.h"


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
    selected = nullptr;
    weiss = wxColor(255, 255, 255);
    green = wxColor(175, 245, 190);
    orange = wxColor(255, 180, 100);
    light_orange = wxColor(255,195,133);

    //black pieces
    b_pawn = "../assets/black-pawn.png";
    b_king = "../assets/black-king.png";
    b_queen = "../assets/black-queen.png";
    b_rook = "../assets/black-rook.png";
    b_knight = "../assets/black-knight.png";
    b_bishop = "../assets/black-bishop.png";

    //white pieces
    w_pawn = "../assets/white-pawn.png";
    w_king = "../assets/white-king.png";
    w_queen = "../assets/white-queen.png";
    w_rook = "../assets/white-rook.png";
    w_knight= "../assets/white-knight.png";
    w_bishop = "../assets/white-bishop.png";
}


void MainGamePanel::buildGameState(GameState* gameState, Player* me) {

    // remove any existing UI
    this->DestroyChildren();

    //get all players
    std::vector<Player*> players = gameState->get_players();

    // find our own Player object in the list of players
    int myPosition = -1;
    auto it = std::find_if(players.begin(), players.end(), [me](const Player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
        myPosition = it - players.begin();
    } else {
        GameController::showError("Game state error", "Could not find this Player among players of server game.");
        return;
    }

    //get other Player
    Player* otherPlayer = players.at((myPosition + 1) % 2);

    // show our own Player
    this->buildThisPlayer(gameState, me, otherPlayer);

    // update layout
    this->Layout();
}


void MainGamePanel::buildThisPlayer(GameState* gameState, Player* me, Player* otherPlayer) {

    // Setup two nested box sizer structure
    auto* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    auto* boardLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(boardLayout, 0, wxEXPAND);
    auto innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 0, wxEXPAND);
    auto* otherInformation = new wxBoxSizer(wxVERTICAL);
    innerLayout->Add(otherInformation, 1, wxEXPAND | wxTOP, 5);
    auto *place_holder = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(50,200));
    innerLayout->Add(place_holder, 3, wxEXPAND);
    auto* myInformation = new wxBoxSizer(wxVERTICAL);
    innerLayout->Add(myInformation, 1, wxEXPAND | wxBOTTOM, 5);

    //build Board
    MainGamePanel::board = MainGamePanel::buildBoard(gameState, me);//this is actually the grid sizer
    boardLayout->Add(MainGamePanel::board, 0, wxALIGN_CENTER);

    //if it's our turn
    if (gameState->get_current_player() == me) {

        //start the game
        if(!gameState->is_started()) {
            GameController::startGame();
        }

        //add the "resign" button
        auto *resignButton = new wxButton(this, wxID_ANY, "Resign", wxDefaultPosition, wxSize(200, 18));
        resignButton->SetForegroundColour(wxColor(0,0,0));

        resignButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
            GameController::resign();
        });
        myInformation->Add(resignButton, 1, wxEXPAND);
        // Show our name
        wxStaticText *playerName = buildStaticText(me->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        myInformation->Add(playerName, 1, wxEXPAND);
        //display other players name
        wxStaticText *other_name = buildStaticText(otherPlayer->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        otherInformation->Add(other_name, 1, wxEXPAND);
        //display that other Player is waiting
        std::string statusText = "waiting...";
        wxStaticText *status_txt = buildStaticText(statusText,wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
        otherInformation->Add(status_txt, 1, wxEXPAND);

    // if it's not our turn
    } else {
        //display waiting
        wxStaticText *playerStatus = buildStaticText("waiting...",wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
        myInformation->Add(playerStatus, 1, wxEXPAND);
        // show our name
        wxStaticText* playerName = buildStaticText(me->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        myInformation->Add(playerName, 1, wxEXPAND);
        // Show other Player name
        wxStaticText *other_name = buildStaticText(otherPlayer->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        otherInformation->Add(other_name, 1, wxEXPAND);
        // Show other Player's status label
        wxStaticText *status_txt = buildStaticText("their turn",wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
        otherInformation->Add(status_txt, 1, wxEXPAND);
    }
}


wxGridSizer* MainGamePanel::buildBoard(GameState* gameState, Player* me) {
    /*
     * builds the chess Board from gameState
     */

    //the Board is a grid sizer containing 64 panels
    auto *grid = new wxGridSizer(8, 8, 0, 0);

    // fill the panels with the pieces contained in Board
    color_board();

    //add pieces to the Board
    add_pieces(gameState, me);

    //add functionality to the pieces
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            //add functionality to the panel
            panels[i * 8 + j]->Bind(wxEVT_LEFT_DOWN, [=](wxMouseEvent &event) {
                OnPanelClick(i, j, gameState, me);
            });

            grid->Add(panels[i * 8 + j], 1, wxEXPAND | wxALL, 0);
        }
    }

    grid->SetMinSize(wxSize(800, 800));
    return grid;
}


void MainGamePanel::OnPanelClick(int i, int j, GameState *gameState, Player *me) {
    /*
     * Adds functionality to the panels such as:
            -selecting a piece
            - displaying valid moves
            -moving a piece
     */

    //if it's our turn we can move pieces
    if (gameState->get_current_player() == me) {

        //if no panel is selected, select one!
        if (MainGamePanel::selected == nullptr) {

            //get the piece from the selected panel
            Piece *_piece = nullptr;
            std::vector<std::vector<bool>> possible_moves;
            if (me->get_color() == white && gameState->get_board()->get_piece(i, j) != nullptr) {
                _piece = gameState->get_board()->get_piece(i, j);
                possible_moves = _piece->legal_moves(i, j);
            } else if (me->get_color() == black && gameState->get_board()->get_piece(7 - i, 7 - j) != nullptr) {
                _piece = gameState->get_board()->get_piece(7 - i, 7 - j);
                possible_moves = _piece->legal_moves(7 - i, 7 - j);
            }

            //if the panel contains a piece
            if (_piece != nullptr) {

                //sum the total number of possible moves
                int sum = 0;
                for (const auto &row: possible_moves) {
                    for (bool value: row) {
                        sum += value;
                    }
                }

                //if the selected piece is our piece and has possible moves we select it
                if (_piece->get_color() == me->get_color() && sum > 0) {
                    MainGamePanel::selected = new unsigned int[2];
                    MainGamePanel::selected[0] = i;
                    MainGamePanel::selected[1] = j;
                    //display valid moves
                    display_moves(possible_moves, me);
                } else {
                    GameController::showError("Error", "Select a different piece!");
                }

            //if the panel doesn't contain a piece we display an error
            } else {
                GameController::showError("Error", "No piece here!");
            }

        //if we click the selected piece again we deselect it
        } else if (i == MainGamePanel::selected[0] && j == MainGamePanel::selected[1]) {
            delete[] MainGamePanel::selected;
            MainGamePanel::selected = nullptr;
            //remove the highlighted valid moves
            deselect_moves();

        //if a piece already has been selected we check if we can move the piece
        } else {

            //position of the previously selected piece
            unsigned int from_i = MainGamePanel::selected[0];
            unsigned int from_j = MainGamePanel::selected[1];

            //check if selected destination is a possible move of the selected piece
            std::vector<std::vector<bool>> possible_moves;
            if (me->get_color() == white) {
                possible_moves = gameState->select_piece(from_i, from_j);
                if (possible_moves[i][j] == true) {
                    GameController::movePiece(from_i, from_j, i, j);
                    //deselect piece
                    delete[] MainGamePanel::selected;
                    MainGamePanel::selected = nullptr;
                } else {
                    GameController::showError("Error", "Not a valid move!");
                }
            } else {
                //check if selected destination is a possible move
                possible_moves = gameState->select_piece(7 - from_i, 7 - from_j);
                if (possible_moves[7 - i][7 - j] == true) {
                    GameController::movePiece(7 - from_i, 7 - from_j, 7 - i, 7 - j);
                    //deselect piece
                    delete[] MainGamePanel::selected;
                    MainGamePanel::selected = nullptr;
                } else {
                    GameController::showError("Error", "Not a valid move!");
                }
            }
        }
    // if it's not our turn we display an error message
    } else {
        GameController::showError("Error", "It's not your turn!");
    }
}


void MainGamePanel::color_board(){
    /*
     * builds up the colored chess Board by initializing the panels
     */

    for (int i = 7; i >= 0; --i){
        for (int j = 0; j < 8; ++j) {
            panels[i * 8 + j] = new MyPanel(this);
            if ((i + j) % 2 == 0) {
                panels[i * 8 + j]->SetBackgroundColour(green);
            } else {
                panels[i * 8 + j]->SetBackgroundColour(weiss);
            }
        }
    }
    this->Refresh();
}


void MainGamePanel::add_pieces(GameState *gameState, Player *me){
    /*
     * adds pieces to the Board
     */

    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {

            // get the piece
            Piece *_piece = nullptr;
            if (me->get_color() == white) {
                _piece = gameState->get_board()->get_piece(i, j);
            } else {
                _piece = gameState->get_board()->get_piece(7 - i, 7 - j);
            }

            if (_piece != nullptr) {

                //add bitmap which represents the piece to the panel
                if (_piece->get_color() == white) {
                    if (_piece->get_type() == pawn) {
                        panels[i * 8 + j]->SetImage(w_pawn);
                    } else if (_piece->get_type() == king) {
                        panels[i * 8 + j]->SetImage(w_king);
                    } else if (_piece->get_type() == queen) {
                        panels[i * 8 + j]->SetImage(w_queen);
                    } else if (_piece->get_type() == rook) {
                        panels[i * 8 + j]->SetImage(w_rook);
                    } else if (_piece->get_type() == knight) {
                        panels[i * 8 + j]->SetImage(w_knight);
                    } else if (_piece->get_type() == bishop) {
                        panels[i * 8 + j]->SetImage(w_bishop);
                    }

                } else {
                    if (_piece->get_type() == pawn) {
                        panels[i * 8 + j]->SetImage(b_pawn);
                    } else if (_piece->get_type() == king) {
                        panels[i * 8 + j]->SetImage(b_king);
                    } else if (_piece->get_type() == queen) {
                        panels[i * 8 + j]->SetImage(b_queen);
                    } else if (_piece->get_type() == rook) {
                        panels[i * 8 + j]->SetImage(b_rook);
                    } else if (_piece->get_type() == knight) {
                        panels[i * 8 + j]->SetImage(b_knight);
                    } else if (_piece->get_type() == bishop) {
                        panels[i * 8 + j]->SetImage(b_bishop);
                    }
                }
            }
        }
    }
}


void MainGamePanel::display_moves(std::vector<std::vector<bool>> possible_moves, Player* me) {
    /*
     * displays possible moves by coloring the panels of possible moves differently
     */

    if(me->get_color() == white){
        for (int k = possible_moves.size() - 1; k >= 0; --k) {
            for (int l = 0; l < possible_moves.at(k).size(); ++l) {
                if(possible_moves.at(k).at(l) == true){
                    if ((k + l) % 2 == 0) {
                        panels[k * 8 + l]->SetBackgroundColour(orange);
                    } else {
                        panels[k * 8 + l]->SetBackgroundColour(light_orange);
                    }
                }
            }
        }
    } else if(me->get_color() == black){
        for (int k = possible_moves.size() - 1; k >= 0; --k) {
            for (int l = 0; l < possible_moves.at(k).size(); ++l) {
                if(possible_moves.at(k).at(l) == true){
                    if ((7-k + 7-l) % 2 == 0) {
                        panels[(7-k)*8+7-l]->SetBackgroundColour(orange);
                    } else {
                        panels[(7-k)*8+7-l]->SetBackgroundColour(light_orange);
                    }
                }
            }
        }
    }
    this->Refresh();
}


void MainGamePanel::deselect_moves(){
    /*
     * paints the chess Board in original coloring and therefor can be used to not show the possible moves anymore
     */
    for (int i = 7; i >= 0; --i){
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0) {
                panels[i * 8 + j]->SetBackgroundColour(green);
            } else {
                panels[i * 8 + j]->SetBackgroundColour(weiss);
            }
        }
    }
    this->Refresh();
}


wxStaticText* MainGamePanel::buildStaticText(const std::string& content, wxPoint position, wxSize size, long textAlignment, bool bold) {
    auto* staticText = new wxStaticText(this, wxID_ANY, content, position, size, textAlignment);
    staticText->SetForegroundColour(wxColor(0,0,0));
    if(bold) {
        wxFont font = staticText->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticText->SetFont(font);
    }
    return staticText;
}


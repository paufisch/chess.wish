#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../GameController.h"
//not jet existing includes:
#include "../../common/game_state/board.h"
#include "../../common/game_state/piece/piece.h"
#include "../../common/game_state/color.h"




MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
    selected = nullptr;

}

void MainGamePanel::buildGameState(game_state* gameState, player* me) {

    // remove any existing UI
    this->DestroyChildren();

    std::vector<player*> players = gameState->get_players();

    // find our own player object in the list of players
    int myPosition = -1;
    auto it = std::find_if(players.begin(), players.end(), [me](const player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
        myPosition = it - players.begin();
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    //get other player
    player* otherPlayer = players.at((myPosition + 1) % 2);

    // show our own player
    this->buildThisPlayer(gameState, me, otherPlayer);

    // update layout
    this->Layout();
}


void MainGamePanel::buildThisPlayer(game_state* gameState, player* me, player* otherPlayer) {

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


    // if the game has not yet started we say so
    if(!gameState->is_started()) {

        // show button that allows our player to start the game
        auto* startGameButton = new wxButton(this, wxID_ANY, "Start Game!", wxDefaultPosition, wxSize(200, 18));
        startGameButton->SetForegroundColour(wxColour(0,0,0));
        startGameButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
            GameController::startGame();
        });
        myInformation->Add(startGameButton, 1, wxEXPAND);

        // Show our name
        wxStaticText* playerName = buildStaticText(me->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        myInformation->Add(playerName, 1, wxEXPAND);

        //add other player
        wxStaticText *other_name = buildStaticText(otherPlayer->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
        otherInformation->Add(other_name, 1,  wxEXPAND);

        //display waiting
        wxStaticText *text_wait = buildStaticText("waiting...",wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
        otherInformation->Add(text_wait,0, wxEXPAND);

    //if the game has started we can build the chess board
    } else {

        //build board
        MainGamePanel::board = MainGamePanel::buildBoard(gameState, me);//this is actually the grid sizer
        boardLayout->Add(MainGamePanel::board, 0, wxALIGN_CENTER);

        //if it's our turn
        if (gameState->get_current_player() == me) {

            auto *resignButton = new wxButton(this, wxID_ANY, "Resign", wxDefaultPosition, wxSize(200, 18));
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

            //display that other player is waiting
            std::string statusText = "waiting...";
            wxStaticText *status_txt = buildStaticText(statusText,wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
            otherInformation->Add(status_txt, 1, wxEXPAND);

        // if it's not our turn, display "waiting..."
        } else {
             //display waiting
             wxStaticText *playerStatus = buildStaticText("waiting...",wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
             myInformation->Add(playerStatus, 1, wxEXPAND);

             // show our name
             wxStaticText* playerName = buildStaticText(me->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
             myInformation->Add(playerName, 1, wxEXPAND);

             // Show other player name
             wxStaticText *other_name = buildStaticText(otherPlayer->get_player_name(),wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,true);
             otherInformation->Add(other_name, 1, wxEXPAND);

             // Show other player's status label
             wxStaticText *status_txt = buildStaticText("their turn",wxDefaultPosition,wxSize(200, 18),wxALIGN_CENTER,false);
             otherInformation->Add(status_txt, 1, wxEXPAND);
        }
    }
}


//this function builds the chess board
wxGridSizer* MainGamePanel::buildBoard(game_state* gameState, player* me) {

    //color declarations
    wxColor yellow = wxColor(225, 245, 150);
    wxColor pink = wxColor(245, 175, 230);
    //wxColor white = wxColor(255, 255, 255);

    //black pieces
    wxBitmap b_pawn("../assets/black-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_king("../assets/black-king.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_queen("../assets/black-queen.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_rook("../assets/black-rook.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_knight("../assets/black-knight.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_bishop("../assets/black-bishop.png", wxBITMAP_TYPE_PNG);

    //white pieces
    wxBitmap w_pawn("../assets/white-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_king("../assets/white-king.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_queen("../assets/white-queen.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_rook("../assets/white-rook.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_knight("../assets/white-knight.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_bishop("../assets/white-bishop.png", wxBITMAP_TYPE_PNG);

    //the board is a grid sizer containing panels
    auto *grid = new wxGridSizer(8, 8, 0, 0);
    auto *panels = new wxPanel *[8*8];
    //wxPanel *panels[64];
    //std::array<wxPanel, 64> panels;


    // fill the panels with the pieces contained in board
    for (int i = 7; i >= 0; --i){
        for (int j = 0; j < 8; ++j) {

            panels[i * 8 + j] = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

            //color panels
            if ((i + j) % 2 == 0) {
                panels[i * 8 + j]->SetBackgroundColour(pink);
            } else {
                panels[i * 8 + j]->SetBackgroundColour(yellow);
            }
        }
    }
    for (int i = 7; i >= 0; --i){
        for (int j = 0; j < 8; ++j){

            auto *vbox = new wxBoxSizer(wxVERTICAL);
            // Add chess figures as bitmaps to the panels
            // get the piece
            Piece *_piece = nullptr;
            if(me->get_color() == white) { //white is a value of the enum "color" defined in color.h
                _piece = gameState->get_board()->get_piece(i, j);
            } else {
                _piece = gameState->get_board()->get_piece(7-i,7-j);
            }
            
            if (_piece != nullptr) {

                //add bitmap which represents the piece to the panel
                if(_piece->get_color() == white){
                    if(_piece->get_type() == pawn){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_pawn); //maybe set ID to the ID of the piece
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == king){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_king);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == queen){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_queen);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == rook){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_rook);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == knight){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_knight);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == bishop){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_bishop);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    }

                } else {
                    if(_piece->get_type() == pawn){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_pawn); //maybe set ID to the ID of the piece
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == king){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_king);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == queen){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_queen);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == rook){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_rook);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == knight){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_knight);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    } else if (_piece->get_type() == bishop){
                        auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_bishop);
                        vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                    }
                }
            }

            panels[i*8+j]->SetSizer(vbox);

            //add functionality to the button
            panels[i*8+j]->Bind(wxEVT_LEFT_DOWN, [=](wxMouseEvent& event) {
                //if it's our turn we can move pieces
                if (gameState->get_current_player() == me) {
                    //if no panel is selected, select one!
                    if (MainGamePanel::selected == nullptr) {

                        //get the selected piece
                        Piece *_piece = nullptr;
                        std::vector<std::vector<bool>> possible_moves;

                        if(me->get_color() == white && gameState->get_board()->get_piece(i, j) != nullptr) {
                            _piece = gameState->get_board()->get_piece(i, j);
                            possible_moves = _piece->legal_moves(i,j);
                            /*
                            std::cout << "selected piece in position " << i << " " << j << std::endl;
                            std::cout << "displaying legal moves: "<< std::endl;
                            for (int k = possible_moves.size() - 1; k >= 0; --k) {
                                for (int l = 0; l < possible_moves.at(k).size(); ++l) {
                                    std::cout << possible_moves.at(k).at(l) << " ";
                                }
                                std::cout << std::endl;
                            }
                            */
                        } else if(me->get_color() == black && gameState->get_board()->get_piece(7-i, 7-j) != nullptr) {
                            _piece = gameState->get_board()->get_piece(7 - i, 7 - j);
                            possible_moves = _piece->legal_moves(7-i,7-j);
                        }

                        if(_piece != nullptr){
                            //sum the total number of possible moves
                            int sum = 0;
                            for (const auto& row : possible_moves) {
                                for (bool value : row) {
                                    sum += value;
                                }
                            }

                            //if the selected piece is our piece and has possible moves we select it
                            if(_piece->get_color() == me->get_color() && sum > 0){
                                MainGamePanel::selected = new unsigned int[2];
                                MainGamePanel::selected[0] = i;
                                MainGamePanel::selected[1] = j;
                                //TODO: display valid moves
                                //display_moves(panels, possible_moves, me);
                                if(me->get_color() == white){
                                    for (int k = possible_moves.size() - 1; k >= 0; --k) {
                                        for (int l = 0; l < possible_moves.at(k).size(); ++l) {
                                            if(possible_moves.at(k).at(l) == true){
                                                panels[k*8+l]->SetOwnBackgroundColour(wxColor(100,255, 0, 0.5));
                                            }
                                        }
                                    }
                                } else if(me->get_color() == black){
                                    for (int k = possible_moves.size() - 1; k >= 0; --k) {
                                        for (int l = 0; l < possible_moves.at(k).size(); ++l) {
                                            if(possible_moves.at(k).at(l) == true){
                                                panels[(7-k)*8+7-l]->SetOwnBackgroundColour(wxColor(100,255, 0, 0.5));
                                            }
                                        }
                                    }
                                }
                                this->Refresh();
                                //....
                            } else {
                                GameController::showError("Error", "Select a different piece!");

                            }
                        } else {
                            GameController::showError("Error", "No piece here!");
                        }



                    //if we click the selected piece again we deselect it
                    } else if (i == MainGamePanel::selected[0] && j == MainGamePanel::selected[1]){
                        std::cout << "deselected piece in position " << i << " " << j << std::endl;
                        delete[] MainGamePanel::selected;
                        MainGamePanel::selected = nullptr;
                        //TODO: remove the highlighted valid moves
                        for (int i = 7; i >= 0; --i){
                            for (int j = 0; j < 8; ++j) {
                                //color panels
                                if ((i + j) % 2 == 0) {
                                    panels[i * 8 + j]->SetBackgroundColour(pink);
                                } else {
                                    panels[i * 8 + j]->SetBackgroundColour(yellow);
                                }
                            }
                        }
                        this->Refresh();

                        //else if move previously selected piece to new position
                    } else {
                        unsigned int from_i = MainGamePanel::selected[0];
                        unsigned int from_j = MainGamePanel::selected[1];

                        std::vector<std::vector<bool>> possible_moves;
                        if(me->get_color() == white){
                            //check if selected destination is a possible move
                            possible_moves = gameState->select_piece(from_i, from_j);
                            if (possible_moves[i][j] == true){
                                GameController::movePiece(from_i, from_j, i, j);
                                //deselect piece
                                delete[] MainGamePanel::selected;
                                MainGamePanel::selected = nullptr;
                            } else {
                                GameController::showError("Error", "Not a valid move!");
                            }
                        } else {
                            //check if selected destination is a possible move
                            possible_moves = gameState->select_piece(7-from_i, 7-from_j);
                            if (possible_moves[7-i][7-j] == true){
                                GameController::movePiece(7-from_i, 7-from_j, 7-i, 7-j);
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
            });

            grid->Add(panels[i*8+j],1, wxEXPAND | wxALL, 0);
        }
    }

    grid->SetMinSize(wxSize(800, 800));
    return grid;
}


void MainGamePanel::display_moves(wxPanel (&panel)[64], std::vector<std::vector<bool>> possible_moves, player* me) {

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


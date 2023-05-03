#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../GameController.h"


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {

}

void MainGamePanel::buildGameState(game_state* gameState, player* me) {

    // remove any existing UI
    this->DestroyChildren();

    std::vector<player*> players = gameState->get_players();

    // find our own player object in the list of players
    int myPosition = -1;
    std::vector<player*>::iterator it = std::find_if(players.begin(), players.end(), [me](const player* x) {
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
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    wxBoxSizer* boardLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(boardLayout, 0, wxEXPAND);
    wxBoxSizer* innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 0, wxEXPAND);
    wxBoxSizer* otherInformation = new wxBoxSizer(wxVERTICAL);
    innerLayout->Add(otherInformation, 1, wxEXPAND | wxTOP, 5);
    wxPanel *place_holder = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(50,200));
    innerLayout->Add(place_holder, 3, wxEXPAND);
    wxBoxSizer* myInformation = new wxBoxSizer(wxVERTICAL);
    innerLayout->Add(myInformation, 1, wxEXPAND | wxBOTTOM, 5);


    // if the game has not yet started we say so
    if(!gameState->is_started()) {

        // show button that allows our player to start the game
        wxButton* startGameButton = new wxButton(this, wxID_ANY, "Start Game!", wxDefaultPosition, wxSize(200, 18));
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

    } else {

        //build board
        MainGamePanel::board = MainGamePanel::buildBoard(gameState, me);
        boardLayout->Add(MainGamePanel::board, 0, wxALIGN_CENTER);

        //if it's our turn
        if (gameState->get_current_player() == me) {

            wxButton *resignButton = new wxButton(this, wxID_ANY, "Resign", wxDefaultPosition, wxSize(200, 18));
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
wxGridSizer* MainGamePanel::buildBoard(game_state* gameState, player* me){


    //new version added some figures
    wxGridSizer *grid = new wxGridSizer(8, 8, 0, 0);
    wxColor white = wxColor(255, 255, 255);
    wxColor pink = wxColor(245, 175, 230);
    wxColor green = wxColor(225, 245, 150);
    wxBitmap b_pawn("../assets/black-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_king("../assets/black-king.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_pawn("../assets/white-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_king("../assets/white-king.png", wxBITMAP_TYPE_PNG);

    // Add bitmaps for other pieces
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            wxBitmapButton *button = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
            if ((i+j)%2 == 0) {
                button->SetBackgroundColour(green);
            } else {
                button->SetBackgroundColour(pink);
            }
            grid->Add(button, 1, wxEXPAND | wxALL, 0);
            // Add bitmaps to appropriate buttons
            if (i == 1) {
                button->SetBitmap(b_pawn);
            } else if (i == 0 && j == 4) {
                button->SetBitmap(b_king);
            } else if (i == 6) {
                button->SetBitmap(w_pawn);
            } else if (i==7 && j == 4) {
                button->SetBitmap(w_king);
            } else {
                //button->SetBitmap(wxNullBitmap);
                //button->Show(false);
            }
        }
    }
    grid->SetMinSize(wxSize(800, 800));
    return grid;

}


wxStaticText* MainGamePanel::buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold) {
    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, content, position, size, textAlignment);
    staticText->SetForegroundColour(wxColor(0,0,0));
    if(bold) {
        wxFont font = staticText->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticText->SetFont(font);
    }
    return staticText;
}


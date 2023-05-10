#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../GameController.h"


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
    selected_panel = nullptr;

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

    } else {

        //build board
        MainGamePanel::board = MainGamePanel::buildBoard(gameState, me);
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


    //new version added some figures
    auto *grid = new wxGridSizer(8, 8, 0, 0);
    wxColor white = wxColor(255, 255, 255);
    wxColor pink = wxColor(245, 175, 230);
    wxColor green = wxColor(225, 245, 150);
    wxBitmap b_pawn("../assets/black-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap b_king("../assets/black-king.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_pawn("../assets/white-pawn.png", wxBITMAP_TYPE_PNG);
    wxBitmap w_king("../assets/white-king.png", wxBITMAP_TYPE_PNG);

    // Add bitmaps for other pieces

    auto *panels = new wxPanel *[8*8];
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j)
        {
            panels[i*8+j] = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
            auto *vbox = new wxBoxSizer(wxVERTICAL);

            if ((i + j) % 2 == 0){
                panels[i*8+j]->SetBackgroundColour(green);
            } else {
                panels[i*8+j]->SetBackgroundColour(pink);
            }

            // Add chess figures as bitmaps to the panels
            //whites perspective
            if(me->get_player_name() == "white"){
                if (i == 1) {
                auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_pawn);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                } else if ((i == 0) && (j == 4)) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_king);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                }else if (i == 6) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_pawn);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                } else if ((i == 7) && (j == 4)) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_king);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER | wxALL, 5);
                }
            //blacks perspective
            } else {
                if (i == 1) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_pawn);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                } else if ((i == 0) && (j == 4)) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, b_king);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                }else if (i == 6) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_pawn);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                } else if ((i == 7) && (j == 4)) {
                    auto *sbmp = new wxStaticBitmap(panels[i*8+j], wxID_ANY, w_king);
                    vbox->Add(sbmp, 1, wxALIGN_CENTER | wxALL, 5);
                }
            }

            panels[i*8+j]->SetSizer(vbox);

            panels[i*8+j]->Bind(wxEVT_LEFT_DOWN, [=](wxMouseEvent& event) {

                //if no panel is selected, select one!
                if(MainGamePanel::selected_panel == nullptr) {
                    MainGamePanel::selected_panel = panels[i*8+j];

                //else move previously selected piece to new position
                } else {
                    wxSizer *sizer = MainGamePanel::selected_panel->GetSizer();//sizer of source panel
                    wxSizer *box = panels[i*8+j]->GetSizer();//sizer of destination panel

                    //if the selected panel contains a piece move it
                    if(sizer->GetItemCount() > 0){
                        int idx  = sizer->GetItemCount() - 1;//piece id
                        auto *child = dynamic_cast<wxStaticBitmap*>(sizer->GetItem(idx)->GetWindow());
                        wxBitmap piece = child->GetBitmap();
                        //remove piece from old position
                        MainGamePanel::selected_panel->RemoveChild(child);
                        sizer->Detach(child);
                        child->Destroy();

                        //check if the destination panel contains a piece
                        if(box->GetItemCount()>0){
                            int idx2 = box->GetItemCount() -1;
                            auto *child2 = dynamic_cast<wxStaticBitmap*>(box->GetItem(idx2)->GetWindow());//get contained piece
                            //remove piece from old position
                            panels[i*8+j]->RemoveChild(child2);
                            box->Detach(child2);
                            child2->Destroy();
                            auto* newBitmap = new wxStaticBitmap(panels[i*8+j], wxID_ANY, piece);
                            box->Add(newBitmap, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                        } else {
                            auto* newBitmap = new wxStaticBitmap(panels[i*8+j], wxID_ANY, piece);
                            box->Add(newBitmap, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxSHAPED | wxALL, 5);
                        }
                    } else {
                        //here we should display a warning to select a different piece.
                    }
                    box->Layout();
                    MainGamePanel::selected_panel = nullptr;
                }
            });

            grid->Add(panels[i*8+j],1, wxEXPAND | wxALL, 0);
        }
    }

    grid->SetMinSize(wxSize(800, 800));
    return grid;
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


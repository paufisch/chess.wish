#ifndef CHESS_CLIENT_MAINGAMEPANEL_H
#define CHESS_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/GameState.h"
#include "MyPanel.h"


class MainGamePanel : public wxPanel {

public:
    explicit MainGamePanel(wxWindow* parent);
    void buildGameState(GameState* gameState, Player* me);


private:
    wxGridSizer* board{};
    unsigned int* selected;
    MyPanel *panels[64];

    wxGridSizer* buildBoard(GameState* gameState, Player* me);
    void buildThisPlayer(GameState* gameState, Player* me, Player* otherPlayer);
    void color_board();
    void add_pieces(GameState* gameState, Player* me);
    void display_moves(std::vector<std::vector<bool>>, Player*);
    void deselect_moves();
    void OnPanelClick(int i, int j, GameState*, Player* me);
    wxStaticText* buildStaticText(const std::string& content, wxPoint position, wxSize size, long textAlignment, bool bold = false);

    //colors
    wxColor green;
    wxColor weiss;
    wxColor orange;
    wxColor light_orange;

    //paths to chess pieces
    wxString b_pawn;
    wxString b_king;
    wxString b_queen;
    wxString b_rook;
    wxString b_knight;
    wxString b_bishop;
    wxString w_pawn;
    wxString w_king;
    wxString w_queen;
    wxString w_rook;
    wxString w_knight;
    wxString w_bishop;
};


#endif //CHESS_CLIENT_MAINGAMEPANEL_H

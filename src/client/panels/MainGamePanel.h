#ifndef CHESS_CLIENT_MAINGAMEPANEL_H
#define CHESS_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/game_state.h"
#include "MyPanel.h"


class MainGamePanel : public wxPanel {

public:
    explicit MainGamePanel(wxWindow* parent);
    void buildGameState(game_state* gameState, player* me);


private:
    wxGridSizer* board{};
    unsigned int* selected;
    MyPanel *panels[64];

    wxGridSizer* buildBoard(game_state* gameState, player* me);
    void buildThisPlayer(game_state* gameState, player* me, player* otherPlayer);
    void color_board();
    void add_pieces(game_state* gameState, player* me);
    void display_moves(std::vector<std::vector<bool>>, player*);
    void deselect_moves();
    void OnPanelClick(int i, int j, game_state*, player* me);
    wxStaticText* buildStaticText(const std::string& content, wxPoint position, wxSize size, long textAlignment, bool bold = false);

    //colors
    wxColor green;
    wxColor weiss;
    wxColor orange;

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

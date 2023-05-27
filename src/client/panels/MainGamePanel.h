#ifndef CHESS_CLIENT_MAINGAMEPANEL_H
#define CHESS_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/game_state.h"
#include "MyPanel.h"


class MainGamePanel : public wxPanel {

public:
    explicit MainGamePanel(wxWindow* parent);
    void buildGameState(game_state* gameState, player* me);
    void display_moves(std::vector<std::vector<bool>>, player*);
    void color_board();
    void deselect_moves();
    void OnPanelClick(int i, int j, game_state*, player* me);



private:
    wxGridSizer* board{};
    void buildThisPlayer(game_state* gameState, player* me, player* otherPlayer);
    wxGridSizer* buildBoard(game_state* gameState, player* me);
    wxStaticText* buildStaticText(const std::string& content, wxPoint position, wxSize size, long textAlignment, bool bold = false);
    unsigned int* selected;
    MyPanel *panels[64];
    void OnPaint(wxPaintEvent& event);

    //colors
    wxColor yellow;
    wxColor pink;
    wxColor high_yellow;
    wxColor high_pink;

    //chess pieces
    wxBitmap b_pawn;
    wxBitmap b_king;
    wxBitmap b_queen;
    wxBitmap b_rook;
    wxBitmap b_knight;
    wxBitmap b_bishop;
    wxBitmap w_pawn;
    wxBitmap w_king;
    wxBitmap w_queen;
    wxBitmap w_rook;
    wxBitmap w_knight;
    wxBitmap w_bishop;

};


#endif //CHESS_CLIENT_MAINGAMEPANEL_H

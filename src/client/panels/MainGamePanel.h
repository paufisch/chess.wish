#ifndef LAMA_CLIENT_MAINGAMEPANEL_H
#define LAMA_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/game_state.h"


class MainGamePanel : public wxPanel {

public:
    explicit MainGamePanel(wxWindow* parent);
    void buildGameState(game_state* gameState, player* me);



private:
    wxGridSizer* board{};
    void buildThisPlayer(game_state* gameState, player* me, player* otherPlayer);
    wxGridSizer* buildBoard(game_state* gameState, player* me);
    wxStaticText* buildStaticText(const std::string& content, wxPoint position, wxSize size, long textAlignment, bool bold = false);
    wxPanel* selected_panel;
};


#endif //LAMA_CLIENT_MAINGAMEPANEL_H

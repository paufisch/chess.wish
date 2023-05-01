#ifndef LAMA_CLIENT_MAINGAMEPANEL_H
#define LAMA_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/game_state.h"


class MainGamePanel : public wxPanel {

public:
    MainGamePanel(wxWindow* parent);
    void buildGameState(game_state* gameState, player* me);


private:

    void buildOtherPlayerLabels(game_state* gameState, player* otherPlayer, double playerAngle, int side);
    void buildThisPlayer(game_state* gameState, player* me);
    void buildBoard(game_state* gameState, player* me);
    wxStaticText* buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold = false);
    wxPoint getPointOnEllipse(double horizontalRadius, double verticalRadius, double angle);

    // define key constant layout values
    wxSize const panelSize = wxSize(960, 680); // also set in the constructor implementation
    wxPoint const tableCenter = wxPoint(480, 300);
    double const otherPlayerLabelDistanceFromCenter = 275.0;
    double const twoPi = 6.28318530718;
};


#endif //LAMA_CLIENT_MAINGAMEPANEL_H

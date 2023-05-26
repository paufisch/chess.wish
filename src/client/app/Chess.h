#ifndef CHESSUI_CHESS_H
#define CHESSUI_CHESS_H

#include <wx/wx.h>
#include "../windows/GameWindow.h"
#include "../GameController.h"


// Main app class
class Chess : public wxApp
{
public:
    virtual bool OnInit();
};


#endif //CHESSUI_CHESS_H

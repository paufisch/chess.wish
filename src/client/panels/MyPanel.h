//
// Created by Paul Fischer on 27.05.23.
//
//The MyPanel is a wxPanel which overrides the OnPaint function which shows the chess piece saved at a given
// image Path on the panel

#ifndef CHESS_MYPANEL_H
#define CHESS_MYPANEL_H

#include <wx/wx.h>

class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow* parent);
    void SetImage(const wxString& imagePath);

private:
    wxString m_imagePath;
    void OnPaint(wxPaintEvent& event);
};


#endif //CHESS_MYPANEL_H

//
// Created by Paul Fischer on 27.05.23.
//

#ifndef CHESS_MYPANEL_H
#define CHESS_MYPANEL_H

#include <wx/wx.h>

class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow* parent)
            : wxPanel(parent)
    {
        // Bind the paint event handler
        Bind(wxEVT_PAINT, &MyPanel::OnPaint, this);
    }

    void SetImage(const wxString& imagePath)
    {
        m_imagePath = imagePath;
        Refresh(); // Refresh the panel to trigger a repaint
    }

private:
    wxString m_imagePath;

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        if (!m_imagePath.IsEmpty())
        {
            // Load the PNG image
            wxImage image;
            if (image.LoadFile(m_imagePath, wxBITMAP_TYPE_PNG))
            {
                // Scale the image to fit the panel size
                wxSize panelSize = GetClientSize();
                wxImage scaledImage = image.Rescale(0.8*panelSize.x, 0.8*panelSize.y, wxIMAGE_QUALITY_HIGH);

                // Calculate the position to center the image within the panel
                int x = (panelSize.x - scaledImage.GetWidth()) / 2;
                int y = (panelSize.y - scaledImage.GetHeight()) / 2;

                // Draw the image on the panel
                dc.DrawBitmap(wxBitmap(scaledImage), x, y);
            }
            else
            {
                wxMessageBox("Failed to load image: " + m_imagePath);
            }
        }
    }
};


#endif //CHESS_MYPANEL_H

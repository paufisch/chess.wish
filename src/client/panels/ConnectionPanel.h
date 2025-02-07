#ifndef TEST_CONNECTIONPANEL_H
#define TEST_CONNECTIONPANEL_H

#include <wx/wx.h>
#include "../uiElements/InputField.h"


class ConnectionPanel : public wxPanel {

public:
    explicit ConnectionPanel(wxWindow* parent);
    wxString getServerAddress();
    wxString getServerPort();
    wxString getPlayerName();

private:
    InputField* _serverAddressField;
    InputField* _serverPortField;
    InputField* _playerNameField;

};


#endif //TEST_CONNECTIONPANEL_H

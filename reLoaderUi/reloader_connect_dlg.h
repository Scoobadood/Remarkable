//
// Created by Dave Durbin (Old) on 20/2/21.
//

#ifndef REMARKABLE_RELOADER_CONNECT_DLG_H
#define REMARKABLE_RELOADER_CONNECT_DLG_H

#include <string>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class reloader_connect_dlg : public wxDialog {
    enum {
        BTN_CANCEL,
        BTN_CONNECT
    };
    enum {
        TXT_HOST,
        TXT_PASSWORD
    };
    wxTextCtrl * txt_host_name;
    wxTextCtrl * txt_password;
public:
    std::string get_host();
    std::string get_password();

    reloader_connect_dlg(const std::string& host, const std::string & password);
};


#endif //REMARKABLE_RELOADER_CONNECT_DLG_H

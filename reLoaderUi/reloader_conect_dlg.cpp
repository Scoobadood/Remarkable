//
// Created by Dave Durbin (Old) on 20/2/21.
//

#include "reloader_connect_dlg.h"

reloader_connect_dlg::reloader_connect_dlg(const std::string &host, const std::string &password)  : wxDialog(nullptr, 1, "Connect") {
    auto sizer = new wxFlexGridSizer(3, 2, 10, 10);

    sizer->Add(new wxStaticText(this, TXT_HOST, "Host"));
    txt_host_name = new wxTextCtrl(this, TXT_HOST, host);
    sizer->Add(txt_host_name);

    sizer->Add(new wxStaticText(this, TXT_PASSWORD, "Password"));
    txt_password = new wxTextCtrl(this, TXT_PASSWORD, password);
    sizer->Add(txt_password);

    auto btn_connect = new wxButton( this, BTN_CONNECT, "Connect");
    auto btn_cancel = new wxButton( this, BTN_CANCEL, "Exit");
    sizer->Add( btn_cancel);
    sizer->Add( btn_connect);
    SetSizer(sizer);
    SetDefaultItem(btn_connect);
    SetEscapeId(BTN_CANCEL);
    SetAffirmativeId(BTN_CONNECT);
}

std::string reloader_connect_dlg::get_host(){
    return txt_host_name->GetValue().ToStdString();
}
std::string reloader_connect_dlg::get_password() {
    return txt_password->GetValue().ToStdString();
}

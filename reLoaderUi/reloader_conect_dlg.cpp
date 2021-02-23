//
// Created by Dave Durbin (Old) on 20/2/21.
//

#include "reloader_connect_dlg.h"
std::string reloader_connect_dlg::get_host(){
    return txt_host_name->GetValue().ToStdString();
}
std::string reloader_connect_dlg::get_password() {
    return txt_password->GetValue().ToStdString();
}

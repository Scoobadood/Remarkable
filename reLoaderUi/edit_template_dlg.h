//
// Created by Dave Durbin (Old) on 28/2/21.
//

#ifndef REMARKABLE_EDIT_TEMPLATE_DLG_H
#define REMARKABLE_EDIT_TEMPLATE_DLG_H

#include <string>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class edit_template_dlg : public wxDialog {
    wxTextCtrl * txt_template_name;
    wxCheckBox * cb_is_landscape;
public:
    std::string get_is_landscape();
    std::string get_template_name();

    edit_template_dlg(wxWindow * parent, const std::string & template_name, bool is_landscape);
};

#endif //REMARKABLE_EDIT_TEMPLATE_DLG_H

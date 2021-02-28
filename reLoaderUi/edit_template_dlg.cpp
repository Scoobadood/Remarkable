//
// Created by Dave Durbin (Old) on 28/2/21.
//

#include "edit_template_dlg.h"

edit_template_dlg::edit_template_dlg(wxWindow *parent, const std::string &template_name, bool is_landscape)
        : wxDialog(parent, 2, "Edit") {
    auto sizer = new wxFlexGridSizer(3, 2, 10, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Template name"));
    txt_template_name = new wxTextCtrl(this, wxID_ANY, template_name);
    sizer->Add(txt_template_name);

    sizer->Add(new wxStaticText(this, wxID_ANY, "Landscape?"));
    cb_is_landscape = new wxCheckBox(this, wxID_ANY, "label");
    cb_is_landscape->Set3StateValue(is_landscape ? wxCHK_CHECKED : wxCHK_UNCHECKED);
    sizer->Add(cb_is_landscape);

    auto btn_cancel = new wxButton(this, wxID_ANY, "Cancel");
    auto btn_connect = new wxButton(this, wxID_ANY, "Save");
    sizer->Add(btn_cancel);
    sizer->Add(btn_connect);
    SetSizer(sizer);
    SetDefaultItem(btn_connect);
    SetEscapeId(btn_cancel->GetId());
    SetAffirmativeId(btn_connect->GetId());
}
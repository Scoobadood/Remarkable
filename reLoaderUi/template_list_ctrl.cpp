//
// Created by Dave Durbin (Old) on 27/2/21.
//

#include "template_list_ctrl.h"

int wxCALLBACK sort_template_list(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData) {
    // Sort installed templates to the front
    auto *tp1 = (rm_template *) item1;
    auto *tp2 = (rm_template *) item2;
    if (tp1->is_built_in() && !tp2->is_built_in()) {
        return 1;
    }
    if (!tp1->is_built_in() && tp2->is_built_in()) {
        return -1;
    }
    return tp1->get_name().compare(tp2->get_name());
}

template_list_ctrl::template_list_ctrl(wxWindow *parent) : wxScrolledWindow(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxVSCROLL) {
    SetScrollRate(0,1);
}

void template_list_ctrl::set_templates(const std::vector<rm_template> &templates) {
    DestroyChildren();
    auto sizer = new wxBoxSizer(wxVERTICAL);
    for (const auto &tplate : templates) {
        // Make a Panel
        auto panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxWHITE);

        

        // Add text to it
        auto text = new wxStaticText(panel, wxID_ANY,tplate.get_name());
        if( tplate.is_built_in()) {
            text->SetForegroundColour(*wxLIGHT_GREY);
            text->SetFont(*wxITALIC_FONT);
        }
        // Add it to the box sizer
        sizer->Add(panel,wxSizerFlags(1).Align(wxALIGN_LEFT).Expand());
    }
    SetSizerAndFit(sizer);
}

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
    selected_row = nullptr;
}

void set_row_colour(wxPanel * row, bool is_selected, bool is_built_in ) {
    auto text = dynamic_cast<wxStaticText *>(row->GetChildren().GetFirst()->GetData());
    wxColour bgColour;
    wxColour fgColour;

    if( is_selected) {
        fgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT);
        bgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    } else {
        if( is_built_in) {
            fgColour = *wxLIGHT_GREY;
            text->SetFont(*wxITALIC_FONT);
        } else {
            fgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_LISTBOXTEXT);
        }
        bgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_LISTBOX);
    }
    text->SetForegroundColour(fgColour);
    text->SetBackgroundColour(bgColour);
    row->SetBackgroundColour(bgColour);
    row->Refresh();
}

bool is_row_built_in(wxPanel * panel) {
    auto flag_ptr = (bool *)(panel->GetClientData());
    return *flag_ptr;
}

void template_list_ctrl::row_selected(wxEvent & event) {
    if( selected_row != nullptr ) {
        set_row_colour(selected_row, false, is_row_built_in(selected_row));
        selected_row = nullptr;
    }
    selected_row = (wxPanel *)event.GetEventObject();
    set_row_colour(selected_row, true, is_row_built_in(selected_row));
}

void template_list_ctrl::set_templates(const std::vector<rm_template> &templates) {
    DestroyChildren();
    auto sizer = new wxBoxSizer(wxVERTICAL);
    for (const auto &tplate : templates) {
        // Make a Panel
        auto panel = new wxPanel(this);

        // Add text to it
        auto text = new wxStaticText(panel, wxID_ANY,tplate.get_name());
        bool * is_built_in_ptr = new bool{tplate.is_built_in()};
        panel->SetClientData(is_built_in_ptr);
        set_row_colour(panel, false, *is_built_in_ptr );

        // Add selectability
        panel->Bind(wxEVT_LEFT_DOWN,&template_list_ctrl::row_selected, this);

        // Add it to the box sizer
        sizer->Add(panel,wxSizerFlags(1).Align(wxALIGN_LEFT).Expand());
    }
    SetSizerAndFit(sizer);
}

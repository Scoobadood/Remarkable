//
// Created by Dave Durbin (Old) on 27/2/21.
//

#include "template_list_ctrl.h"
#include "bitmaps/trash_icon_16.xpm"
#include "bitmaps/edit_icon_16.xpm"
#include "edit_template_dlg.h"

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

template_list_ctrl::template_list_ctrl(wxWindow *parent) : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition,
                                                                            wxDefaultSize, wxVSCROLL) {
    SetScrollRate(0, 1);
    selected_row = nullptr;
    SetBackgroundColour(wxSystemSettingsNative::GetColour(wxSYS_COLOUR_LISTBOX));
}

void set_row_colour(wxPanel *row, bool is_selected, bool is_built_in) {
    auto text = dynamic_cast<wxStaticText *>(row->GetChildren().GetFirst()->GetData());
    wxColour bgColour;
    wxColour fgColour;

    if (is_selected) {
        fgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT);
        bgColour = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    } else {
        if (is_built_in) {
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

bool is_row_built_in(wxPanel *panel) {
    auto tplate_ptr = (rm_template *) (panel->GetClientData());
    return tplate_ptr->is_built_in();
}

void template_list_ctrl::row_selected(wxEvent &event) {
    if (selected_row != nullptr) {
        set_row_colour(selected_row, false, is_row_built_in(selected_row));
        selected_row = nullptr;
    }
    selected_row = (wxPanel *) event.GetEventObject();
    set_row_colour(selected_row, true, is_row_built_in(selected_row));
}

rm_template *pointer_for_button(wxObject *source) {
    // Get source parent; the panel
    auto panel = (wxPanel *) ((wxWindow *) source)->GetParent();
    auto tplate_ptr = (rm_template *) panel->GetClientData();
    return tplate_ptr;
}

void template_list_ctrl::on_delete(wxEvent &event) {
    auto tplate_ptr = pointer_for_button(event.GetEventObject());
    std::cout << "on_delete " << tplate_ptr->get_name() << std::endl;
}

void template_list_ctrl::on_edit(wxEvent &event) {
    auto tplate_ptr = pointer_for_button(event.GetEventObject());
    std::cout << "on_edit " << tplate_ptr->get_name() << std::endl;
    // Display modal dialog allowing for editing of name, landscapishness.
    auto dlg = new edit_template_dlg(this, tplate_ptr->get_name(), tplate_ptr->is_landscape());
    dlg->ShowModal();
}


wxPanel *template_list_ctrl::make_template_row_panel(wxWindow *parent, const rm_template &tplate) {
    // Make a Panel
    auto panel = new wxPanel(parent);

    // Add text to it

    auto text = new wxStaticText(panel, wxID_ANY, tplate.get_name());
    auto tplate_copy = new rm_template(tplate);
    panel->SetClientData(tplate_copy);
    set_row_colour(panel, false, tplate.is_built_in());

    auto row_sizer = new wxBoxSizer(wxHORIZONTAL);
    row_sizer->Add(text,
                   wxSizerFlags(8)
                           .Expand()
                           .Left()
                           .Border(wxALL, 5)
    );

    if (!tplate.is_built_in()) {
        // Add edit button to it
        auto edit_bmp = new wxBitmap(edit_icon_xpm);
        auto edit_button = new wxStaticBitmap(panel, wxID_ANY, *edit_bmp, wxDefaultPosition, wxSize(16, 16));
        edit_button->Bind(wxEVT_LEFT_DOWN, &template_list_ctrl::on_edit, this);
        row_sizer->Add(edit_button,
                       wxSizerFlags(1)
                               .CenterVertical()
                               .Border(wxUP, 5)
                               .Border(wxDOWN, 5)
                               .Border(wxRIGHT, 5)
        );

        // Add a delete button to it
        auto delete_bmp = new wxBitmap(trash_icon_xpm);
        auto delete_button = new wxStaticBitmap(panel, wxID_ANY, *delete_bmp, wxDefaultPosition, wxSize(16, 16));
        delete_button->Bind(wxEVT_LEFT_DOWN, &template_list_ctrl::on_delete, this);
        row_sizer->Add(delete_button,
                       wxSizerFlags(1)
                               .CenterVertical()
                               .Border(wxUP, 5)
                               .Border(wxDOWN, 5)
                               .Border(wxRIGHT, 5)
        );
    }
    panel->SetSizerAndFit(row_sizer);

    return panel;
}

void template_list_ctrl::set_templates(const std::vector<rm_template> &templates) {
    DestroyChildren();
    auto sizer = new wxBoxSizer(wxVERTICAL);
    for (const auto &tplate : templates) {

        auto panel = make_template_row_panel(this, tplate);

        // Add selectability
        panel->Bind(wxEVT_LEFT_DOWN, &template_list_ctrl::row_selected, this);

        // Add it to the box sizer
        sizer->Add(panel,
                   wxSizerFlags(1)
                           .Left()
                           .Border(wxALL, 2));
    }
    SetSizerAndFit(sizer);
}

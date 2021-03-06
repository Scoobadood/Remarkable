//
// Created by Dave Durbin (Old) on 27/2/21.
//

#ifndef REMARKABLE_TEMPLATE_LIST_CTRL_H
#define REMARKABLE_TEMPLATE_LIST_CTRL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <vector>

#include <remarkable/rm_template.h>

class template_list_ctrl : public wxScrolledWindow {
public:
    explicit template_list_ctrl(wxWindow *parent);

    void set_templates(const std::vector<rm_template> &templates);

private:
    wxPanel * selected_row;
    void row_selected(wxEvent &event);
    void on_delete(wxEvent &event);
    void on_edit(wxEvent &event);
    wxPanel * make_template_row_panel(wxWindow * parent, const rm_template& tplate);
};

#endif //REMARKABLE_TEMPLATE_LIST_CTRL_H

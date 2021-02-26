//
// Created by Dave Durbin (Old) on 27/2/21.
//

#include <wx/imaglist.h>
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

template_list_ctrl::template_list_ctrl(wxWindow *parent) : wxListView(parent) {
    AppendColumn("Templates");
}

void template_list_ctrl::set_templates(const std::vector<rm_template> &templates) {
    DeleteAllItems();
    auto item_index = 0;
    for (const auto &tplate : templates) {
        InsertItem(item_index, tplate.get_name());
        SetItemPtrData(item_index, (wxUIntPtr) &templates.at(item_index));
        SetItemTextColour(item_index,
                          tplate.is_built_in()
                          ? *wxLIGHT_GREY
                          : *wxBLACK
        );

        if (tplate.is_built_in()) {
            SetItemFont(item_index, *wxITALIC_FONT);
        }
        item_index++;
    }
    SortItems(sort_template_list, 0);
    SetColumnWidth(0, wxLIST_AUTOSIZE);
    // Remove client data because the vectors going out of scope.
    for (int i = 0; i < GetItemCount(); ++i) {
        SetItemPtrData(i, 0);
    }
}

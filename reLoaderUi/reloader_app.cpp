//
// Created by Dave Durbin (Old) on 19/2/21.
//

#include <wx/splitter.h>
#include <wx/listctrl.h>
#include "reloader_app.h"
#include "reloader_connect_dlg.h"

template<typename ... Args>
std::string string_format(const std::string &format, Args ... args) {
    int size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

int wxCALLBACK sort_template_list(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)  {
    // Sort installed templates to the front
    auto * tp1 = (rm_template *) item1;
    auto * tp2 = (rm_template *) item2;
    if(tp1->is_built_in() && !tp2->is_built_in()) {
        return 1;
    }
    if(!tp1->is_built_in() && tp2->is_built_in()) {
        return -1;
    }
    return tp1->get_name().compare(tp2->get_name());
}


wxListView * make_template_list(wxWindow *parent, remarkable * const device) {
    auto list = new wxListView(parent, 401);
    list->AppendColumn("Templates");
    auto item_index = 0;
    auto installed_templates = device->get_installed_templates();
    for (const auto &tplate : installed_templates) {
        list->InsertItem(item_index, tplate.get_name());
        list->SetItemPtrData(item_index, (wxUIntPtr) &installed_templates.at(item_index));
        list->SetItemTextColour(item_index,
                                tplate.is_built_in()
                                ? *wxLIGHT_GREY
                                : *wxBLACK
        );
        if (tplate.is_built_in()) {
            list->SetItemFont(item_index, *wxITALIC_FONT);
        }
        item_index++;
    }
    list->SortItems(sort_template_list, 0);
    list->SetColumnWidth(0, wxLIST_AUTOSIZE);
    // Remove client data because the vectors going out of scope.
    for( int i=0; i<list->GetItemCount(); ++i ) {
        list->SetItemPtrData(i, 0);
    }

    return list;
}

bool reloader_app::OnInit() {
    // TODO: Pull these from persistent storage
    auto *connect_dialog = new reloader_connect_dlg("192.168.0.7", "wg8czspbIz");
    auto dialog_return_value = connect_dialog->ShowModal();
    // You do not have cancel button so only check wxYES and wxNO
    switch (dialog_return_value) {
        // CONNECT
        case 1 : {
            device = new remarkable(connect_dialog->get_host(), connect_dialog->get_password());
            connect_dialog->Close();
            auto *frame = new wxFrame(nullptr, 69, "reLoader");

            auto *splitter = new wxSplitterWindow(frame, wxID_ANY);
            splitter->SetSplitMode(wxSPLIT_HORIZONTAL);


            auto *file_list_box = new wxListBox(splitter, 201);
            file_list_box->Append("File1");
            file_list_box->Append("File1");
            file_list_box->Append("File2");
            file_list_box->Append("File3");

            auto *template_list = make_template_list(splitter, device);

            splitter->SplitVertically(file_list_box, template_list);
            frame->Show();
            break;
        }

        case wxID_CANCEL :
            Exit();

        default : /* ignore or handle error */ ;
            auto msg = string_format("Unexpected button pushed [%d]", dialog_return_value);
            throw std::runtime_error(msg);
    };

    return true;
}

wxIMPLEMENT_APP(reloader_app);
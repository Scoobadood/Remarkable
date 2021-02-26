//
// Created by Dave Durbin (Old) on 19/2/21.
//

#include <wx/splitter.h>
#include <wx/listctrl.h>
#include "reloader_app.h"
#include "reloader_connect_dlg.h"
#include "template_list_ctrl.h"

template<typename ... Args>
std::string string_format(const std::string &format, Args ... args) {
    int size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

wxListView * make_template_list(wxWindow *parent, remarkable * const device) {

    auto list = new template_list_ctrl(parent);
    list->set_templates(device->get_installed_templates());
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
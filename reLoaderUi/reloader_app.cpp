//
// Created by Dave Durbin (Old) on 19/2/21.
//

#include <wx/splitter.h>
#include "reloader_app.h"
#include "reloader_connect_dlg.h"

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args ) {
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

bool reloader_app::OnInit() {
    auto *connect_dialog = new reloader_connect_dlg("","");
    auto dialog_return_value = connect_dialog->ShowModal();
    // You do not have cancel button so only check wxYES and wxNO
    switch( dialog_return_value) {
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

            auto *template_list_box = new wxListBox(splitter, 401);
            for (const auto &tplate : device->get_installed_templates()) {
                template_list_box->Append(tplate.get_name());
            }

            splitter->SplitVertically(file_list_box, template_list_box);
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
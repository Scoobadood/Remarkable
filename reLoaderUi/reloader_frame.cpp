//
// Created by Dave Durbin (Old) on 19/2/21.
//

#include "reloader_frame.h"

enum {
    ID_Hello = 1
};


reloader_frame::reloader_frame() : wxFrame(NULL, wxID_ANY, "Hello World") {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &reloader_frame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &reloader_frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &reloader_frame::OnExit, this, wxID_EXIT);
}

void reloader_frame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void reloader_frame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void reloader_frame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}

//
// Created by Dave Durbin (Old) on 19/2/21.
//

#ifndef REMARKABLE_RELOADER_FRAME_H
#define REMARKABLE_RELOADER_FRAME_H


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class reloader_frame : public wxFrame {
public:
    reloader_frame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};



#endif //REMARKABLE_RELOADER_FRAME_H

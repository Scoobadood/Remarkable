//
// Created by Dave Durbin (Old) on 19/2/21.
//

#ifndef REMARKABLE_RELOADER_APP_H
#define REMARKABLE_RELOADER_APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class reloader_app : public wxApp {
public:
    virtual bool OnInit();
};

#endif //REMARKABLE_RELOADER_APP_H

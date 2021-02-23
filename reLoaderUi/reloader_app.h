//
// Created by Dave Durbin (Old) on 19/2/21.
//

#ifndef REMARKABLE_RELOADER_APP_H
#define REMARKABLE_RELOADER_APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <remarkable/remarkable.h>

class reloader_app : public wxApp {
    std::string host;
    std::string password;

    remarkable * device;
public:
    bool OnInit() override;
};

#endif //REMARKABLE_RELOADER_APP_H

//
// Created by Dave Durbin (Old) on 19/2/21.
//

#include "reloader_app.h"
#include "reloader_frame.h"

bool reloader_app::OnInit() {
    reloader_frame *frame = new reloader_frame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(reloader_app);


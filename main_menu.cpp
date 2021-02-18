//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "main_menu.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"
#include "main.h"
#include "install_menu.h"
#include "uninstall_menu.h"
#include "edit_menu.h"

void
main_menu(void *data) {
    using namespace std;

    cli_menu main{"Main",
                  vector<cli_menu_option>{
                          cli_menu_option{'a', "Install templates", install_menu},
                          cli_menu_option{'b', "Delete templates", uninstall_menu},
                          cli_menu_option{'c', "Change templates", edit_menu},
                          cli_menu_option{'x', "Exit", nullptr}
                  }
    };
    main.execute(cout, cin, data);
}
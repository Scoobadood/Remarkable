//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "edit_menu.h"
#include "main.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"

#include <vector>

void edit_menu(void *adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    auto installed_templates = ad->device.get_installed_templates();

    vector<cli_menu_option> options;
    int i = 0;
    for (const auto &templ : installed_templates) {
        options.emplace_back('a' + i, templ.get_name(), nullptr);
        i++;
    }
    options.emplace_back('a' + i, "Exit to main cli_menu", nullptr);
    cli_menu menu{"Change Templates", options};
    menu.execute(cout, cin, ad);
}



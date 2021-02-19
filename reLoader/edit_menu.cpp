//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "edit_menu.h"
#include "main.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"

#include <vector>

void
do_edit_template(void *data) {
    using namespace std;
    rm_template *tmpl_ptr = (rm_template *) data;
    cout << "Editing " << tmpl_ptr->get_name() << endl;
}

void edit_menu(void *adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    auto installed_templates = ad->device.get_installed_templates();

    vector<cli_menu_option> options;
    char c = 'a';
    for (auto &templ : installed_templates) {
        if( !templ.is_built_in()) {
            options.emplace_back(c++, templ.get_name(), do_edit_template, &templ);
        }
    }
    options.emplace_back(c, "Exit to main cli_menu", nullptr, nullptr);
    cli_menu menu{"Change Templates", options};
    menu.execute(cout, cin, adp);
}



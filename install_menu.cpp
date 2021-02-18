//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "install_menu.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"

void do_install_template(void * data) {
    using namespace std;
    template_files * tf_ptr = (template_files *) data;
    cout << "Installing " << tf_ptr->file_stem << endl;
}

void install_menu(void *data) {
    using namespace std;

    auto app_data_ptr = (app_data *) data;

    vector<cli_menu_option> options;
    char c = 'a';
    options.reserve(app_data_ptr->candidates.size());
    for (auto &candidate : app_data_ptr->candidates) {
        options.emplace_back(c++, candidate.file_stem, do_install_template, &candidate);
    }
    options.emplace_back(c, "Exit to main cli_menu", nullptr, nullptr);
    cli_menu menu{"Install Templates", options};
    menu.execute(cout, cin, data);
}


//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "install_menu.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"

void do_install_template(const rm_template & tplate) {
    using namespace std;
    cout << "Installing " << tplate.get_name() << endl;
}

void install_menu(void *data) {
    using namespace std;

    auto app_data_ptr = (app_data *) data;
    vector<cli_menu_option> options;
    int i = 0;
    options.reserve(app_data_ptr->candidates.size());
    for (const auto &candidate : app_data_ptr->candidates) {
        options.emplace_back('a' + i, candidate.file_stem, nullptr);
        i++;
    }
    options.emplace_back('a' + i, "Exit to main cli_menu", nullptr);
    cli_menu menu{"Install Templates", options};
    menu.execute(cout, cin, data);
}


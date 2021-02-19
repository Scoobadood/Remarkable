//
// Created by Dave Durbin (Old) on 18/2/21.
//

#include "install_menu.h"
#include "menu/cli_menu.h"
#include "menu/cli_menu_option.h"

std::string collect_template_name(const std::string &default_name) {
    using namespace std;

    cout << "Enter template name [" << default_name << "] :";
    std::string name;
    cin >> name;
    if (name.empty()) {
        name = default_name;
    }
    return name;
}

bool collect_landscape_setting() {
    using namespace std;

    cout << "Is this a landscape template? [n]";
    std::string response;
    cin >> response;
    bool landscape = false;
    if (!response.empty()) {
        auto c = response[0];
        landscape = ((c == 'y') || (c == 'Y'));
    }
    return landscape;
}

// TODO: We must allow the user to select from the superset of all categories or enter a new one.
std::set<std::string> collect_categories(const std::set<std::string> &existing_categories) {
    using namespace std;
    return existing_categories;
}

void do_install_template(void *data) {
    using namespace std;

    auto aod = (app_option_data *) data;
    auto tpl_ptr = (template_files *) aod->option_data;

    cout << "Installing " << tpl_ptr->file_stem << endl;

    auto template_name = collect_template_name(tpl_ptr->file_stem);
    auto template_is_landscape = collect_landscape_setting();
    auto template_categories = collect_categories(aod->device.get_known_categories());

    rm_template tpl{
            template_name,
            tpl_ptr->file_stem,
            "\ue9fe",
            template_categories,
            template_is_landscape};
    aod->device.install_template(tpl);
}

void install_menu(void *data) {
    using namespace std;

    auto app_data_ptr = (app_data *) data;

    vector<cli_menu_option> options;
    char c = 'a';
    options.reserve(app_data_ptr->candidates.size());
    for (auto &candidate : app_data_ptr->candidates) {
        options.emplace_back(c++, candidate.file_stem, do_install_template,
                             new app_option_data(app_data_ptr, &candidate));
    }
    options.emplace_back(c, "Exit to main cli_menu", nullptr, nullptr);
    cli_menu menu{"Install Templates", options};
    menu.execute(cout, cin, data);

    for (auto &option : options) {
        delete (app_option_data *) (option.get_option_data());
    }
}


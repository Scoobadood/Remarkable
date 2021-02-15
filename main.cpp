#define LIBSSH_STATIC 1

#include <any>
#include <vector>
#include <type_traits>

#include "file_utilities.h"
#include "menu.h"
#include "remarkable.h"

struct app_data {
    remarkable device;
    std::vector<template_files> candidates;
public:
    app_data(remarkable device, std::vector<template_files> candidate_files) : device{std::move(device)}, candidates{std::move(candidate_files)}{}
};

void
doInstallMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    vector<menu_option> options;
    int i = 0;
    options.reserve(ad->candidates.size());
    for (const auto &candidate : ad->candidates) {
        options.emplace_back('a' + i, candidate.file_stem, nullptr);
        i++;
    }
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    menu menu{"Install Templates", options};
    menu.execute(ad);
}

void
doDeleteMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    auto installed_templates = ad->device.get_installed_temnplates();

    vector<menu_option> options;
    int i = 0;
    for (const auto &templ : installed_templates) {
        options.emplace_back('a' + i, templ.get_name(), nullptr);
        i++;
    }
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    menu menu{"Delete Templates", options};
    menu.execute(ad);
}

void doChangeMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    auto installed_templates = ad->device.get_installed_temnplates();

    vector<menu_option> options;
    int i = 0;
    for (const auto &templ : installed_templates) {
        options.emplace_back('a' + i, templ.get_name(), nullptr);
        i++;
    }
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    menu menu{"Change Templates", options};
    menu.execute(ad);
}

void doMainMenu(const remarkable& device, const std::vector<template_files>& candidates) {
    using namespace std;

    menu main{"Main",
              vector<menu_option>{
                      menu_option{'1', "Install templates", doInstallMenu},
                      menu_option{'2', "Delete templates", doDeleteMenu},
                      menu_option{'3', "Change templates", doChangeMenu},
                      menu_option{'X', "Exit", nullptr}
              }
    };

    auto *ad = new app_data(device, candidates);
    main.execute(ad);
    delete ad;
}

int main(int argc, char *argv[]) {
    using namespace std;

    const char *host = argv[1];
    const char *password = argv[2];

    remarkable device{host, password};
    auto candidates = find_eligible_templates_in_directory("/Users/dave/Documents/Writing");

    doMainMenu(device, candidates);
}
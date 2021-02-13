#define LIBSSH_STATIC 1

#include <iostream>
#include <any>
#include <string>
#include <utility>
#include <variant>
#include <algorithm>
#include <vector>
#include <set>
#include <type_traits>
#include <optional>
#include <sstream>
#include <limits>
#include <cctype>

#include <libssh/libssh.h>
#include "json/json.h"
#include "scp.h"
#include "page_template.h"
#include "file_utilities.h"

typedef void (* Menu_Processing_Function_Ptr)(void*);

void installTemplate(const std::string &templateName) {}

void deleteTemplate(const std::string &templateName) {}

struct app_data {
    ssh_session session{};
    std::vector<template_files> candidates;
    std::vector<page_template> templates;
};

struct MenuOption {
    const char optionLetter;
    const std::string optionText;
    Menu_Processing_Function_Ptr ptr{};

    MenuOption(char optionLetter,
               std::string optionText,
               Menu_Processing_Function_Ptr ptr) : optionLetter{optionLetter},
               optionText{std::move(optionText)},
               ptr{ptr}{}
};

class Menu {
    const std::string name;
    std::vector<MenuOption> options;
public:
    Menu(std::string name, std::vector<MenuOption> options) : name(std::move(name)), options(std::move(options)) {}

    void doit(void * data) {
        using namespace std;

        bool done = false;
        do {
            cout << name << endl;
            cout << "----------------------" << endl;
            for (const auto &option : options) {
                cout << option.optionLetter << ". " << option.optionText << endl;
            }

            unsigned char selection;

            cin >> selection;
            bool validChoice = false;
            for (const auto &option: options) {
                if (selection == option.optionLetter) {
                    validChoice = true;
                    if (option.ptr == nullptr) {
                        done = true;
                    } else {
                        option.ptr(data);
                    }
                }
            }
            if (!validChoice) {
                cout << "bad selection" << endl;
            }
        } while (!done);
    }
};

const std::set<std::string> BUILTIN_TEMPLATES {
        "Blank",
        "Checklist double",
        "Checklist",
        "Dayplanner",
        "Dots bottom",
        "Dots top",
        "Grid bottom",
        "Grid margin large",
        "Grid margin med",
        "Grid top",
        "Lined bottom",
        "Lined medium",
        "Lined small",
        "Lined top",
        "Margin medium",
        "Margin small",
        "One storyboard 1",
        "One storyboard 2",
        "One storyboard",
        "Two storyboards",
        "Two storyboards",
        "Four storyboards",
        "Four storyboards",
        "Weekplanner US",
        "Weekplanner",
        "Checklist",
        "Cornell",
        "Dayplanner",
        "Dots S bottom",
        "Dots S top",
        "Dots S",
        "Dots large",
        "Dots S",
        "Dots large",
        "Grid bottom",
        "Grid large",
        "Grid medium",
        "Grid small",
        "Grid large",
        "Grid medium",
        "Grid small",
        "Grid margin large",
        "Grid margin med",
        "Grid top",
        "Lined bottom",
        "Lined heading",
        "Lined top",
        "Lined large",
        "Lined medium",
        "Lined small",
        "Margin large",
        "Margin medium",
        "Margin small",
        "US College",
        "US Legal",
        "Weekplanner 1",
        "Weekplanner 2",
        "Weekplanner US",
        "Isometric",
        "Perspective 1",
        "Perspective 2",
        "Calligraphy large",
        "Calligraphy medium",
        "Piano sheet large",
        "Piano sheet medium",
        "Piano sheet small",
        "Calligraphy large",
        "Calligraphy medium",
        "Music",
        "Bass tablature",
        "Guitar chords",
        "Guitar tablature",
        "Piano sheet large",
        "Piano sheet medium",
        "Piano sheet small",
        "Hexagon large",
        "Hexagon medium",
        "Hexagon small",
};
void doInstallMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    vector<MenuOption> options;
    int i = 0;
    options.reserve(ad->candidates.size());
    for (const auto &candidate : ad->candidates) {
        options.emplace_back('a' + i, candidate.file_stem, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Install Templates", options};
    menu.doit(ad);
}

void doDeleteMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    vector<MenuOption> options;
    int i = 0;
    for (const auto &templ : ad->templates) {
        options.emplace_back('a' + i, templ.name, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Delete Templates", options};
    menu.doit(ad);
}

void doChangeMenu(void * adp) {
    using namespace std;

    auto ad = (app_data *) adp;
    vector<MenuOption> options;
    int i = 0;
    for (const auto &templ : ad->templates) {
        options.emplace_back('a' + i, templ.name, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Change Templates", options};
    menu.doit(ad);
}

void doMainMenu(ssh_session session, std::vector<page_template> templates, std::vector<template_files> candidates) {
    using namespace std;

    Menu main{"Main",
              vector<MenuOption>{
                      MenuOption{'1', "Install templates", doInstallMenu},
                      MenuOption{'2', "Delete templates", doDeleteMenu},
                      MenuOption{'3', "Change templates", doChangeMenu},
                      MenuOption{'X', "Exit", nullptr}
              }
    };
    auto *ad = new app_data;
    ad->candidates = std::move(candidates);
    ad->templates = std::move(templates);
    ad->session = session;
    main.doit(ad);
    delete ad;
}

int main(int argc, char *argv[]) {
    using namespace std;

    const char *host = argv[1];
    const char *password = argv[2];
    const char *user_id = "root";

    cout << "Connecting to reMarkable..." << endl;
    ssh_session session = ssh_new();
    if (session == nullptr) {
        cerr << "Failed to start SSH session" << endl;
        exit(-1);
    }

    cout << "Reading list of installed templates..." << endl;
    vector<page_template> page_templates;
    const auto templates = get_templates_json_from_device(session, host, user_id, password);
    for (const auto &t : templates) {
        if(! BUILTIN_TEMPLATES.count(t["name"].asString())) {
            auto pt = page_template::from_json(t);
            page_templates.push_back(pt);
        }
    }

    cout << "Checking local directory..." << endl;
    auto candidates = find_eligible_templates_in_directory("/Users/dave/Documents/Writing");

    doMainMenu(session, page_templates, candidates);

    ssh_free(session);
}
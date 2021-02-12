#define LIBSSH_STATIC 1

#include <iostream>
#include <any>
#include <string>
#include <utility>
#include <variant>
#include <vector>
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

typedef void (*Menu_Processing_Function_Ptr)();

void installTemplate(const std::string &templateName) {}

void deleteTemplate(const std::string &templateName) {}


struct MenuOption {
    const char optionLetter;
    const std::string optionText;
    Menu_Processing_Function_Ptr ptr{};

    MenuOption(char optionLetter, std::string optionText, const Menu_Processing_Function_Ptr ptr) : optionLetter(
            optionLetter),
                                                                                                    optionText(
                                                                                                            std::move(
                                                                                                                    optionText)),
                                                                                                    ptr(ptr) {}
};

class Menu {
    const std::string name;
    std::vector<MenuOption> options;
public:
    Menu(std::string name, std::vector<MenuOption> options) : name(std::move(name)), options(std::move(options)) {}

    void doit() {
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
                        option.ptr();
                    }
                }
            }
            if (!validChoice) {
                cout << "bad selection" << endl;
            }
        } while (!done);
    }
};

void doInstallMenu() {
    using namespace std;

    vector<string> templateNames{"one", "two", "three"};
    vector<MenuOption> options;
    int i = 0;
    options.reserve(templateNames.size());
    for (const auto &name : templateNames) {
        options.emplace_back('a' + i, name, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Install Templates", options};
    menu.doit();
}

void doDeleteMenu() {
    using namespace std;

    vector<string> templateNames{"one", "two", "three"};
    vector<MenuOption> options;
    int i = 0;
    for (const auto &name : templateNames) {
        options.emplace_back('a' + i, name, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Delete Templates", options};
    menu.doit();
}

void doChangeMenu() {
    using namespace std;

    vector<string> templateNames{"one", "two", "three"};
    vector<MenuOption> options;
    int i = 0;
    for (const auto &name : templateNames) {
        options.emplace_back('a' + i, name, nullptr);
        i++;
    };
    options.emplace_back('a' + i, "Exit to main menu", nullptr);
    Menu menu{"Change Templates", options};
    menu.doit();
}

void doMainMenu() {
    using namespace std;

    Menu main{"Main",
              vector<MenuOption>{
                      MenuOption{'1', "Install templates", doInstallMenu},
                      MenuOption{'2', "Delete templates", doDeleteMenu},
                      MenuOption{'3', "Change templates", doChangeMenu},
                      MenuOption{'X', "Exit", nullptr}
              }
    };
    main.doit();
}

int main(int argc, char *argv[]) {
    using namespace std;

    const char *host = argv[1];
    const char *password = argv[2];
    const char *user_id = "root";


    doMainMenu();


    ssh_session session = ssh_new();
    if (session == nullptr) {
        cerr << "Failed to start SSH session" << endl;
        exit(-1);
    }

    auto candidates = find_eligible_templates_in_directory("/Users/dave/Documents/Writing");

    const auto templates = get_templates_json_from_device(session, host, user_id, password);
    for (const auto &t : templates) {
        auto pt = page_template::from_json(t);
        std::cout << pt.name << std::endl;
    }

    ssh_free(session);
}
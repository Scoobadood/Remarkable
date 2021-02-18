//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include <iostream>
#include <utility>
#include "cli_menu.h"

cli_menu::cli_menu(
        std::string name,
        std::vector<cli_menu_option> options) : name{std::move(name)}, options{std::move(options)} {
}

void
cli_menu::execute(std::ostream &output, std::istream &input, void *data) const {
    using namespace std;

    bool done = false;
    do {
        output << name << endl;
        output << "----------------------" << endl;
        for (const auto &option : options) {
            output << option.get_option_letter() << ". " << option.get_option_text() << endl;
        }

        unsigned char selection;
        input >> selection;

        const cli_menu_option * selected_option = nullptr;
        for (const auto &option: options) {
            if (selection == option.get_option_letter()) {
                selected_option = &option;
            }
        }

        if (selected_option == nullptr) {
            output << "bad selection" << endl;
        } else {
            done = selected_option->execute();
        }
    } while (!done);
}
//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include <iostream>
#include "cli_menu.h"

cli_menu::cli_menu(
        const std::string & name,
        const std::vector<cli_menu_option> & options) : name{name}, options{options}{
}

void
cli_menu::execute(std::ostream & output, std::istream & input, void *data) const {
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
        bool validChoice = false;
        for (const auto &option: options) {
            if (selection == option.get_option_letter()) {
                validChoice = true;
                auto menu_function =option.get_menu_function();
                if ( menu_function == nullptr) {
                    done = true;
                } else {
                    menu_function(data);
                }
            }
        }
        if (!validChoice) {
            output << "bad selection" << endl;
        }
    } while (!done);
}
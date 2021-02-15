//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include "menu.h"

void
menu::execute(void *data) {
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
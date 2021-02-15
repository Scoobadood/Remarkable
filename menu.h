//
// Created by Dave Durbin (Old) on 15/2/21.
//

#ifndef REMARKABLE_MENU_H
#define REMARKABLE_MENU_H

#include <string>
#include <iostream>
#include <vector>

typedef void (*menu_processing_function_ptr)(void *);

struct menu_option {
    const char optionLetter;
    const std::string optionText;
    menu_processing_function_ptr ptr{};

    menu_option(char optionLetter,
                std::string optionText,
                menu_processing_function_ptr ptr) : optionLetter{optionLetter},
                                                    optionText{std::move(optionText)},
                                                    ptr{ptr} {}
};

class menu {
    const std::string name;
    std::vector<menu_option> options;

public:
    menu(std::string name, std::vector<menu_option> options) : name(std::move(name)), options(std::move(options)) {}

    void execute(void *data);
};


#endif //REMARKABLE_MENU_H

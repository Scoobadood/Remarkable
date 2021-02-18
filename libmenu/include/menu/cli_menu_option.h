//
// Created by Dave Durbin (Old) on 15/2/21.
//

#pragma once

#include <string>

typedef void (*menu_processing_function_ptr)(void *);

class cli_menu_option {
    const char option_letter;
    const std::string option_text;
    const menu_processing_function_ptr ptr;
    void * option_data;

public:
    cli_menu_option(
            char optionLetter,
            std::string optionText,
            menu_processing_function_ptr ptr,
            void * option_data);

    char get_option_letter() const { return option_letter;}
    const std::string & get_option_text() const { return option_text;}
    /**
     * @return True if this option should quit the menu.
     */
    bool execute() const;
};

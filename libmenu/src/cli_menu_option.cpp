//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include "cli_menu_option.h"

#include <utility>

cli_menu_option::cli_menu_option(
        char option_letter,
        std::string option_text,
        const menu_processing_function_ptr ptr,
        void *option_data
) : option_letter{option_letter},
    option_text{std::move(option_text)},
    ptr{ptr},
    option_data{option_data} {
}


bool
cli_menu_option::execute() const {
    if (ptr == nullptr) {
        return true;
    }
    ptr(option_data);
    return false;
}
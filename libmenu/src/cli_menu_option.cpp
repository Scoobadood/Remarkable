//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include "cli_menu_option.h"

cli_menu_option::cli_menu_option(
        char option_letter,
        const std::string &option_text,
        const menu_processing_function_ptr ptr) : option_letter{option_letter}, option_text{option_text}, ptr{ptr} {
}
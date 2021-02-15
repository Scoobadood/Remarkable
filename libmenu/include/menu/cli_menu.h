//
// Created by Dave Durbin (Old) on 15/2/21.
//

#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "cli_menu_option.h"

class cli_menu {
    const std::string name;
    const std::vector<cli_menu_option> options;

public:
    cli_menu(const std::string & name, const std::vector<cli_menu_option> & options);

    void execute(std::ostream & output, std::istream & input, void *data) const;

    const std::string & get_name() const {return name;}
};
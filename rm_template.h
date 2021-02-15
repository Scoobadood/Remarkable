//
// Created by Dave Durbin (Old) on 9/2/21.
//

#ifndef REMARKABLE_RM_TEMPLATE_H
#define REMARKABLE_RM_TEMPLATE_H

#include <string>
#include <vector>
#include <set>
#include <json/json.h>

class rm_template {
    const std::string name;
    const std::string file_name;
    const std::string icon_code;
    const std::vector<std::string> categories;
    const bool landscape;

public:
    rm_template(
            std::string name,
            std::string file_name,
            std::string icon_code,
            std::vector<std::string> categories,
            bool landscape
    );

    static rm_template from_json(const Json::Value &value);

    static bool is_built_in(std::string & template_name);

    const std::string &get_name() const;

    const std::string &get_file_name() const;

    const std::string &get_icon_code() const;

    const std::vector<std::string> &get_categories() const;

    const bool is_landscape() const;

};

#endif //REMARKABLE_RM_TEMPLATE_H

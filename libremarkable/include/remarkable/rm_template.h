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
    // Template name
    std::string name;

    // File name stem for PNG and SVG files
    std::string file_name;

    // Icon code; new templates default to blank
    std::string icon_code;

    // The set of categories to which this template belongs
    std::set<std::string> categories;

    // True if this is a landscape template
    bool landscape;

    // True iff this is a built in template, otherwise false.
    bool built_in;

    static bool is_built_in_template_name(const std::string &template_name);

public:
    rm_template(
            std::string name,
            std::string file_name,
            std::string icon_code,
            std::set<std::string> categories,
            bool landscape
    );

    explicit rm_template(const Json::Value &value);

    /**
     * A template is 'equal' to a string if the template name matches the string.
     */
    bool operator==(const std::string & other) {
        return name == other;
    }

    /**
     * Convert to JSON.
     */
    Json::Value to_json() const;

    bool is_built_in() const;

    const std::string &get_name() const;

    const std::string &get_file_name() const;

    const std::string &get_icon_code() const;

    const std::set<std::string> & get_categories() const;

    bool is_landscape() const;
};

#endif //REMARKABLE_RM_TEMPLATE_H

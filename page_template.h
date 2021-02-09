//
// Created by Dave Durbin (Old) on 9/2/21.
//

#ifndef REMARKABLE_PAGE_TEMPLATE_H
#define REMARKABLE_PAGE_TEMPLATE_H

#include <string>
#include <vector>
#include "scp.h"

struct page_template {
    const std::string name;
    const std::string file_name;
    const std::string icon_code;
    const std::vector<std::string> categories;
    const bool landscape;

    page_template(
            std::string name,
            std::string file_name,
            std::string icon_code,
            std::vector<std::string> categories,
            bool landscape
    );
    static page_template from_json(const Json::Value &value);
};


#endif //REMARKABLE_PAGE_TEMPLATE_H

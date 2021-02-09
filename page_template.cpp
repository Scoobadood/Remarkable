//
// Created by Dave Durbin (Old) on 9/2/21.
//

#include "page_template.h"
#include "scp.h"

#include <utility>

page_template page_template::from_json(const Json::Value &value) {
    using namespace std;

    auto name = value["name"].asString();
    auto file_name = value["filename"].asString();
    auto icon_code = value["iconCode"].asString();
    auto landscape = value["landscape"].asBool();
    auto categories_json = value["categories"];
    vector<string> categories{};
    for( const auto& cat : categories_json) {
        categories.push_back(cat.asString());
    }
    page_template pt{
        name, file_name, icon_code, categories, landscape
    };
    return pt;
}

page_template::page_template(std::string name,
                             std::string file_name,
                             std::string icon_code,
                             std::vector<std::string> categories,
                             const bool landscape) :
        name{std::move(name)},
        file_name{std::move(file_name)},
        icon_code{std::move(icon_code)},
        categories{std::move(categories)},
        landscape{landscape} {}

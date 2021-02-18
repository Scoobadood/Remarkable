//
// Created by Dave Durbin (Old) on 9/2/21.
//

#include "rm_template.h"

#include <utility>

static std::set<std::string> get_builtin_names() {
    static std::set<std::string> BUILTIN_TEMPLATES = {
            "Blank",
            "Checklist double",
            "Checklist",
            "Dayplanner",
            "Dots bottom",
            "Dots top",
            "Grid bottom",
            "Grid margin large",
            "Grid margin med",
            "Grid top",
            "Lined bottom",
            "Lined medium",
            "Lined small",
            "Lined top",
            "Margin medium",
            "Margin small",
            "One storyboard 1",
            "One storyboard 2",
            "One storyboard",
            "Two storyboards",
            "Two storyboards",
            "Four storyboards",
            "Four storyboards",
            "Weekplanner US",
            "Weekplanner",
            "Checklist",
            "Cornell",
            "Dayplanner",
            "Dots S bottom",
            "Dots S top",
            "Dots S",
            "Dots large",
            "Dots S",
            "Dots large",
            "Grid bottom",
            "Grid large",
            "Grid medium",
            "Grid small",
            "Grid large",
            "Grid medium",
            "Grid small",
            "Grid margin large",
            "Grid margin med",
            "Grid top",
            "Lined bottom",
            "Lined heading",
            "Lined top",
            "Lined large",
            "Lined medium",
            "Lined small",
            "Margin large",
            "Margin medium",
            "Margin small",
            "US College",
            "US Legal",
            "Weekplanner 1",
            "Weekplanner 2",
            "Weekplanner US",
            "Isometric",
            "Perspective 1",
            "Perspective 2",
            "Calligraphy large",
            "Calligraphy medium",
            "Piano sheet large",
            "Piano sheet medium",
            "Piano sheet small",
            "Calligraphy large",
            "Calligraphy medium",
            "Music",
            "Bass tablature",
            "Guitar chords",
            "Guitar tablature",
            "Piano sheet large",
            "Piano sheet medium",
            "Piano sheet small",
            "Hexagon large",
            "Hexagon medium",
            "Hexagon small",
    };
    return BUILTIN_TEMPLATES;
}

bool
rm_template::is_built_in_template_name(const std::string &template_name){
    return (get_builtin_names().count(template_name) > 0);
}

rm_template::rm_template(const Json::Value &value) {
    using namespace std;

    name = value["name"].asString();
    file_name = value["filename"].asString();
    icon_code = value["iconCode"].asString();

    // Some built in templates have "true" . Grrr...
    auto landscape_value = value["landscape"];
    landscape = false;
    if (landscape_value.isBool()) {
        landscape = landscape_value.asBool();
    } else if (landscape_value.isString()) {
        landscape = (landscape_value.asString() == "true");
    }
    auto categories_json = value["categories"];
    for (const auto &cat : categories_json) {
        categories.emplace(cat.asString());
    }
    built_in = is_built_in_template_name(name);
}

rm_template::rm_template(std::string name,
                         std::string file_name,
                         std::string icon_code,
                         std::set<std::string> categories,
                         const bool landscape) :
        name{std::move(name)},
        file_name{std::move(file_name)},
        icon_code{std::move(icon_code)},
        categories{std::move(categories)},
        landscape{landscape} {
    built_in = is_built_in_template_name(name);
}

Json::Value
rm_template::to_json() const {
    Json::Value json;
    json["name"] = name;
    json["file_name"] = file_name;
    json["icon_code"] = icon_code;
    if( landscape) {
        json["landscape"] = true;
    }
    for( const auto& category : categories) {
        json["categories"].append(category);
    }
    return json;
}

bool rm_template::is_built_in() const {
    return built_in;
}

const std::string &rm_template::get_name() const {
    return name;
}

const std::string &rm_template::get_file_name() const {
    return file_name;
}

const std::string &rm_template::get_icon_code() const {
    return icon_code;
}

const std::set<std::string> &rm_template::get_categories() const {
    return categories;
}

bool rm_template::is_landscape() const {
    return landscape;
}

//
// Created by Dave Durbin (Old) on 9/2/21.
//

#ifndef REMARKABLE_FILE_UTILITIES_H
#define REMARKABLE_FILE_UTILITIES_H

#include <string>
#include <utility>
#include <vector>

struct template_files {
    bool is_png_found;
    bool is_svg_found;
    const std::string file_stem;

    template_files(bool is_png_found, bool is_svg_found, std::string file_stem) :
            is_png_found{is_png_found}, is_svg_found{is_svg_found}, file_stem{std::move(file_stem)} {}

    bool operator<(template_files t) {
        return file_stem < t.file_stem;
    }
};

std::vector<template_files> find_eligible_templates_in_directory(const std::string &directory);

#endif //REMARKABLE_FILE_UTILITIES_H

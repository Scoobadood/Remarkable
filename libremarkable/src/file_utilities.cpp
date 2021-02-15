//
// Created by Dave Durbin (Old) on 9/2/21.
//

#include <set>
#include "file_utilities.h"
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<template_files>
find_eligible_templates_in_directory(const std::string &directory) {

    // TODO: Check that directpry exists
    using namespace std;
    namespace fs = std::__fs::filesystem;

    string p = (directory.size() > 0 &&  directory.at(0) == '/')
            ? directory
            : fs::current_path().append(directory).string();

    vector<template_files> candidates;

    vector<string> png_files;
    vector<string> svg_files;

    // Get file listing for SVG and PNG files
    for (const auto &entry : fs::directory_iterator(p)) {
        if (entry.is_regular_file() && entry.path().has_extension()) {
            const auto & p = entry.path();
            auto ext = p.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            const auto & fn = p.filename().replace_extension().string();
            if (ext == ".png") {
                png_files.push_back(fn);
            } else if (ext == ".svg") {
                svg_files.push_back(fn);
            }
        }
    }

    // Candidates are names which appear in the intersection
    vector<string> intersection;
    sort(png_files.begin(), png_files.end());
    sort(svg_files.begin(), svg_files.end());
    set_intersection(begin(png_files), end(png_files),
                               begin(svg_files), end(svg_files),
                               back_inserter(intersection));
    candidates.reserve(intersection.size());
    for (const auto &v : intersection) {
        candidates.emplace_back(true, true, v);
    }
    return candidates;
}

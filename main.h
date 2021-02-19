//
// Created by Dave Durbin (Old) on 18/2/21.
//

#ifndef REMARKABLE_MAIN_H
#define REMARKABLE_MAIN_H

#include "remarkable/remarkable.h"
#include "remarkable/file_utilities.h"

struct app_data {
    remarkable device;
    std::vector<template_files> candidates;
public:
    app_data(remarkable device, std::vector<template_files> candidate_files)
            : device{std::move(device)},
              candidates{std::move(candidate_files)} {
    }
};

struct app_option_data {
    remarkable device;
    std::vector<template_files> candidates;
    void *option_data;
public:
    app_option_data(const app_data * ad, void *option_data)
            : device{ad->device},
              candidates{ad->candidates},
              option_data{option_data} {
    }
};

#endif //REMARKABLE_MAIN_H

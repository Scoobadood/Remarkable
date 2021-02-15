//
// Created by Dave Durbin (Old) on 15/2/21.
//

#ifndef REMARKABLE_TESTMENU_H
#define REMARKABLE_TESTMENU_H

#include <gtest/gtest.h>

class test_menu  : public ::testing::Test {
public:
    void set_up( );
    void tear_down();
};

#endif //REMARKABLE_TESTMENU_H

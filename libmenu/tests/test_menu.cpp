//
// Created by Dave Durbin (Old) on 15/2/21.
//

#include "test_menu.h"
#include "gtest/gtest.h"
#include <menu/cli_menu_option.h>

void test_menu::set_up() {}
void test_menu::tear_down() {}

TEST(test_menu, constructor_throws_with_null_function_ptr) {

EXPECT_DEATH(
        cli_menu_option('c', "Close", nullptr),
        "PARSE ERROR: Argument: -x*");
}


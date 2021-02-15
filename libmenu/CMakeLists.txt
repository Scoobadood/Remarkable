add_library(
		menu STATIC
		include/menu/cli_menu.h
		include/menu/cli_menu_option.h
		src/cli_menu.cpp
		src/cli_menu_option.cpp
)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(
		menu
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE
		include/menu
)

add_executable(
		test_menu
		tests/main.cpp
		tests/test_menu.cpp
		tests/test_menu.h
)

target_link_libraries(
		test_menu
		menu
		gtest
		gmock
)

add_test(
		NAME SkewSymmetricMatrixShouldBeCorrect
		COMMAND testGeom --gtest_filter=SkewSymmetricMatrixShouldBeCorrect
)
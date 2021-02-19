cmake_minimum_required(VERSION 3.18)

project(remarkable
		VERSION 0.1
		LANGUAGES CXX )

set(PROJECT_VERSION 0.2)

set(GIT_VERSION_STRING "git version 2.20.1 (Apple Git-117)")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(CMAKE_CXX_FLAGS "")

# Write targets to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)

set(CMAKE_CXX_STANDARD 14)

# Need spdlog
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
		"${CMAKE_BINARY_DIR}/googletest-build")

# Enable Test
enable_testing( )

add_executable(reLoader
		reLoader/main.h reLoader/main.cpp
		reLoader/main_menu.h reLoader/main_menu.cpp
		reLoader/uninstall_menu.h reLoader/uninstall_menu.cpp
		reLoader/install_menu.h reLoader/install_menu.cpp
		reLoader/edit_menu.h reLoader/edit_menu.cpp
)

target_link_libraries(reLoader
		menu
		remarkable
)

add_executable(reLoaderUi
		reLoaderUi/reloader_app.cpp reLoaderUi/reloader_app.h
		reLoaderUi/reloader_frame.cpp reLoaderUi/reloader_frame.h
		)

target_link_libraries(reLoaderUi
		wx::net wx::core wx::base
		remarkable)

add_subdirectory( libmenu )
add_subdirectory( libremarkable )
add_subdirectory( reLoaderUi )

cmake_minimum_required(VERSION 3.18)
project(Remarkable)

set(CMAKE_CXX_STANDARD 14)

find_package(libssh REQUIRED)
if (NOT TARGET ssh)
	add_library(ssh SHARED IMPORTED)
	set_target_properties(ssh
			PROPERTIES
			​          IMPORTED_LOCATION "${LIBSSH_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${LIBSSH_INCLUDE_DIR}")
endif()

#find_package(jsoncpp REQUIRED)

add_executable(Remarkable main.cpp scp.cpp scp.h)

find_library(shaz jsoncpp REQUIRED PATHS /usr/local/lib)

message(STATUS ${shaz})
target_link_libraries(Remarkable
		ssh
		${shaz})


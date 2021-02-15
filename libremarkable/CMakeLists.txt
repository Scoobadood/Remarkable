add_library(
		remarkable SHARED
		include/remarkable/remarkable.h
		include/remarkable/file_utilities.h
		include/remarkable/rm_template.h
		src/file_utilities.cpp
		src/remarkable.cpp
		src/rm_template.cpp
)

find_package(libssh REQUIRED)
if (NOT TARGET ssh)
	add_library(ssh STATIC IMPORTED)
	set_target_properties(ssh
			PROPERTIES
			​ IMPORTED_LOCATION "${LIBSSH_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${LIBSSH_INCLUDE_DIR}")
endif ()


# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(
		remarkable
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE
		include/remarkable)

#add_executable(
#		testRemarkable
#		tests/main.cpp
#		tests/TestRemarkable.cpp tests/TestRemarkable.h
#)
#
#target_link_libraries(
#		testRemarkable
#		remarkable
#		gtest
#		gmock
#)

find_library(shaz jsoncpp REQUIRED PATHS /usr/local/lib)

message(STATUS ${shaz})
target_link_libraries(
		remarkable
		ssh
		${shaz}
		menu
)

add_test(
		NAME SkewSymmetricMatrixShouldBeCorrect
		COMMAND testGeom --gtest_filter=SkewSymmetricMatrixShouldBeCorrect
)
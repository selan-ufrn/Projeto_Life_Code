cmake_minimum_required(VERSION 3.5)
project (GLife VERSION 1.0.0 LANGUAGES CXX )

#=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Appending to existing flags the correct way (two methods)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
# string(APPEND CMAKE_CXX_FLAGS " -Wall -Werror")


#=== ADDING SUBDIRECTORIES ===#
# The add_subdirectory() command allows a project to bring another directory into the build. That
# directory must have its own CMakeLists.txt file which will be processed at the point where
# add_subdirectory() is called and a corresponding directory will be created for it in the project’s
set( LODEPNG_LIB "lodepng" )
set( CANVAS_LIB "canvas" )
set( TIP_LIB "tip" )
set( APP_NAME "glife")
# include_directories(lib)
add_subdirectory(lib) # This will ask this lib to be build
add_subdirectory(src)

# * CMAKE_SOURCE_DIR
# The top-most directory of the source tree (i.e. where the top-most CMakeLists.txt file resides).
# This variable never changes its value.

# * CMAKE_BINARY_DIR
# The top-most directory of the build tree. This variable never changes its value.

# * CMAKE_CURRENT_SOURCE_DIR
# The directory of the CMakeLists.txt file currently being processed by CMake. It is updated each
# time a new file is processed as a result of an add_subdirectory() call and is restored back again
# when processing of that directory is complete.

# * CMAKE_CURRENT_BINARY_DIR
# The build directory corresponding to the CMakeLists.txt file currently being processed by
# CMake. It changes for every call to add_subdirectory() and is restored again when
# add_subdirectory() returns.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(SIMDB)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

# root dir
aux_source_directory(. DIR_MAIN)

# debug mode
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# add sub_dir
include_directories(.)

# compile
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(db ${DIR_MAIN})
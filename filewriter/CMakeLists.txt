cmake_minimum_required(VERSION 2.6)

project(filewriter)

include_directories(../include)

aux_source_directory(. SRC_LIST)

add_library(filewriter SHARED ${SRC_LIST})

#set_target_properties(filewriter PROPERTIES VERSION 1.2 SOVERSION 1)


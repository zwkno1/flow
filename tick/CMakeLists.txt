cmake_minimum_required(VERSION 2.6)

project(tick)

include_directories(../include)

aux_source_directory(. SRC_LIST)

add_library(tick SHARED ${SRC_LIST})

#set_target_properties(filewriter PROPERTIES VERSION 1.2 SOVERSION 1)


cmake_minimum_required(VERSION 2.6)

project(flow)

add_subdirectory(manager)

list(APPEND SLOT_DIRS filewriter tick)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/slots)

foreach(dir ${SLOT_DIRS})
	add_subdirectory(${dir})
endforeach()
	
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/slots)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/config.xml)


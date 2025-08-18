cmake_minimum_required(VERSION 2.8)

project (WejseEngine2D)

include_directories(${CMAKE_CURRENT_LIST_DIR}/CoreLib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/freetype)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Assets)
include_directories(${CMAKE_CURRENT_LIST_DIR}/imgui)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/FMOD)




file(GLOB_RECURSE WejseEngine2D_source_files 
    ${CMAKE_CURRENT_LIST_DIR}/WejseEngine2D/*.[ch]pp
    ${CMAKE_CURRENT_LIST_DIR}/WejseEngine2D/*.h
)
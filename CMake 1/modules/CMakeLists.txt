cmake_minimum_required(VERSION 3.10)
project(modules)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/A1
    ${CMAKE_CURRENT_SOURCE_DIR}/A2
    ${CMAKE_CURRENT_SOURCE_DIR}/M1
    ${CMAKE_CURRENT_SOURCE_DIR}/M2
)
add_subdirectory(A1)
add_subdirectory(A2)
add_subdirectory(M1)
add_subdirectory(M2)
add_library(modules INTERFACE)
target_link_libraries(modules INTERFACE A1 A2 M1 M2)
#link_libraries(A1 A2 M1 M2)
#[[
set(Modules_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include CACHE PATH "Modules_INCLUDE_DIRS")
file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(m ${subdirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${m})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${m}/CMakeLists.txt)
            add_subdirectory(${m})
        endif()
    endif()
endforeach()
]]#
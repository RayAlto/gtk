cmake_minimum_required(VERSION 3.1)
project(
  ra-gtk
  VERSION 1.0
  LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

set(RA_GTK_SRCS
  ${CMAKE_CURRENT_LIST_DIR}/src/image.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/message_bubble.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/red_dot.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/runner.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/util.cc)
set(RA_GTK_INCLUDES
  ${CMAKE_CURRENT_LIST_DIR}/include/)
set(RA_GTK_RESOURCES
  ${CMAKE_CURRENT_LIST_DIR}/res/resources.cc)

add_library(ra-gtk STATIC ${RA_GTK_SRCS})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${RA_GTK_INCLUDES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

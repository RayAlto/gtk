cmake_minimum_required(VERSION 3.1)
project(
  ra-gtk-test
  VERSION 1.0
  LANGUAGES CXX)

if(WIN32)
  set(GUI_OPTION WIN32)
elseif(APPLE)
  set(GUI_OPTION MACOSX_BUNDLE)
endif()

# macro(ra_test_add target_name target_src)
#   add_executable(${target_name} ${GUI_OPTION} ${target_src} ${RA_GTK_RESOURCES}) 
#   set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 17)
#   target_include_directories(${target_name} PRIVATE ${RA_GTK_INCLUDES})
#   target_link_libraries(${target_name} PRIVATE ra-gtk)
# endmacro()

add_executable(test ${GUI_OPTION} window.cc window.h test.cc ${RA_GTK_RESOURCES}) 
set_property(TARGET test PROPERTY CXX_STANDARD 17)
target_include_directories(test PRIVATE ${RA_GTK_INCLUDES})
target_link_libraries(test PRIVATE ra-gtk)

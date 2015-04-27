cmake_minimum_required(VERSION 2.8)

include_directories(
  STEP_SDK
  ${CMAKE_CURRENT_SOURCE_DIR})
add_library(STEP_mid.objs OBJECT
  STEP_mid.cpp
  SMFUtil.cpp)
set(rcfile "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
configure_file(resource.rc.in ${rcfile})

if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static")
  add_library(STEP_mid.lib STATIC $<TARGET_OBJECTS:STEP_mid.objs>)
  set(expfile "${CMAKE_CURRENT_BINARY_DIR}/STEP_mid.exp.obj")
  add_custom_command(
    OUTPUT ${expfile}
    COMMAND ${CMAKE_DLLTOOL} -e ${expfile} --add-stdcall-underscore $<TARGET_FILE:STEP_mid.lib>
    DEPENDS STEP_mid.lib
  )
  add_library(STEP_mid MODULE
    ${rcfile}
    STEP_SDK/STEPlugin.cpp
    ${expfile}
  )
  target_link_libraries(STEP_mid STEP_mid.lib)
else()
  add_library(STEP_mid MODULE
    ${rcfile}
    STEP_SDK/STEPlugin.cpp
    $<TARGET_OBJECTS:STEP_mid.objs>
  )
endif()

set_target_properties(STEP_mid PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS STEP_mid LIBRARY DESTINATION .)

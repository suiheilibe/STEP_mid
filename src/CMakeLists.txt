cmake_minimum_required(VERSION 2.8.11)

include_directories(
  STEP_SDK
  ${CMAKE_CURRENT_SOURCE_DIR})
set(rcfile "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
configure_file(resource.rc.in ${rcfile})
add_library(${target_objs} OBJECT
  ${rcfile}
  STEP_mid.cpp
  SMFUtil.cpp)

if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static")
  add_library(${target_lib} STATIC $<TARGET_OBJECTS:${target_objs}>)
  set(expfile "${CMAKE_CURRENT_BINARY_DIR}/STEP_mid.exp.obj")
  add_custom_command(
    OUTPUT ${expfile}
    COMMAND ${CMAKE_DLLTOOL} -e ${expfile} --add-stdcall-underscore $<TARGET_FILE:${target_lib}>
    DEPENDS ${target_lib}
  )
  add_library(${target} MODULE
    STEP_SDK/STEPlugin.cpp
    ${expfile}
  )
  target_link_libraries(${target} ${target_lib})
else()
  add_library(${target} MODULE
    STEP_SDK/STEPlugin.cpp
    $<TARGET_OBJECTS:${target_objs}>
  )
endif()

set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS ${target} LIBRARY DESTINATION .)

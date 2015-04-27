cmake_minimum_required(VERSION 2.8)

include_directories(STEP_SDK ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS STEP_mid.cpp SMFUtil.cpp)
add_library(STEP_mid.objs OBJECT ${SRCS})
set(RCFILE "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
configure_file(resource.rc.in ${RCFILE})

if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static")
  add_library(STEP_mid.lib STATIC $<TARGET_OBJECTS:STEP_mid.objs>)
  set(EXPFILE STEP_mid.exp.obj)
  add_custom_command(OUTPUT ${EXPFILE} COMMAND ${CMAKE_DLLTOOL} -e ${EXPFILE} --add-stdcall-underscore $<TARGET_FILE:STEP_mid.lib> DEPENDS STEP_mid.lib)
  add_library(STEP_mid MODULE ${RCFILE} ${EXPFILE} STEP_SDK/STEPlugin.cpp)
  target_link_libraries(STEP_mid STEP_mid.lib)
else()
  add_library(STEP_mid MODULE ${RCFILE} $<TARGET_OBJECTS:STEP_mid.objs> STEP_SDK/STEPlugin.cpp)
endif()

set_target_properties(STEP_mid PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS STEP_mid LIBRARY DESTINATION .)

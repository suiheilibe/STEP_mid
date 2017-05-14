cmake_minimum_required(VERSION 2.8.11)

if(STEP_K)
  include_directories(STEP_K_SDK/STEP_SDK)
  set(steplugin "STEP_K_SDK/STEP_SDK/STEPlugin.cpp")
else()
  include_directories(STEP_SDK)
  set(steplugin "STEP_SDK/STEPlugin.cpp")
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR})
set(rcfile "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
configure_file(resource.rc.in ${rcfile})
add_library(${target_objs} OBJECT
  ${rcfile}
  STEP_mid.cpp
  SMFUtil.cpp
  ${steplugin})

if(MINGW)
  set(expfile "${CMAKE_CURRENT_BINARY_DIR}/STEP_mid.exp.obj")
  if(STEP_K)
    add_custom_command(
      OUTPUT ${expfile}
      COMMAND ${CMAKE_DLLTOOL} -e ${expfile} -d "${CMAKE_CURRENT_SOURCE_DIR}/STEP_mid_k.def"
    )
  else()
    add_custom_command(
      OUTPUT ${expfile}
      COMMAND ${CMAKE_DLLTOOL} -e ${expfile} --add-stdcall-underscore -d "${CMAKE_CURRENT_SOURCE_DIR}/STEP_mid_m.def"
    )
  endif()
  add_library(${target} MODULE ${expfile} $<TARGET_OBJECTS:${target_objs}>)
else()
  add_library(${target} MODULE $<TARGET_OBJECTS:${target_objs}>)
endif()

if(STEP_K)
  target_compile_definitions(${target} PRIVATE UNICODE=1 _UNICODE=1)
  target_compile_definitions(${target_objs} PRIVATE UNICODE=1 _UNICODE=1 STEP_K=1)
endif()

set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS ${target} LIBRARY DESTINATION .)

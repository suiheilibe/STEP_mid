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
  SMFUtil.cpp)

if(MINGW)
  add_library(${target_lib} STATIC $<TARGET_OBJECTS:${target_objs}>)
  set(expfile "${CMAKE_CURRENT_BINARY_DIR}/STEP_mid.exp.obj")
  if(STEP_K)
    add_custom_command(
      OUTPUT ${expfile}
      COMMAND ${CMAKE_DLLTOOL} -e ${expfile} -d "${CMAKE_CURRENT_SOURCE_DIR}/STEP_mid_k.def"
      DEPENDS ${target_lib}
    )
  else()
    add_custom_command(
      OUTPUT ${expfile}
      COMMAND ${CMAKE_DLLTOOL} -e ${expfile} --add-stdcall-underscore $<TARGET_FILE:${target_lib}>
      DEPENDS ${target_lib}
    )
  endif()
  add_library(${target} MODULE
    ${steplugin}
    ${expfile}
  )
  target_link_libraries(${target} ${target_lib})
else()
  add_library(${target} MODULE
    ${steplugin}
    $<TARGET_OBJECTS:${target_objs}>
  )
endif()

if(STEP_K)
  target_compile_definitions(${target} PRIVATE UNICODE=1 _UNICODE=1)
  target_compile_definitions(${target_objs} PRIVATE UNICODE=1 _UNICODE=1)
endif()

set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS ${target} LIBRARY DESTINATION .)

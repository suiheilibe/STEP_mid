if(STEP_K)
  include_directories(STEP_K_SDK/STEP_SDK)
  set(steplugin "STEP_K_SDK/STEP_SDK/STEPlugin.cpp")
else()
  include_directories(STEP_SDK)
  set(steplugin "STEP_SDK/STEPlugin.cpp")
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${target_objs} OBJECT
  resource.rc
  STEP_mid.cpp
  SMFUtil.cpp
  STEPMidUtil.cpp
  STEPMidTls.cpp
  ${steplugin})

if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static -Wl,--enable-stdcall-fixup")
  set(expfile "${CMAKE_CURRENT_BINARY_DIR}/STEP_mid.exp.obj")
  if(STEP_K)
    set(deffile "${CMAKE_CURRENT_SOURCE_DIR}/STEP_mid_k.def")
  else()
    set(deffile "${CMAKE_CURRENT_SOURCE_DIR}/STEP_mid_m.def")
  endif()
  add_custom_command(
    OUTPUT ${expfile}
    COMMAND ${CMAKE_DLLTOOL} -e "${expfile}" -d "${deffile}"
    DEPENDS ${deffile}
  )
  add_library(${target} MODULE ${expfile} $<TARGET_OBJECTS:${target_objs}>)
else()
  if(STEP_K)
    add_library(${target} MODULE $<TARGET_OBJECTS:${target_objs}> STEP_mid_k.def)
  else()
    add_library(${target} MODULE $<TARGET_OBJECTS:${target_objs}>)
  endif()
endif()

if(STEP_K)
  target_compile_definitions(${target} PRIVATE UNICODE=1 _UNICODE=1)
  target_compile_definitions(${target_objs} PRIVATE UNICODE=1 _UNICODE=1 STEP_K=1)
endif()

# Use .ste as suffix
set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX .ste)

install(TARGETS ${target} LIBRARY DESTINATION .)

cmake_minimum_required(VERSION 2.8)

include_directories(STEP_SDK)
set(SRCS STEP_mid.cpp SMFUtil.cpp STEP_SDK/STEPlugin.cpp)
set(STEP_mid_VERSION 0.1.1)
set(RCFILE "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc")
configure_file(resource.rc.in ${RCFILE})

if(NOT CMAKE_DLLTOOL)
  set(CMAKE_DLLTOOL dlltool)
endif()

if(MSVC)
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif()

if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static")
  add_library(STEP_mid.lib STATIC ${SRCS})
  set(EXPFILE STEP_mid.exp.obj)
  add_custom_command(OUTPUT ${EXPFILE} COMMAND ${CMAKE_DLLTOOL} -e ${EXPFILE} -U $<TARGET_FILE:STEP_mid.lib> DEPENDS STEP_mid.lib)
  add_library(STEP_mid MODULE ${RCFILE} ${EXPFILE})
  target_link_libraries(STEP_mid STEP_mid.lib)
else()
  add_library(STEP_mid MODULE ${RCFILE} ${SRCS})
endif()

set_target_properties(STEP_mid PROPERTIES PREFIX "" SUFFIX .ste)

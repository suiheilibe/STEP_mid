cmake_minimum_required(VERSION 2.8.11)

project(STEP_mid)
enable_testing()

execute_process(
  COMMAND git describe --tags
  OUTPUT_VARIABLE STEP_mid_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT CMAKE_DLLTOOL)
  set(CMAKE_DLLTOOL dlltool)
endif()

# TODO: Separate this block to an individual file
if(MINGW)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static")
  set(CMAKE_CXX_FLAGS  "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG  "-g -DDEBUG -D_DEBUG")
elseif(MSVC)
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif()

set(target STEP_mid)
set(target_lib "${target}.lib")
set(target_objs "${target}.objs")

add_subdirectory(src)
add_subdirectory(test)
install(FILES COPYING readme.txt DESTINATION .)

set(CPACK_GENERATOR ZIP)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${STEP_mid_VERSION}")
set(CPACK_STRIP_FILES TRUE)

include(CPack)

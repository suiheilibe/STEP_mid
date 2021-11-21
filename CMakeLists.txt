cmake_minimum_required(VERSION 3.12.4)

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

# Use C++11 features
set(CMAKE_CXX_STANDARD 11)

if(MINGW)
  set(CMAKE_CXX_FLAGS  "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG  "-g -DDEBUG -D_DEBUG")
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

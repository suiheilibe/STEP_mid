cmake_minimum_required(VERSION 2.8)

project(STEP_mid)

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

add_subdirectory(extra/hayai)
add_subdirectory(src)
add_subdirectory(test)

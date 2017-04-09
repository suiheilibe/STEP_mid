cmake_minimum_required(VERSION 2.8.7)

set(test_lib CppUTest)
if(MINGW)
  list(APPEND test_lib pthread)
endif()
set(test_lib_project cpputest)
set(test_lib_project_args "-DTESTS=OFF")

set(test_target "${target}.test")
set(test_name "STEP_mid")

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../src/STEP_SDK"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/lib")

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

add_executable(${test_target} test.cpp $<TARGET_OBJECTS:${target_objs}>)
target_link_libraries(${test_target} ${test_lib})

file(COPY test.mid DESTINATION .)

add_test(NAME ${test_name} COMMAND ${test_target})

include(ExternalProject)

ExternalProject_Add(
  ${test_lib_project}
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/${test_lib_project}"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
             "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
             ${test_lib_project_args}
)

add_dependencies(${test_target} ${test_lib_project})

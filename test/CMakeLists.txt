cmake_minimum_required(VERSION 2.8.11)

set(test_lib CppUTest)
if(MINGW)
  list(APPEND test_lib pthread)
endif()
set(test_lib_project cpputest)
set(test_lib_project_args "-DTESTS=OFF")

set(test_target "${target}.test")
set(test_name "STEP_mid")

if(STEP_K)
  include_directories(../src/STEP_K_SDK/STEP_SDK)
else()
  include_directories(../src/STEP_SDK)
endif()

include_directories(
  ../src
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/lib")

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

add_executable(${test_target} test.cpp $<TARGET_OBJECTS:${target_objs}>)
target_link_libraries(${test_target} ${test_lib})

if(STEP_K)
  target_compile_definitions(${test_target} PRIVATE UNICODE=1 _UNICODE=1)
endif()

file(GLOB test_files "*.mid")
file(COPY ${test_files} DESTINATION .)

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

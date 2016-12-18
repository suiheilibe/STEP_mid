cmake_minimum_required(VERSION 2.8)

set(testlib CppUTest)
if(MINGW)
  list(APPEND testlib pthread)
endif()
set(testlib_project cpputest)
set(testlib_project_args "-DTESTS=OFF")

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../src/STEP_SDK"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/lib")

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

add_executable(STEP_mid.test test.cpp $<TARGET_OBJECTS:STEP_mid.objs>)
target_link_libraries(STEP_mid.test ${testlib})

file(COPY test.mid DESTINATION .)

add_test(NAME "STEP_mid" COMMAND STEP_mid.test)

include(ExternalProject)

ExternalProject_Add(
  ${testlib_project}
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/${testlib_project}"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
             "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
             ${testlib_project_args}
)

add_dependencies(STEP_mid.test ${testlib_project})

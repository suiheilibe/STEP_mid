cmake_minimum_required(VERSION 2.8)

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
target_link_libraries(STEP_mid.test CppUTest)

file(COPY test.mid DESTINATION .)

add_test(NAME "STEP_mid" COMMAND STEP_mid.test)

include(ExternalProject)

ExternalProject_Add(
  cpputest
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/cpputest"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
             "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
)

add_dependencies(STEP_mid.test cpputest)

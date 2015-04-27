cmake_minimum_required(VERSION 2.8)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/STEP_SDK
  ${CMAKE_CURRENT_SOURCE_DIR}/../extra/hayai/src
)

add_executable(STEP_mid.test test.cpp $<TARGET_OBJECTS:STEP_mid.objs>)
target_link_libraries(STEP_mid.test ${CMAKE_CURRENT_BINARY_DIR}/../extra/hayai/src/libhayai_main.a)
add_dependencies(STEP_mid.test hayai_main)

add_test(NAME "STEP_mid" COMMAND STEP_mid.test)

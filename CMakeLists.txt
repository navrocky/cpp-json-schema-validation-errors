cmake_minimum_required(VERSION 3.5)

project(json-schema-validator-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D__RESOURCES_DIR__=${CMAKE_CURRENT_SOURCE_DIR}/resources)

find_package(Catch2 3 REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)

add_executable(tests test.cpp
  README.md)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain nlohmann_json_schema_validator)

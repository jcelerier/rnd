cmake_minimum_required(VERSION 3.9)
project(rnd LANGUAGES CXX)

add_library(rnd
  src/random.cpp
  include/rnd/random.hpp
)
add_library(rnd::rnd ALIAS rnd)

target_include_directories(rnd PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(rnd PUBLIC $<BUILD_INTERFACE:cxx_std_17>)

include(GenerateExportHeader)
generate_export_header(rnd)

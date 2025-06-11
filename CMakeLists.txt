cmake_minimum_required(VERSION 3.22)
project(divinus)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE DIVINUS_SRC_LIST
        3rd/divinus/src/hal/*
        3rd/divinus/src/error.c
        3rd/divinus/src/compat.c
)

add_library(hal ${DIVINUS_SRC_LIST})
include_directories(3rd/divinus/src/)

set(ENABLE_TESTS OFF CACHE BOOL "")
set(ENABLE_MP4 OFF CACHE BOOL "")
set(ENABLE_SRT OFF CACHE BOOL "")
set(ENABLE_HLS OFF CACHE BOOL "")
set(ENABLE_MP4 OFF CACHE BOOL "")
add_subdirectory(3rd/ZLMediaKit)

include_directories(3rd/ZLMediaKit/src)
include_directories(3rd/ZLMediaKit/3rdpart/ZLToolKit/src)

add_executable(TalusIPC
        src/main.cpp
        src/media.cpp
)
target_link_libraries(TalusIPC hal zlmediakit ext-codec)
